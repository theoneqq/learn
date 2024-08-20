#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

#define SEM_PROJ_ID 2048
#define SEM_PATH_NAME "/Users/liuyang/Documents/learn/c/malloc"

int sem_init(void);
int sem_wait(int sem_id);
int sem_post(int sem_id);
int sem_release(int sem_id);
void on_error(const char *msg);
