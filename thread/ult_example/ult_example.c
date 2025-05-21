#define _XOPEN_SOURCE 700
#include <ucontext.h>
#include <stdio.h>

#define STACK_SIZE 8192

ucontext_t main_ctx, ctx1, ctx2;

void thread1() {
    for (int i = 0; i < 5; i++) {
        printf("[Thread 1] Iteration %d\n", i);
        swapcontext(&ctx1, &ctx2);  // yield to thread 2
    }
    printf("[Thread 1] Finished.\n");
}

void thread2() {
    for (int i = 0; i < 5; i++) {
        printf("                 [Thread 2] Iteration %d\n", i);
        swapcontext(&ctx2, &ctx1);  // yield to thread 1
    }
    printf("                 [Thread 2] Finished.\n");
    setcontext(&main_ctx); // go back to main
}

int main() {
    char stack1[STACK_SIZE];
    char stack2[STACK_SIZE];

    getcontext(&ctx1);
    ctx1.uc_stack.ss_sp = stack1;
    ctx1.uc_stack.ss_size = STACK_SIZE;
    ctx1.uc_link = &main_ctx;
    makecontext(&ctx1, thread1, 0);

    getcontext(&ctx2);
    ctx2.uc_stack.ss_sp = stack2;
    ctx2.uc_stack.ss_size = STACK_SIZE;
    ctx2.uc_link = &main_ctx;
    makecontext(&ctx2, thread2, 0);

    printf(">>> Starting user-level threads\n");
    swapcontext(&main_ctx, &ctx1);
    printf(">>> Back to main, all done.\n");

    return 0;
}

