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

void on_error(const char *msg) {
	perror(msg);
	exit(1);
}

#define PATH_NAME "/Users/liuyang/Documents/learn/c/malloc"
int main() {
	int proj_id = 1024;
	key_t shm_key;
	if ((shm_key = ftok(PATH_NAME, proj_id)) == -1) {
		on_error("ftok failed");
	}
	//把之前的老数据清理掉
	int shm_id = shmget(shm_key, sizeof(int), IPC_CREAT|0600);
	if (shmctl(shm_id, IPC_RMID, NULL) < 0) {
		on_error("shmctl failed");
	}

	//IPC_EXCL文件存在则调用失败
	shm_id = shmget(shm_key, sizeof(int), IPC_CREAT|IPC_EXCL|0600);
	//int shm_id = shmget(shm_key, sizeof(int), IPC_CREAT|0600);

	printf("shmkey: %d shmid: %d\n", shm_key, shm_id);

	if (shm_id < 0) {
		on_error("shmget failed");
	}

	int *shmp = (int *)shmat(shm_id, NULL, 0);
	if ((void *)shmp == (void*) -1) {
		on_error("shmat failed");
	}
	
	while(1) {
		*shmp = 777;
		if (*shmp != 777) {
			printf("shmp now value: %d\n", *shmp);
			break;
		}
	}

	//解除映射，不删除共享内存
	if (shmdt(shmp) < 0) {
		on_error("shmdt failed");
	}

	if (shmctl(shm_id, IPC_RMID, NULL) < 0) {
		on_error("shmctl failed");
	}

	return 0;
}
