#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe创建失败");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork失败");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 子进程
        close(pipefd[1]); // 关闭写端

        printf("子进程[%d]等待父进程信号...\n", getpid());

        char buf;
        read(pipefd[0], &buf, 1); // 阻塞读取(等待父进程写入)

        printf("子进程[%d]收到信号，开始工作\n", getpid());
        sleep(1);
        printf("子进程[%d]工作完成\n", getpid());

        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    } else {
        // 父进程
        close(pipefd[0]); // 关闭读端

        printf("父进程[%d]正在准备数据...\n", getpid());
        sleep(2);

        printf("父进程: 唤醒子进程\n");
        write(pipefd[1], "G", 1); // 写入数据唤醒子进程

        close(pipefd[1]);
        waitpid(pid, NULL, 0); // 等待子进程结束
    }

    return 0;
}

