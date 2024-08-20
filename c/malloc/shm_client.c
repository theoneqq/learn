#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "sem.h"

#define PATH_NAME "/Users/liuyang/Documents/learn/c"
int main() {
	int proj_id = 1024;
	key_t shm_key;
	if ((shm_key = ftok(PATH_NAME, proj_id)) == -1) {
		on_error("ftok failed");
	}

	//IPC_EXCL文件存在则调用失败
	//int shm_id = shmget(shm_key, sizeof(int), IPC_CREAT|IPC_EXCL|0600);
	int shm_id = shmget(shm_key, sizeof(int), IPC_CREAT|0600);

	printf("shmkey: %d shmid: %d\n", shm_key, shm_id);

	if (shm_id < 0) {
		on_error("shmget failed");
	}

	int *shmp = (int *)shmat(shm_id, NULL, 0);
	if ((void *)shmp == (void*) -1) {
		on_error("shmat failed");
	}

	int sem_id = sem_init();
	if (sem_id < 0) {
		on_error("sem init failed");
	}
	
	while(1) {
		sem_wait(sem_id);
		*shmp = 999;
		if (*shmp != 999) {
			printf("shmp now value: %d\n", *shmp);
			break;
		}
		sem_post(sem_id);
	}

	//解除映射，不删除共享内存
	if (shmdt(shmp) < 0) {
		on_error("shmdt failed");
	}

	/*if (shmctl(shm_id, IPC_RMID, NULL) < 0) {
		on_error("shmctl failed");
	}*/

	if (sem_release(sem_id) < 0) {
		on_error("sem release failed");
	}

	return 0;
}
