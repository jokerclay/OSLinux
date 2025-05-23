/*

📝 一、原理简介
抢占式优先级调度是一种根据进程优先级决定调度顺序的算法。它的特点是：
    * 实时检测是否有新进程到达；
    * 若新来的进程优先级更高（优先级数值更小），立即抢占当前运行的进程；
    * 进程继续执行直到被抢占或执行完毕。

和 抢占式 SJF 非常相似，不同点在于判断是否抢占是基于优先级，而不是剩余时间。

📊 示例数据
PID 到达时间    执行时间    优先级
1       0           10          2
2       2           4           1
3       4           6           3

*/


#include <stdio.h>

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
    int finish_time;
    int waiting_time;
    int turnaround_time;
    int started;
} Process;

int all_done(Process p[], int n) {
    for (int i = 0; i < n; ++i)
        if (p[i].remaining_time > 0) return 0;
    return 1;
}

int select_process(Process p[], int n, int time) {
    int idx = -1;
    int min_priority = 9999;
    for (int i = 0; i < n; ++i) {
        if (p[i].arrival_time <= time && p[i].remaining_time > 0) {
            if (p[i].priority < min_priority) {
                min_priority = p[i].priority;
                idx = i;
            }
        }
    }
    return idx;
}

void preemptive_priority(Process p[], int n) {
    int time = 0;

    while (!all_done(p, n)) {
        int idx = select_process(p, n, time);
        if (idx == -1) {
            time++; // idle time
            continue;
        }

        if (!p[idx].started) p[idx].started = 1;

        p[idx].remaining_time--;
        time++;

        if (p[idx].remaining_time == 0) {
            p[idx].finish_time = time;
            p[idx].turnaround_time = p[idx].finish_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
        }
    }
}

void print_result(Process p[], int n) {
    float total_wait = 0, total_turn = 0;
    printf("PID\tArrival\tBurst\tPrio\tFinish\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; i++) {
        total_wait += p[i].waiting_time;
        total_turn += p[i].turnaround_time;
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].priority,
               p[i].finish_time, p[i].waiting_time, p[i].turnaround_time);
    }
    printf("Average Waiting Time: %.2f\n", total_wait / n);
    printf("Average Turnaround Time: %.2f\n", total_turn / n);
}

int main() {
    Process processes[] = {
        {1, 0, 10, 10, 2, 0, 0, 0, 0},
        {2, 2, 4, 4, 1, 0, 0, 0, 0},
        {3, 4, 6, 6, 3, 0, 0, 0, 0},
    };
    int n = sizeof(processes) / sizeof(Process);

    preemptive_priority(processes, n);
    print_result(processes, n);
    return 0;
}


/*
📚 总结
特性            内容
类型            抢占式
决策依据        优先级（数值越小优先级越高）
是否公平        不一定，低优先级进程可能饿死
实用性          比非抢占式更适合需要响应速度的任务调度

*/

