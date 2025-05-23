// single_thread.c
// 单线程依次处理 5 个任务

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

void handle_task(int id) {
    printf("[Task %d] doing heavy work...\n", id);
    usleep(2000000);  // 2000ms
    printf("[Task %d] writing shared data...\n", id);
    usleep(500000);   // 500ms
    printf("[Task %d] done\n", id);
}

int main() {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < 5; ++i) {
        handle_task(i + 1);
    }

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) + 
                     (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Single-thread elapsed time: %.2f seconds\n", elapsed);
    return 0;
}

