#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NPROC 15
#define GROUPS 4

// professor's original test from the pdf
void professor_test(void) {
    printf("\n=== Professor's Original Test (50%% Favoritism) ===\n");
    
    int lock_id = israeli_create(50);
    lcg_srand(getpid());
    
    for (int i = 0; i < NPROC; i++) {
        int pid = fork();
        if (pid == 0) {
            int gid = lcg_rand() % GROUPS;
            setgid(gid);
            israeli_acquire(lock_id);
            printf("Process %d (gid=%d) acquired the lock\n", getpid(), getgid());
            sleep(10);
            israeli_release(lock_id);
            exit(0);
        }
    }
    
    for (int i = 0; i < NPROC; i++)
        wait(0);
        
    israeli_destroy(lock_id);
    printf("=== End of Professor's Test ===\n\n");
}

// additional tests for edge cases
void run_additional_test(int favoritism) {
    printf("--- Running Additional Test (Favoritism: %d%%) ---\n", favoritism);
    
    int lock_id = israeli_create(favoritism);
    lcg_srand(uptime()); // use uptime to get a different seed

    for (int i = 0; i < NPROC; i++) {
        int pid = fork();
        if (pid == 0) {
            int gid = lcg_rand() % GROUPS;
            setgid(gid);
            israeli_acquire(lock_id);
            printf("Process %d (Group %d) acquired the lock\n", getpid(), getgid());
            sleep(10);
            israeli_release(lock_id);
            exit(0);
        }
    }

    for (int i = 0; i < NPROC; i++) {
        wait(0);
    }
    
    israeli_destroy(lock_id);
    printf("--- Test %d%% Complete ---\n\n", favoritism);
}

int main(void) {
    // 1. run the required test
    professor_test();
    
    sleep(30); 
    
    // 2. run our extra tests
    printf("=== Starting Additional Edge-Case Tests ===\n");
    
    // pure fifo - expecting no streaks
    run_additional_test(0);
    sleep(30);
    
    // max favoritism - expecting long streaks
    run_additional_test(100);

    exit(0);
}