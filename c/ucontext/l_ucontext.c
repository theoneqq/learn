#include <ucontext.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

void other_func(uint32_t param1, uint32_t param2) {
	int sum = param1 + param2;
	printf("sum: %d\n", sum);
}

void test_setcontext() {
	int i  = 1;
	ucontext_t context;
	getcontext(&context);
	for (;;) {
		if (i++ % 10 == 0) {
			printf("now i: %d\n", i);
			break;
		}
	}
	sleep(1);
	setcontext(&context);
}

void test_makecontext() {
	int i = 0;
	ucontext_t main_context;
	getcontext(&main_context);
	printf("test_makecontext begin: %d!\n", i++);
	
	ucontext_t context;
	getcontext(&context);
	char stack[1024];
	context.uc_stack.ss_sp = stack;
	context.uc_stack.ss_size = sizeof(stack);
	context.uc_link = &main_context;
	makecontext(&context, (void (*) (void)) other_func, 11, 8);
	setcontext(&context);
}

char stack[1024];
ucontext_t ctxs[2];
void print_hello(uint32_t old_idx, uint32_t new_idx) {
	printf("hello before\n");
	swapcontext(&ctxs[1], &ctxs[0]);
	printf("hello after\n");
}

void init_context(int idx) {
	ucontext_t* context = &(ctxs[idx]);
	getcontext(context);
	context->uc_stack.ss_sp = stack;
	context->uc_stack.ss_size = sizeof(stack);
	context->uc_link = NULL;
}
void test_swapcontext() {
	init_context(0);
	init_context(1);
	uint32_t old_idx = 0;
	uint32_t new_idx = 1;
	printf("swap begin\n");
	makecontext(&ctxs[1], (void (*) (void)) print_hello, old_idx, new_idx);
	swapcontext(&ctxs[0], &ctxs[1]);
	printf("swap end\n");
}

int main() {
	test_swapcontext();
	return 0;
}
