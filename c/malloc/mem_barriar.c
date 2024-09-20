#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t begin_sem1;
sem_t begin_sem2;
sem_t end_sem;

int x, y;
int r1, r2;

void *thread_func1(void *param) {
	while(1) {
		sem_wait(&begin_sem1);
		x = 1;
		__asm__ __volatile__("mfence":::"memory");
		r1 = y;
		sem_post(&end_sem);
	}
	return NULL;
}

void *thread_func2(void *param) {
	while(1) {
		sem_wait(&begin_sem2);
		y = 1;
		__asm__ __volatile__("mfence":::"memory");
		r2 = x;
		sem_post(&end_sem);
	}
	return NULL;
}

int main() {
	sem_init(&begin_sem1, 0, 0);
	sem_init(&begin_sem2, 0, 0);
	sem_init(&end_sem, 0, 0);

	pthread_t thread1, thread2;
	pthread_create(&thread1, NULL, thread_func1, NULL);
	pthread_create(&thread2, NULL, thread_func2, NULL);

	int detected = 0;
	int iterations = 0;

	for (iterations = 1;;iterations++) {
		x = 0;
		y = 0;
		sem_post(&begin_sem1);
		sem_post(&begin_sem2);
		sem_wait(&end_sem);
		sem_wait(&end_sem);

		if (r1 == 0 && r2 == 0) {
			detected++;
			printf("%d reorders detected after %d iterations\n", detected, iterations);
		}
		if (detected > 5) {
			break;
		}
	}
	return 0;
}
