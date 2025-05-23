/*

# 🧠 高响应比优先调度（HRRN，Highest Response Ratio Next）

---

## 📝 一、原理简介

**高响应比优先调度（HRRN）** 是一种 **非抢占式** 的调度算法，综合考虑了进程的等待时间和服务时间，从而兼顾了 **公平性** 和 **响应速度**。

### 响应比公式：

$$
\text{响应比} = \frac{\text{等待时间} + \text{服务时间}}{\text{服务时间}} = 1 + \frac{\text{等待时间}}{\text{服务时间}}
$$

* 等待时间越长，响应比越大。
* 服务时间越短，响应比越大。
* 每次调度时选择响应比 **最大** 的进程运行。

---

## 📊 示例数据

| PID | 到达时间 | 执行时间 |
| --- | ----     | ----     |
| 1   | 0        | 8        |
| 2   | 1        | 4        |
| 3   | 2        | 2        |
| 4   | 3        | 1        |

---

## 🎯 总结

| 特性       | 描述                                          |
| -----      | ----------------------------                  |
| 类型       | 非抢占式                                      |
| 响应比公式 | $1 + \frac{等待时间}{服务时间}$               |
| 公平性     | 高：不会饿死低优先级任务                      |
| 动态优先级 | 等待时间越长响应比越大，自然提升调度优先级    |
| 实用性     | 很适合 **批处理系统** 和 **响应时间敏感场景** |

---

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

int all_done(Process p[], int n) {
    for (int i = 0; i < n; ++i)
        if (!p[i].completed) return 0;
    return 1;
}

int select_hrrn(Process p[], int n, int time) {
    float max_response_ratio = -1;
    int idx = -1;

    for (int i = 0; i < n; ++i) {
        if (!p[i].completed && p[i].arrival_time <= time) {
            int wait = time - p[i].arrival_time;
            float rr = (float)(wait + p[i].burst_time) / p[i].burst_time;
            if (rr > max_response_ratio) {
                max_response_ratio = rr;
                idx = i;
            }
        }
    }

    return idx;
}

void hrrn(Process p[], int n) {
    int time = 0;

    while (!all_done(p, n)) {
        int idx = select_hrrn(p, n, time);
        if (idx == -1) {
            time++;
            continue;
        }

        time += p[idx].burst_time;
        p[idx].finish_time = time;
        p[idx].turnaround_time = time - p[idx].arrival_time;
        p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
        p[idx].completed = 1;
    }
}

void print_result(Process p[], int n) {
    float total_wait = 0, total_turn = 0;
    printf("PID\tArrival\tBurst\tFinish\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; i++) {
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
    Process p[] = {
        {1, 0, 8, 0, 0, 0, 0},
        {2, 1, 4, 0, 0, 0, 0},
        {3, 2, 2, 0, 0, 0, 0},
        {4, 3, 1, 0, 0, 0, 0},
    };

    int n = sizeof(p) / sizeof(Process);

    hrrn(p, n);
    print_result(p, n);
    return 0;
}
