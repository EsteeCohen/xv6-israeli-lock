#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

#define NLOCKS 15
#define MAX_QUEUE 16

struct israeli_lock {
    int active;
    int favoritism;
    int owner_pid;
    int owner_gid;
    int queue[MAX_QUEUE];
    int q_count;
    struct spinlock lk;
};

struct israeli_lock ilocks[NLOCKS];
int team_scores[10];            // TASK 2 - array to hold scores for each team
struct spinlock score_lock;     // TASK 2 - lock to protect access to team_scores

void israeli_init(void) {
    initlock(&score_lock, "score_lock");                // TASK 2 - initialize score lock
    for(int i = 0; i < 10; i++) team_scores[i] = 0;     // TASK 2 - initialize team scores to 0
    for(int i = 0; i < NLOCKS; i++) {
        initlock(&ilocks[i].lk, "israeli_lock");
        ilocks[i].active = 0;
    }
}

int israeli_create(int favoritism) {
    for(int i = 0; i < NLOCKS; i++) {
        acquire(&ilocks[i].lk);
        if(ilocks[i].active == 0) {
            ilocks[i].active = 1;
            ilocks[i].favoritism = favoritism;
            ilocks[i].owner_pid = 0;
            ilocks[i].owner_gid = -1;
            ilocks[i].q_count = 0;
            release(&ilocks[i].lk);
            return i;
        }
        release(&ilocks[i].lk);
    }
    return -1; // No available locks
}

int israeli_acquire(int lock_id) {
    if(lock_id < 0 || lock_id >= NLOCKS) return -1;
    struct israeli_lock *il = &ilocks[lock_id];
    
    acquire(&il->lk);
    if(!il->active) {
        release(&il->lk);
        return -1;
    }
    
    int mypid = myproc()->pid;
    
    // Add to queue
    if(il->q_count < MAX_QUEUE) {
        il->queue[il->q_count++] = mypid;
    } else {
        panic("israeli lock queue full");
    }
    
    // If lock is free and I am the first in queue, take it immediately
    if(il->owner_pid == 0 && il->queue[0] == mypid) {
        il->owner_pid = mypid;
        il->owner_gid = myproc()->gid;
        
        // Dequeue myself
        for(int i = 0; i < il->q_count - 1; i++) {
            il->queue[i] = il->queue[i+1];
        }
        il->q_count--;
        
        release(&il->lk);
        return 0;
    }
    
    // Wait until it's my turn (yield CPU to avoid busy-wait)
    while(il->owner_pid != mypid) {
        release(&il->lk);
        yield();
        acquire(&il->lk);
    }
    
    // Lock was handed to me by release
    il->owner_gid = myproc()->gid;
    
    // Dequeue myself
    for(int i = 0; i < il->q_count; i++) {
        if(il->queue[i] == mypid) {
            for(int j = i; j < il->q_count - 1; j++) {
                il->queue[j] = il->queue[j+1];
            }
            il->q_count--;
            break;
        }
    }
    
    release(&il->lk);
    return 0;
}

int israeli_release(int lock_id) {
    if(lock_id < 0 || lock_id >= NLOCKS) return -1;
    struct israeli_lock *il = &ilocks[lock_id];
    
    acquire(&il->lk);
    if(!il->active || il->owner_pid != myproc()->pid) {
        release(&il->lk);
        return -1;
    }
    
    int next_owner = 0;
    if(il->q_count > 0) {
        int G = il->owner_gid;
        int c = il->favoritism;
        int candidate_idx = -1;
        
        // Find earliest process with same GID
        for(int i = 0; i < il->q_count; i++) {
            if(get_proc_gid(il->queue[i]) == G) {
                candidate_idx = i;
                break;
            }
        }
        
        // Roll PRNG for favoritism
        if(candidate_idx != -1 && (lcg_rand() % 100) < c) {
            next_owner = il->queue[candidate_idx];
        } else {
            next_owner = il->queue[0]; // Strict FIFO
        }
    }
    
    il->owner_pid = next_owner;
    if(next_owner == 0) il->owner_gid = -1;
    
    release(&il->lk);
    return 0;
}

int israeli_destroy(int lock_id) {
    if(lock_id < 0 || lock_id >= NLOCKS) return -1;
    struct israeli_lock *il = &ilocks[lock_id];
    
    acquire(&il->lk);
    if(!il->active) {
        release(&il->lk);
        return -1;
    }
    il->active = 0;
    release(&il->lk);
    return 0;
}

// TASK 2 - scoring functions for Israeli lock game
int israeli_inc_score(int team) {
    if(team < 0 || team >= 10) return -1;
    acquire(&score_lock);
    team_scores[team]++;
    int s = team_scores[team];
    release(&score_lock);
    return s;
}

// TASK 2 - get max score among all teams
int israeli_get_max_score(void) {
    int max = 0;
    acquire(&score_lock);
    for(int i = 0; i < 10; i++) {
        if(team_scores[i] > max) max = team_scores[i];
    }
    release(&score_lock);
    return max;
}

// TASK 2 - reset all team scores to 0
void israeli_reset_scores(void) {
    acquire(&score_lock);
    for(int i = 0; i < 10; i++) team_scores[i] = 0;
    release(&score_lock);
}