#include "types.h"
#include "riscv.h"    
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
//added for task 0
static uint lcg_state = 1;
static struct spinlock lcg_lock;

void lcg_init(void) {
    initlock(&lcg_lock, "lcg");
}

void lcg_srand(uint seed) {
    acquire(&lcg_lock);
    lcg_state = seed;
    release(&lcg_lock);
}

uint lcg_rand(void) {
    acquire(&lcg_lock);
    lcg_state = 1664525 * lcg_state + 1013904223;
    uint val = lcg_state;
    release(&lcg_lock);
    return val;
}