#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NUM_TEAMS 3
#define RUNNERS_PER_TEAM 5
#define TARGET_SCORE 30

void run_race(int favoritism) {
    printf("\n==========================================\n");
    printf("Starting Relay Race (Favoritism: %d%%)\n", favoritism);
    printf("==========================================\n\n");
    
    int lock_id = israeli_create(favoritism);
    israeli_reset_scores(); // reset scores for a fresh start

    for (int t = 0; t < NUM_TEAMS; t++) {
        for (int r = 0; r < RUNNERS_PER_TEAM; r++) {
            int pid = fork();
            if (pid == 0) {
                // runner logic
                setgid(t);
                int my_team = t;
                
                while (1) {
                    israeli_acquire(lock_id);
                    
                    // check if someone already won while we waited
                    if (israeli_get_max_score() >= TARGET_SCORE) {
                        israeli_release(lock_id);
                        break;
                    }
                    
                    // update and print
                    int score = israeli_inc_score(my_team);
                    printf("Runner %d (Team %d) acquired the baton\n", getpid(), my_team);
                    printf(" Team %d score = %d\n", my_team, score);
                    
                    if (score >= TARGET_SCORE) {
                        printf("\n>>> TEAM %d WINS THE RACE! <<<\n", my_team);
                    }
                    
                    israeli_release(lock_id);
                    
                    if (score >= TARGET_SCORE) {
                        break; // stop running if we won
                    }
                    
                    sleep(2); // sleep briefly as requested
                }
                exit(0);
            }
        }
    }
    
    // wait for all child runners to finish
    int total_runners = NUM_TEAMS * RUNNERS_PER_TEAM;
    for (int i = 0; i < total_runners; i++) {
        wait(0);
    }
    
    israeli_destroy(lock_id);
}

int main(void) {
    // test with different configurations
    run_race(0);   // pure fifo
    sleep(30);     // let the system rest
    run_race(50);  // medium favoritism
    sleep(30);
    run_race(100); // max favoritism
    
    exit(0);
}