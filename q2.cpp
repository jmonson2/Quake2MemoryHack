#include <sys/uio.h>
#include <iostream>
#include <limits>
#include <stdlib.h>
pid_t getPid()
{
    char line[10];
    pid_t pid;
    FILE* cmd = popen("pidof -s quake2", "r");
    fgets(line, 10, cmd);
    pid = strtol(line, NULL, 10);

    pclose(cmd);
    return pid;
    
}

u_int64_t getHeap(pid_t pid)
{
    u_int64_t base = -1;
    char line[16];
    std::string s = "grep heap /proc/" + std::to_string(pid) +"/maps | cut -c 1-8";
    FILE* cmd = popen(s.c_str(), "r");
    fgets(line,16,cmd);
    base = strtol(line, NULL, 16);
    return base;
    
}
int main()
{
    u_int64_t shells = 0x67978;
    u_int64_t bullets = 0x6797c;
    u_int64_t cells = 0x67980;
    u_int64_t grenades = 0x67960;
    u_int64_t rockets = 0x67984;
    u_int64_t slugs = 0x67988;
    u_int64_t health = 0x53c75c;
    u_int64_t armor = 0x67934;

    pid_t pid = getPid();
    u_int64_t b_heap = getHeap(pid);

    int c_shells = std::numeric_limits<int>::max();
    int c_bullets = std::numeric_limits<int>::max();
    int c_cells = std::numeric_limits<int>::max();
    int c_grenades = std::numeric_limits<int>::max();
    int c_rockets = std::numeric_limits<int>::max();
    int c_slugs = std::numeric_limits<int>::max();
    int c_health = std::numeric_limits<int>::max();
    int c_armor = std::numeric_limits<int>::max();

    struct iovec local[8];
    struct iovec remote[8];

    ssize_t nread;


    local[0].iov_base = &c_shells;
    local[0].iov_len = sizeof(int);
    remote[0].iov_base = (void*)(b_heap+shells);
    remote[0].iov_len = sizeof(int);
    local[1].iov_base = &c_bullets;
    local[1].iov_len = sizeof(int);
    remote[1].iov_base = (void*)(b_heap+bullets);
    remote[1].iov_len = sizeof(int);
    local[2].iov_base = &c_cells;
    local[2].iov_len = sizeof(int);
    remote[2].iov_base = (void*)(b_heap+cells);
    remote[2].iov_len = sizeof(int);
    local[3].iov_base = &c_grenades;
    local[3].iov_len = sizeof(int);
    remote[3].iov_base = (void*)(b_heap+grenades);
    remote[3].iov_len = sizeof(int);
    local[4].iov_base = &c_rockets;
    local[4].iov_len = sizeof(rockets);
    remote[4].iov_base = (void*)(b_heap+rockets);
    remote[4].iov_len = sizeof(rockets);
    local[5].iov_base = &c_slugs;
    local[5].iov_len = sizeof(int);
    remote[5].iov_base = (void*)(b_heap+slugs);
    remote[5].iov_len = sizeof(int);
    local[6].iov_base = &c_health;
    local[6].iov_len = sizeof(int);
    remote[6].iov_base = (void*)(b_heap+health);
    remote[6].iov_len = sizeof(int);
    local[7].iov_base = &c_armor;
    local[7].iov_len = sizeof(int);
    remote[7].iov_base = (void*)(b_heap+armor);




    nread = process_vm_writev(pid, local, 8, remote, 8, 0);
    std::cout << "Bytes written: " << nread << std::endl;
    return 0;

}

