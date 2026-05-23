// System call numbers
#define SYS_fork    1
#define SYS_exit    2
#define SYS_wait    3
#define SYS_pipe    4
#define SYS_read    5
#define SYS_kill    6
#define SYS_exec    7
#define SYS_fstat   8
#define SYS_chdir   9
#define SYS_dup    10
#define SYS_getpid 11
#define SYS_sbrk   12
#define SYS_sleep  13
#define SYS_uptime 14
#define SYS_open   15
#define SYS_write  16
#define SYS_mknod  17
#define SYS_unlink 18
#define SYS_link   19
#define SYS_mkdir  20
#define SYS_close  21
#define SYS_lcg_srand  22               //added for task 0
#define SYS_lcg_rand   23               //added for task 0
#define SYS_setgid 24                   // TASK 1 - set group ID for Israeli lock
#define SYS_getgid 25                   // TASK 1 - get group ID for Israeli lock
#define SYS_israeli_create 26           // TASK 1 - create Israeli lock, return lock ID on success, -1 on failure
#define SYS_israeli_acquire 27          // TASK 1 - acquire Israeli lock by lock ID
#define SYS_israeli_release 28          // TASK 1 - release Israeli lock by lock ID
#define SYS_israeli_destroy 29          // TASK 1 - destroy Israeli lock by lock ID
#define SYS_israeli_inc_score 30        // TASK 2 - increment score for team, return new score
#define SYS_israeli_get_max_score 31    // TASK 2 - get max score among all teams
#define SYS_israeli_reset_scores 32     // TASK 2 - reset all team scores to 0