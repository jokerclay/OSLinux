#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork失败");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 子进程
        printf("子进程[%d]开始执行\n", getpid());
        sleep(5); // 模拟子进程工作
        printf("子进程[%d]工作完成\n", getpid());
        exit(EXIT_SUCCESS); // 子进程退出
    } else {
        // 父进程
        printf("父进程[%d]等待子进程[%d]...\n", getpid(), pid);

        int status;
        waitpid(pid, &status, 0); // 阻塞等待子进程结束

        if (WIFEXITED(status)) {
            printf("父进程: 子进程[%d]已退出，状态码=%d\n", 
                   pid, WEXITSTATUS(status));
        }
    }

    return 0;
}
