/*

**多级反馈队列调度算法（MLFQ, Multi-Level Feedback Queue Scheduling）** —— 这是现代操作系统中非常重要的一种调度算法。

---

## 🧠 一、MLFQ 概念简介

### 📌 什么是 MLFQ？

> MLFQ 是一种动态优先级调度算法，它允许进程在多个优先级队列之间**上下浮动**，以实现“对短作业更友好”和“防止长作业饿死”的目标。

---

## 🎯 二、MLFQ 核心特点

| 特性                 | 描述                                              |
| ----------           | --------------------------                        |
| 多个优先级队列       | 通常队列越上面优先级越高                          |
| 每个队列调度算法不同 | 通常使用 RR，时间片从高优到低依次变长             |
| 动态迁移             | 若某进程在高优先级队列未完成 → 降级到低优先级队列 |
| 防止饿死机制（可选） | 可定期**提升**低优先级队列中的进程到更高优先级    |

---

## 📊 三、我们用以下配置举例说明：

* 3 个队列（Q0、Q1、Q2）：

  * Q0：时间片 2，Round Robin
  * Q1：时间片 4，Round Robin
  * Q2：FCFS，不抢占

* 初始进程都进入 Q0。

* 执行完时间片没完成就降级（Q0 → Q1 → Q2）

---

## ✅ 总结：MLFQ 的关键点

| 优点             | 缺点              |
| -------------- | --------------- |
| 对短作业非常友好       | 需要复杂的参数调整       |
| 支持动态优先级，防止饿死   | 实现比 FCFS、RR 更复杂 |
| 模拟多种类型任务优先级    | 可能存在不公平调度风险     |
| 高效用于交互+批处理混合系统 |                 |
---

*/

#include <stdio.h>

#define N 4
#define TIME_Q0 2
#define TIME_Q1 4

typedef struct {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int queue_level; // 0,1,2
    int completed;
    int start_time;
    int finish_time;
    int waiting_time;
    int turnaround_time;
} Process;

Process p[N] = {
    {1, 0, 10, 10, 0},
    {2, 1, 4, 4, 0},
    {3, 2, 6, 6, 0},
    {4, 3, 8, 8, 0},
};

int time = 0;

int all_done() {
    for (int i = 0; i < N; i++)
        if (!p[i].completed) return 0;
    return 1;
}

void schedule() {
    while (!all_done()) {
        int executed = 0;

        // Queue 0: RR with 2 units
        for (int i = 0; i < N; i++) {
            if (!p[i].completed && p[i].queue_level == 0 && p[i].arrival <= time) {
                executed = 1;
                int slice = (p[i].remaining > TIME_Q0) ? TIME_Q0 : p[i].remaining;
                time += slice;
                p[i].remaining -= slice;
                if (p[i].remaining == 0) {
                    p[i].completed = 1;
                    p[i].finish_time = time;
                    p[i].turnaround_time = time - p[i].arrival;
                    p[i].waiting_time = p[i].turnaround_time - p[i].burst;
                } else {
                    p[i].queue_level = 1; // demote
                }
            }
        }

        if (executed) continue;

        // Queue 1: RR with 4 units
        for (int i = 0; i < N; i++) {
            if (!p[i].completed && p[i].queue_level == 1 && p[i].arrival <= time) {
                executed = 1;
                int slice = (p[i].remaining > TIME_Q1) ? TIME_Q1 : p[i].remaining;
                time += slice;
                p[i].remaining -= slice;
                if (p[i].remaining == 0) {
                    p[i].completed = 1;
                    p[i].finish_time = time;
                    p[i].turnaround_time = time - p[i].arrival;
                    p[i].waiting_time = p[i].turnaround_time - p[i].burst;
                } else {
                    p[i].queue_level = 2; // demote again
                }
            }
        }

        if (executed) continue;

        // Queue 2: FCFS no time slicing
        for (int i = 0; i < N; i++) {
            if (!p[i].completed && p[i].queue_level == 2 && p[i].arrival <= time) {
                executed = 1;
                time += p[i].remaining;
                p[i].remaining = 0;
                p[i].completed = 1;
                p[i].finish_time = time;
                p[i].turnaround_time = time - p[i].arrival;
                p[i].waiting_time = p[i].turnaround_time - p[i].burst;
            }
        }

        if (!executed) time++; // No one ready
    }
}

void result() {
    float avg_wait = 0, avg_turn = 0;
    printf("PID\tArrival\tBurst\tFinish\tWait\tTurnaround\n");
    for (int i = 0; i < N; i++) {
        avg_wait += p[i].waiting_time;
        avg_turn += p[i].turnaround_time;
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].arrival, p[i].burst,
               p[i].finish_time, p[i].waiting_time, p[i].turnaround_time);
    }
    printf("Average Waiting Time: %.2f\n", avg_wait / N);
    printf("Average Turnaround Time: %.2f\n", avg_turn / N);
}


int main() {
    schedule();
    result();
    return 0;
}




