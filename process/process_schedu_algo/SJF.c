/*

🧠 SJF 原理简介
* 非抢占式调度算法（我们先从非抢占的版本开始）。
* 在所有已到达的进程中，选择执行时间（Burst Time）最短的进程。
* 如果两个进程的到达时间相同，则选择执行时间短的。
* 这种策略可以有效降低平均等待时间，但可能导致“长进程饥饿”。


📋 示例数据（同 FCFS）
PID 到达时间    执行时间
1       0           5
2       1           3
3       2           8
4       3           6


*/

#include <stdio.h>

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int finish_time;
    int waiting_time;
    int turnaround_time;
    int completed;
} Process;

int find_next_process(Process *p, int n, int current_time) {
    int min_burst = 1e9;
    int idx = -1;
    for (int i = 0; i < n; ++i) {
        if (!p[i].completed && p[i].arrival_time <= current_time) {
            if (p[i].burst_time < min_burst) {
                min_burst = p[i].burst_time;
                idx = i;
            }
        }
    }
    return idx;
}

void calculate_sjf(Process *p, int n) {
    int completed = 0, current_time = 0;

    while (completed < n) {
        int idx = find_next_process(p, n, current_time);
        if (idx == -1) {
            current_time++; // 没有进程就绪，时间往后推
            continue;
        }

        p[idx].waiting_time = current_time - p[idx].arrival_time;
        p[idx].finish_time = current_time + p[idx].burst_time;
        p[idx].turnaround_time = p[idx].finish_time - p[idx].arrival_time;
        p[idx].completed = 1;

        current_time = p[idx].finish_time;
        completed++;
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

    calculate_sjf(processes, n);
    print_result(processes, n);
    return 0;
}
/*

🎯 总结：FCFS vs SJF
SJF 平均等待时间 更短
但它对长进程不友好（可能长时间不被调度）

*/
