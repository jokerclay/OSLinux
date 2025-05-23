/*

🧠 FCFS 原理回顾
* 先到的进程先执行，不抢占。
* 根据进程的 到达时间 排序，依次调度。
* 执行完一个进程后，再执行下一个。
* 很公平，但可能会出现「长进程挡路」的问题（Convoy effect）。

📋 示例数据（用于测试）
我们设定 4 个进程：

PID  到达时间    执行时间
1       0           5
2       1           3
3       2           8
4       3           6

*/


#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int finish_time;
} Process;

// 按照 arrival_time 升序排序
int cmp(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->arrival_time - p2->arrival_time;
}

void calculate_fcfs(Process *p, int n) {
    int current_time = 0;
    for (int i = 0; i < n; ++i) {
        if (current_time < p[i].arrival_time)
            current_time = p[i].arrival_time;

        p[i].waiting_time = current_time - p[i].arrival_time;
        p[i].finish_time = current_time + p[i].burst_time;
        p[i].turnaround_time = p[i].finish_time - p[i].arrival_time;

        current_time = p[i].finish_time;
    }
}

void print_result(Process *p, int n) {
    float total_wait = 0, total_turn = 0;
    printf("PID\tArrival\tBurst\tFinish\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; ++i) {
        total_wait += p[i].waiting_time;
        total_turn += p[i].turnaround_time;
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].arrival_time, p[i].burst_time,
               p[i].finish_time, p[i].waiting_time, p[i].turnaround_time);
    }
    printf("Average Waiting Time: %.2f\n", total_wait / n);
    printf("Average Turnaround Time: %.2f\n", total_turn / n);
}

int main() {
    Process processes[] = {
        {1, 0, 5},
        {2, 1, 3},
        {3, 2, 8},
        {4, 3, 6}
    };
    int n = sizeof(processes) / sizeof(Process);

    qsort(processes, n, sizeof(Process), cmp);
    calculate_fcfs(processes, n);
    print_result(processes, n);

    return 0;
}

