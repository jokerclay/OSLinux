#!/bin/bash

banner() {
    local msg="$1"
    echo "--------------------------------------------------------"
    printf "                      %s\n" "$msg"
    echo "--------------------------------------------------------"
}

banner "先来先服务 (FCFS)"
set -xe
gcc -g FCFS.c -o FCFS
./FCFS
set +xe

banner "短作业优先 (SJF)"
set -xe
gcc -g SJF.c -o SJF
./SJF
set +xe

banner "时间片轮转 (RR)"
set -xe
gcc -g RR.c -o RR
./RR
set +xe


banner "优先级调度算法-非抢占式"
set -xe
gcc -g priority_scheduling_algorithm_Non-preemptive.c -o priority_scheduling_algorithm_Non-preemptive
./priority_scheduling_algorithm_Non-preemptive
set +xe


banner "优先级调度算法-抢占式"
set -xe
gcc -g priority_scheduling_algorithm_preemptive.c -o priority_scheduling_algorithm_preemptive
./priority_scheduling_algorithm_preemptive
set +xe



banner "高相应比优先 (HRRN)"
set -xe
gcc -g HRRN.c -o HRRN
./HRRN
set +xe


banner "多级队列调度 (MLQ)"
set -xe
gcc -g MLQ.c -o MLQ
./MLQ
set +xe

banner "多级反馈队列调度 (MLFQ)"
set -xe
gcc -g MLFQ.c -o MLFQ
./MLFQ
set +xe

