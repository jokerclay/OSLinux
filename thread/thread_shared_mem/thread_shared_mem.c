#include <stdio.h>
#include <pthread.h>

/*

由于线程共享同一个进程的地址空间，通信本质上就是对同一变量的读写，但必须注意同步问题。

共享变量（最常用）

*/

int counter = 0;

/*
❗这里会出现竞态条件，两个线程对共享变量 counter 同时修改，会导致结果不可预测。
*/

void* thread_func(void* arg) {
    for (int i = 0; i < 100000; ++i)
        counter++; // 非线程安全
    return NULL;
}

int main() {
    pthread_t t1;
    pthread_t t2;
    pthread_create(&t1, NULL, thread_func, NULL);
    pthread_create(&t2, NULL, thread_func, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("counter = %d\n", counter); // 可能不是200000，存在竞态
}

