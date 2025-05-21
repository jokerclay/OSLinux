#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

/*
操作系统会为每个进程维护一份 PCB（进程控制块，Process Control Block），
虽然不能直接访问 PCB（这是内核的数据结构），但可以通过 /proc/<pid>/ 文件系统，
查看用户层面能拿到的 PCB 信息

为什么不传 pid 给 print_proc_status() 也能得到当前进程信息？
    因为读取的是 /proc/self/status，它是自动指向当前进程的软链接。

self 是如何工作的？
    操作系统在访问时会将 /proc/self 映射到实际的 /proc/<当前PID>，每个进程看到的是自己的视角。

/proc/self/status 总是指“当前正在执行这个代码的进程”，也就是说，
不管是主进程（父进程）还是子进程，只要在那个进程中运行了 print_proc_status()，打印的就是它自己的状态。
    如果你在 父进程中调用 print_proc_status()，显示的是父进程；
    如果你在 子进程中调用 print_proc_status()，显示的是子进程；
    每个进程看到的 /proc/self 都是“以它自己为视角”。
*/

// 打印当前进程状态
void print_proc_status(pid_t pid) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("fopen");
        return;
    }

    char line[256];
    printf("========== /proc/%d/status ==========\n", pid);
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "Name:", 5) == 0 ||
            strncmp(line, "State:", 6) == 0 ||
            strncmp(line, "Pid:", 4) == 0 ||
            strncmp(line, "PPid:", 5) == 0 ||
            strncmp(line, "Threads:", 8) == 0 ||
            strncmp(line, "VmSize:", 7) == 0) {
            printf("%s", line);
        }
    }
    fclose(fp);
    printf("=====================================\n\n");
}


void print_line(int n)
{
    for(int i = 0;i < n; i++)
        printf("=");
    printf("\n");
}

void print_msg(const char *formated_msg)
{
    printf("\n[");
    printf("%s",formated_msg);
    printf("]\n");
}

int main() {

/*
    int main_pid = getpid();
    print_line(20);
    printf("main pid: %d \n",main_pid);
    print_line(20);
    print_proc_status();
    print_line(20);
*/

  pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // 子进程
        printf("[Child] Sleeping 5 seconds...\n");
        sleep(5);  // 模拟阻塞，父进程有时间去观测子进程状态
        printf("[Child] After sleep, my status:\n");
        print_proc_status(getpid());
        exit(0);
    } else {
        // 父进程
        printf("[Parent] My status:\n");
        print_proc_status(getpid());

        printf("[Parent] Observing child process PID: %d\n", pid);
        sleep(1);  // 等子进程进入 sleep 状态
        print_proc_status(pid);

        printf("[Parent] Waiting for child...\n");
        wait(NULL);
        printf("[Parent] Child has exited.\n");
    }



    /*
    pid_t pid = fork();
    printf("pid = %d, getpid = %d\n", pid, getpid());
    */

}




