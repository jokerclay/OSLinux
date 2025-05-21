#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main() {
    int shmid;
    key_t key = 1234; // 共享内存键值
    char *shm, *s;

    // 创建共享内存段
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget失败");
        exit(1);
    }

    // 将共享内存附加到进程地址空间
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat失败");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork失败");
        exit(1);
    } else if (pid == 0) {
        // 子进程(消费者)
        printf("子进程[%d]等待数据...\n", getpid());

        while (*shm != '*') // 等待生产者写入结束标记
            sleep(1);

        printf("子进程收到数据: ");
        for (s = shm; *s != '\0'; s++)
            putchar(*s);
        putchar('\n');

        *shm = '#'; // 通知生产者已完成读取
    } else {
        // 父进程(生产者)
        s = shm;
        char data[] = "Hello from parent process!";

        printf("父进程[%d]写入数据...\n", getpid());
        strncpy(s, data, sizeof(data));
        *s = '*'; // 写入结束标记

        while (*shm != '#') // 等待消费者确认
            sleep(1);

        // 分离并删除共享内存
        shmdt(shm);
        shmctl(shmid, IPC_RMID, NULL);
        printf("父进程: 通信完成\n");
    }

    return 0;
}

