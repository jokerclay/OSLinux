---

## 🧩 调度算法列表和目标

| 算法文件名                                                 | 中文名称                          | 目标                |
| ----------------------------------------------------- | ----------------------------- | ----------------- |
| `FCFS.c`                                              | 先来先服务（First Come First Serve） | 按照到达时间顺序调度        |
| `RR.c`                                                | 时间片轮转（Round Robin）            | 每个进程分配一个时间片，轮流执行  |
| `SJF.c`                                               | 短作业优先（Shortest Job First）     | 优先执行最短的作业         |
| `priority_scheduling_algorithm.c`                     | 优先级调度                         | 根据优先级决定执行顺序       |
| `high_response_ratio_priority_scheduling_algorithm.c` | 高响应比优先调度                      | 综合等待时间与服务时间决定调度优先 |
| `multi-level_queue_scheduling_algorithm.c`            | 多级队列调度                        | 不同类型进程放入不同优先级队列中  |
| `multi-level_feedback_queue_scheduling_algorithm.c`   | 多级反馈队列调度                      | 支持进程动态“上下移动”调度队列  |

---

## ✅ 学习顺序（由浅入深）

1. FCFS
2. SJF
3. Priority Scheduling
4. RR
5. High Response Ratio
6. Multi-Level Queue
7. Multi-Level Feedback Queue

---

## ✅ 每种算法我们会：

1. 简要原理讲解
2. 使用结构体模拟进程信息（如 PID、到达时间、执行时间等）
3. 用 C 实现调度逻辑
4. 输出调度过程和平均等待时间、周转时间等指标
5. 可选：加上 Gantt 图输出

---

### 🔨 准备工作：统一进程结构（以便多算法复用）

```c
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
```

---

