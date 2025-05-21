#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork失败");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 子进程
        printf("子进程[%d]开始执行\n", getpid());

        while(1) {
            printf("子进程[%d]工作中...\n", getpid());
            sleep(1);
        }

        exit(EXIT_SUCCESS);
    } else {
        // 父进程
        printf("父进程[%d]创建了子进程[%d]\n", getpid(), pid);

        sleep(3); // 让子进程运行一会儿

        printf("父进程: 发送SIGSTOP挂起子进程\n");
        kill(pid, SIGSTOP); // 挂起子进程

        sleep(2);

        printf("父进程: 发送SIGCONT恢复子进程\n");
        kill(pid, SIGCONT); // 恢复子进程

        sleep(2);

        printf("父进程: 终止子进程\n");
        kill(pid, SIGTERM); // 终止子进程

        waitpid(pid, NULL, 0); // 等待子进程结束
    }

    return 0;
}

