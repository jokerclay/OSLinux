#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

// ANSI escape codes for colors
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

#define WORKLOAD 5

// Structure to pass data to our threads
typedef struct {
    int id;
    const char *message;
    int work_duration_ms;
    int sleep_duration_s;
    const char *color; // New field for color
} thread_data_t;

// This function will be executed by our first type of thread
void *worker_thread_routine(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    printf("%sThread %d (%s): Started.%s\n", data->color, data->id, data->message, RESET);

    for (int i = 0; i < WORKLOAD; i++) {
        printf("%sThread %d (%s): Working... count %d%s\n", data->color, data->id, data->message, i, RESET);
        usleep(data->work_duration_ms * 1000);
    }

    printf("%sThread %d (%s): Finished.%s\n", data->color, data->id, data->message, RESET);
    pthread_exit(NULL);
}

// This function will be executed by our "blocking" simulation thread
void *blocking_simulation_thread_routine(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    printf("%sThread %d (%s): Started. I'm going to 'block' (sleep) for %d seconds.%s\n",
           data->color, data->id, data->message, data->sleep_duration_s, RESET);

    sleep(data->sleep_duration_s);

    printf("%sThread %d (%s): Woke up and finished!%s\n", data->color, data->id, data->message, RESET);
    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2, thread3;
    thread_data_t data1, data2, data3;

    printf("Main: Program started. Creating threads.\n");

    // Setup data for thread 1 (concurrent worker)
    data1.id = 1;
    data1.message = "Concurrent Worker A";
    data1.work_duration_ms = 500; // Adjusted for quicker demo
    data1.sleep_duration_s = 0;
    data1.color = CYAN; // Assign a color

    // Setup data for thread 2 (blocking simulation)
    data2.id = 2;
    data2.message = "Blocking Simulator";
    data2.work_duration_ms = 0;
    data2.sleep_duration_s = 5; // Adjusted for quicker demo
    data2.color = MAGENTA; // Assign a different color

    // Setup data for thread 3 (another concurrent worker)
    data3.id = 3;
    data3.message = "Concurrent Worker B";
    data3.work_duration_ms = 700; // Adjusted for quicker demo
    data3.sleep_duration_s = 0;
    data3.color = YELLOW; // Assign another color

    // Create the threads
    if (pthread_create(&thread1, NULL, worker_thread_routine, &data1) != 0) {
        perror("Error creating thread 1");
        return 1;
    }
    printf("Main: Created Thread 1 (Concurrent Worker A).\n");

    if (pthread_create(&thread2, NULL, blocking_simulation_thread_routine, &data2) != 0) {
        perror("Error creating thread 2");
        return 1;
    }
    printf("Main: Created Thread 2 (Blocking Simulator).\n");

    if (pthread_create(&thread3, NULL, worker_thread_routine, &data3) != 0) {
        perror("Error creating thread 3");
        return 1;
    }
    printf("Main: Created Thread 3 (Concurrent Worker B).\n");

    printf("Main: All threads created. Main thread might do some work here or just wait.\n");
    for(int i=0; i< WORKLOAD; ++i) {
        printf("Main: Main thread is still running...\n");
        sleep(1);
    }

    // Wait for the threads to finish
    printf("Main: Waiting for threads to complete...\n");
    pthread_join(thread1, NULL);
    printf("Main: Thread 1 joined.\n");
    pthread_join(thread2, NULL);
    printf("Main: Thread 2 joined.\n");
    pthread_join(thread3, NULL);
    printf("Main: Thread 3 joined.\n");

    printf("Main: All threads have completed. Exiting.\n");
    return 0;
}
