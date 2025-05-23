/*

🧠 Round Robin 原理简介
* 每个进程被分配一个固定的时间片（Time Quantum）。
* 按顺序依次运行进程，如果在时间片内没有完成，就将它放到队列末尾等待下一轮。
* 是一种公平的调度策略，适合响应性要求较高的系统。
* 实现上要模拟一个循环队列，不断从就绪队列头部取出进程运行。

📋 示例数据（与前面一致）
PID 到达时间    执行时间
1       0           5
2       1           3
3       2           8
4       3           6

我们假设：时间片 = 2

*/


#include <stdio.h>

#define TIME_QUANTUM 2

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int finish_time;
    int waiting_time;
    int turnaround_time;
} Process;

int all_done(Process *p, int n) {
    for (int i = 0; i < n; ++i)
        if (p[i].remaining_time > 0)
            return 0;
    return 1;
}

void round_robin(Process *p, int n) {
    int time = 0;
    int queue[1000]; // 简单就绪队列
    int front = 0, rear = 0;
    int visited[100] = {0};

    while (!all_done(p, n)) {
        // 把到达的进程放入队列
        for (int i = 0; i < n; ++i) {
            if (p[i].arrival_time <= time && !visited[i] && p[i].remaining_time > 0) {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }

        if (front == rear) {
            time++;
            continue;
        }

        int idx = queue[front++];
        int exec_time = (p[idx].remaining_time < TIME_QUANTUM) ? p[idx].remaining_time : TIME_QUANTUM;
        p[idx].remaining_time -= exec_time;
        time += exec_time;

        // 检查有无新进程到达
        for (int i = 0; i < n; ++i) {
            if (p[i].arrival_time <= time && !visited[i] && p[i].remaining_time > 0) {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }

        if (p[idx].remaining_time > 0) {
            queue[rear++] = idx; // 再次入队
        } else {
            p[idx].finish_time = time;
        }
    }

    // 计算等待时间和周转时间
    for (int i = 0; i < n; ++i) {
        p[i].turnaround_time = p[i].finish_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
    }
}

void print_result(Process *p, int n) {
    float total_wait = 0, total_turn = 0;
    printf("PID\tArrival\tBurst\tFinish\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; ++i) {
        total_wait += p[i].waiting_time;
        total_turn += p[i].turnaround_time;
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].arrival_time, p[i].burst_time,
               p[i].finish_time, p[i].waiting_time, p[i].turnaround_time);
    }
    printf("Average Waiting Time: %.2f\n", total_wait / n);
    printf("Average Turnaround Time: %.2f\n", total_turn / n);
}

int main() {
    Process processes[] = {
        {1, 0, 5, 5},
        {2, 1, 3, 3},
        {3, 2, 8, 8},
        {4, 3, 6, 6}
    };
    int n = sizeof(processes) / sizeof(Process);

    round_robin(processes, n);
    print_result(processes, n);
    return 0;
}

/*

RR 调度更加公平，不会出现长进程“饿死”的问题。
缺点是频繁上下文切换，系统开销会变大。
时间片设置得太小，上下文切换频繁；太大又退化成 FCFS。

*/
