// multi_thread.c
// 多线程并发处理 5 个任务（加锁模拟共享）

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

pthread_mutex_t lock;

void* handle_task(void* arg) {
    int id = *(int*)arg;
    printf("[Task %d] doing heavy work...\n", id);
    usleep(2000000);  // 2000ms

    pthread_mutex_lock(&lock);
    printf("[Task %d] writing shared data...\n", id);
    usleep(500000);   // 500ms
    pthread_mutex_unlock(&lock);

    printf("[Task %d] done\n", id);
    return NULL;
}

int main() {
    pthread_t threads[5];
    int ids[5];
    pthread_mutex_init(&lock, NULL);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < 5; ++i) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, handle_task, &ids[i]);
    }
    for (int i = 0; i < 5; ++i) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) + 
                     (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Multi-thread elapsed time: %.2f seconds\n", elapsed);

    pthread_mutex_destroy(&lock);
    return 0;
}

