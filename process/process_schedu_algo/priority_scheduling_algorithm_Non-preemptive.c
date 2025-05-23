/*

🧠 一、原理简介
优先级调度算法会根据进程的优先级来选择下一个运行的进程：
每个进程都有一个优先级值（数值越小表示优先级越高，或相反，这取决于实现）。
CPU 会选择就绪队列中优先级最高的进程来执行。
可以是：
    非抢占式（Non-preemptive）：当前进程运行完之前不会被打断。
    抢占式（Preemptive）：如果来了一个更高优先级的进程，会抢占当前执行的进程。

我们先学习非抢占式优先级调度，它实现起来更简单，便于理解。

🧾 示例数据（加上优先级）
PID 到达时间    执行时间    优先级
1       0           5           2
2       1           3           1
3       2           8           4gg
4       3           6           3

*/

#include <stdio.h>

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int finish_time;
    int waiting_time;
    int turnaround_time;
    int started;
} Process;

void sort_by_arrival(Process p[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                Process tmp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = tmp;
            }
}

int find_next_process(Process p[], int n, int time) {
    int min_priority = 9999;
    int idx = -1;
    for (int i = 0; i < n; ++i) {
        if (!p[i].started && p[i].arrival_time <= time) {
            if (p[i].priority < min_priority) {
                min_priority = p[i].priority;
                idx = i;
            }
        }
    }
    return idx;
}

void priority_schedule(Process p[], int n) {
    int time = 0;
    int completed = 0;

    sort_by_arrival(p, n);

    while (completed < n) {
        int idx = find_next_process(p, n, time);
        if (idx == -1) {
            time++; // idle
            continue;
        }
        p[idx].started = 1;
        time += p[idx].burst_time;
        p[idx].finish_time = time;
        p[idx].turnaround_time = p[idx].finish_time - p[idx].arrival_time;
        p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
        completed++;
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
        {1, 0, 5, 2, 0, 0, 0, 0},
        {2, 1, 3, 1, 0, 0, 0, 0},
        {3, 2, 8, 4, 0, 0, 0, 0},
        {4, 3, 6, 3, 0, 0, 0, 0},
    };
    int n = sizeof(processes) / sizeof(Process);

    priority_schedule(processes, n);
    print_result(processes, n);
    return 0;
}

/*

✅ 特点总结
优点                    缺点
简单，易实现            优先级低的进程可能饿死
可以处理更紧急的任务    可能不公平，尤其是非抢占式

*/

