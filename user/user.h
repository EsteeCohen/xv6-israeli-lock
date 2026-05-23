struct stat;

// system calls
int fork(void);
int exit(int) __attribute__((noreturn));
int wait(int*);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(const char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
void lcg_srand(uint);               // added for task 0
uint lcg_rand(void);                // added for task 0
void setgid(int);                   // TASK 1 - set group ID for Israeli lock  
int getgid(void);                   // TASK 1 - get group ID for Israeli lock
int israeli_create(int);            // TASK 1 - create Israeli lock for group ID, return 0 on success, -1 on failure
int israeli_acquire(int);           // TASK 1 - acquire Israeli lock by group ID
int israeli_release(int);           // TASK 1 - release Israeli lock by group ID
int israeli_destroy(int);           // TASK 1 - destroy Israeli lock by group ID
int israeli_inc_score(int);         // TASK 2 - increment score for team, return new score
int israeli_get_max_score(void);    // TASK 2 - get max score among all teams
void israeli_reset_scores(void);    // TASK 2 - reset all team scores to 0

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void fprintf(int, const char*, ...);
void printf(const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
int memcmp(const void *, const void *, uint);
void *memcpy(void *, const void *, uint);
