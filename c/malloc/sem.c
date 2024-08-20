#include "sem.h"

void on_error(const char *msg) {
	perror(msg);
	exit(1);
}

int sem_init(void) {
	key_t key;
	int sem_id;
	union semun sem_union;

	if ((key = ftok(SEM_PATH_NAME, SEM_PROJ_ID)) == -1) {
		on_error("sem ftok failed");
	}

	if ((sem_id = semget(key, 1, IPC_CREAT|0644)) < 0) {
		on_error("semget failed");
	}

	sem_union.val = 0;
	if (semctl(sem_id, 0, SETVAL, sem_union) < 0) {
		on_error("semctl failed");
	}

	return sem_id;
}

int sem_release(int sem_id) {
	union semun sem_union;
	if (semctl(sem_id, 0, IPC_RMID, sem_union) < 0) {
		on_error("semctl remove failed");
	}

	return 0;
}

int sem_wait(int sem_id) {
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = -1;
	sb.sem_flg = SEM_UNDO;

	if (semop(sem_id, &sb, 1) < 0) {
		printf("sem wait faild");
		return -1;
	}

	return 0;
}

int sem_post(int sem_id) {
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = 1;
	sb.sem_flg = SEM_UNDO;

	if (semop(sem_id, &sb, 1) < 0) {
		printf("sem wait faild");
		return -1;
	}
	
	return 0;
}
