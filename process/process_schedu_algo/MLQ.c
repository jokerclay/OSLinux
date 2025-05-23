/*

**多级队列调度算法（MLQ, Multi-Level Queue Scheduling）**。

---

## 🧠 一、MLQ（多级队列调度）算法简介

多级队列调度将所有进程 **分为多个队列**，每个队列使用 **不同的调度策略**，并具有不同的 **优先级等级**。

### 🌟 特点：

* **分队列**：如前台交互进程、后台批处理、系统任务……
* **固定分队列**：一个进程一生在一个队列中。
* **优先级高的队列先调度**。
* 每个队列可使用不同调度算法（如 RR、FCFS）。
* 各队列之间不抢占，除非设置支持。

---

## ✅ 总结

| 特性         | 描述                                         |
| ------       | -------------------------                    |
| 策略         | 多队列，每队列不同调度方式                   |
| 队列优先级   | 高优先级队列先调度                           |
| 队列静态分配 | 每个进程固定属于某一队列                     |
| 应用场景     | 系统任务、前台任务、后台任务区分调度         |
| 不足         | 低优先级队列容易 **饿死**（除非设置补偿机制）|

---

✅  “多级反馈队列调度（MLFQ）”，那会更复杂一些 —— 它允许进程在不同队列之间移动，模拟一个更智能的、适应性的调度系统。


*/

#include <stdio.h>

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int queue; // 0: Q0, 1: Q1, 2: Q2
    int start_time;
    int finish_time;
    int waiting_time;
    int turnaround_time;
    int completed;
} Process;

#define N 5
#define TIME_QUANTUM_Q0 2

Process p[N] = {
    {1, 0, 5, 5, 0},
    {2, 1, 3, 3, 1},
    {3, 2, 6, 6, 2},
    {4, 3, 4, 4, 0},
    {5, 4, 5, 5, 2},
};

int all_done() {
    for (int i = 0; i < N; ++i)
        if (!p[i].completed) return 0;
    return 1;
}

void mlq_schedule() {
    int time = 0;

    while (!all_done()) {
        int found = 0;

        // Q0: Round-Robin
        for (int i = 0; i < N; ++i) {
            if (!p[i].completed && p[i].queue == 0 && p[i].arrival_time <= time) {
                found = 1;
                int slice = (p[i].remaining_time > TIME_QUANTUM_Q0) ? TIME_QUANTUM_Q0 : p[i].remaining_time;
                time += slice;
                p[i].remaining_time -= slice;
                if (p[i].remaining_time == 0) {
                    p[i].finish_time = time;
                    p[i].turnaround_time = time - p[i].arrival_time;
                    p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
                    p[i].completed = 1;
                }
            }
        }

        if (found) continue;

        // Q1: FCFS
        for (int i = 0; i < N; ++i) {
            if (!p[i].completed && p[i].queue == 1 && p[i].arrival_time <= time) {
                found = 1;
                time += p[i].burst_time;
                p[i].remaining_time = 0;
                p[i].finish_time = time;
                p[i].turnaround_time = time - p[i].arrival_time;
                p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
                p[i].completed = 1;
                break;
            }
        }

        if (found) continue;

        // Q2: FCFS
        for (int i = 0; i < N; ++i) {
            if (!p[i].completed && p[i].queue == 2 && p[i].arrival_time <= time) {
                found = 1;
                time += p[i].burst_time;
                p[i].remaining_time = 0;
                p[i].finish_time = time;
                p[i].turnaround_time = time - p[i].arrival_time;
                p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
                p[i].completed = 1;
                break;
            }
        }

        if (!found) time++; // 没有进程可调度时推进时间
    }
}

void print_result() {
    float total_wait = 0, total_turn = 0;
    printf("PID\tQueue\tArrival\tBurst\tFinish\tWaiting\tTurnaround\n");
    for (int i = 0; i < N; ++i) {
        total_wait += p[i].waiting_time;
        total_turn += p[i].turnaround_time;
        printf("%d\tQ%d\t%d\t%d\t%d\t%d\t%d\n",
            p[i].pid, p[i].queue, p[i].arrival_time, p[i].burst_time,
            p[i].finish_time, p[i].waiting_time, p[i].turnaround_time);
    }

    printf("Average Waiting Time: %.2f\n", total_wait / N);
    printf("Average Turnaround Time: %.2f\n", total_turn / N);
}

int main() {
    mlq_schedule();
    print_result();
    return 0;
}
