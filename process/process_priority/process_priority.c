#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>
#include <time.h>

void print_sched_info(const char* name) {
    int policy = sched_getscheduler(0);
    struct sched_param param;
    sched_getparam(0, &param);

    const char* polname = "UNKNOWN";
    if (policy == SCHED_OTHER) polname = "SCHED_OTHER";
    else if (policy == SCHED_FIFO) polname = "SCHED_FIFO";
    else if (policy == SCHED_RR) polname = "SCHED_RR";

    printf("[%s] policy=%s priority=%d\n", name, polname, param.sched_priority);
}

void set_cpu_affinity(int cpu) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu, &cpuset);
    if (sched_setaffinity(0, sizeof(cpuset), &cpuset) == -1) {
        perror("sched_setaffinity failed");
        exit(1);
    }
}

void busy_loop(const char* name, int seconds) {
    long count = 0;
    time_t start, now;
    time(&start);

    while (1) {
        count++;
        sched_yield();  // 让出CPU，提高调度机会

        time(&now);
        if (now - start >= seconds) {
            printf("[%s] count = %ld\n", name, count);
            break;
        }
    }
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        set_cpu_affinity(0);

        struct sched_param param;
        param.sched_priority = 80;  // 高优先级
        if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
            perror("child sched_setscheduler failed");
            exit(1);
        }

        print_sched_info("child");
        busy_loop("child", 10);
        exit(0);
    } else {
        set_cpu_affinity(0);

        struct sched_param param;
        param.sched_priority = 50;  // 低优先级
        if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
            perror("parent sched_setscheduler failed");
            // 继续运行
        }

        print_sched_info("parent");
        busy_loop("parent", 10);

        wait(NULL);
    }

    return 0;
}

