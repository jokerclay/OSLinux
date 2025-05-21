#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define MSG_SIZE 128

// 消息结构
struct msg_buffer {
    long msg_type;
    char msg_text[MSG_SIZE];
};

int main() {
    key_t key = 1234; // 消息队列键值
    int msgid;
    struct msg_buffer message;

    // 创建消息队列
    if ((msgid = msgget(key, 0666 | IPC_CREAT)) == -1) {
        perror("msgget失败");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork失败");
        exit(1);
    } else if (pid == 0) {
        // 子进程(接收者)
        printf("子进程[%d]等待消息...\n", getpid());

        // 接收消息(阻塞)
        if (msgrcv(msgid, &message, sizeof(message.msg_text), 1, 0) == -1) {
            perror("msgrcv失败");
            exit(1);
        }

        printf("子进程收到消息: %s\n", message.msg_text);
        exit(EXIT_SUCCESS); // 子进程退出
    } else {
        // 父进程(发送者)
        sleep(1); // 确保接收者先等待

        message.msg_type = 1;
        strcpy(message.msg_text, "Hello from parent via message queue!");

        printf("父进程[%d]发送消息...\n", getpid());
        if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
            perror("msgsnd失败");
            exit(1);
        }

        int status;
        // 使用waitpid等待子进程结束
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid失败");
            exit(1);
        }

        if (WIFEXITED(status)) {
            printf("父进程: 子进程[%d]已退出，状态码=%d\n", pid, WEXITSTATUS(status));
        }

        // 删除消息队列
        msgctl(msgid, IPC_RMID, NULL);
        printf("父进程: 通信完成\n");
    }

    return 0;
}


/*

关键系统调用说明：
msgget() - 创建/获取消息队列
msgsnd() - 发送消息到队列
msgrcv() - 从队列接收消息
msgctl() - 控制消息队列(如删除)

*/
