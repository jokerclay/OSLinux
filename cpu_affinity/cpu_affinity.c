/*
🎯 What is CPU Affinity?
CPU Affinity (also called processor affinity) is the ability to bind a process (or thread) to one or more specific CPU cores.

In other words:

“I want this process or thread to run only on CPU core 0 (or core 2,3...) — not on any CPU the OS picks.”

🧠 Why use CPU Affinity?
Here are some real reasons to set CPU affinity:

Reason              Explanation
Performance         Reduce cache misses by keeping a process on the same core (cache locality).
Isolation           Pin critical real-time processes to dedicated cores.
Debugging/Profiling Easier to track execution and performance when bound to fixed cores.
Avoid contention    Prevent certain processes from running on shared or busy CPUs.

🛠️ How to Set CPU Affinity in Linux (C Example)
You can use sched_setaffinity() from <sched.h>.

👇 Example: Bind process to CPU 0
*/


#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sched.h>

#define BINDED_CPU_CORE 1

int main() {
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(BINDED_CPU_CORE, &set);  // Bind to CPU core 0

    pid_t pid = getpid();
    if (sched_setaffinity(pid, sizeof(set), &set) == -1) {
        perror("sched_setaffinity");
        return 1;
    }

    printf("Process %d bound to CPU %d\n", pid,BINDED_CPU_CORE);

    while (1);  // Infinite loop so you can check with 'htop' or 'top'
}


/*

💡 Run it:
$ gcc -o affinity affinity.c
$ ./affinity

Then in another terminal, use:
$ htop

Press F2 → Display options → Show CPU core
You’ll see the process runs only on CPU 0.

🔍 Check Affinity:
Use taskset:
$ taskset -cp <pid>
Example:
$taskset -cp 1234
# pid 1234's current affinity: 0

⚠️  Notes:
By default, Linux schedules processes on any available CPU.

You can set per-thread affinity (with pthread + pthread_setaffinity_np).

taskset command (CLI) can also run a program with specific affinity.
taskset -c 0 ./your_program

*/
