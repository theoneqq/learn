#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUF_SIZE 100
int main() {
	int socks[2];
	//这用起来跟管道太像了，那么这个有什么不同之处呢
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, socks) == -1) {
		printf("socketpair call failed!\n");
		return 0;
	}

	pid_t pid = fork();
	if (pid == 0) {
		char *send_msg = "send to parent";
		struct iovec iov[1];
		struct msghdr msg;
		iov[0].iov_base = send_msg;
		iov[0].iov_len = BUF_SIZE;
		msg.msg_name = NULL;
		msg.msg_namelen = 0;
		msg.msg_iov = iov;
		msg.msg_iovlen = 1;

		//这里其实也可以用标准的系统调用write函数
		if (sendmsg(socks[0], &msg, 0) == -1) {
			printf("child send msg failed\n");
		}
		exit(0);	
	}
	
	char recv_msg[BUF_SIZE];
	struct iovec iov[1];
	struct msghdr msg;
	iov[0].iov_base = recv_msg;
	iov[0].iov_len = BUF_SIZE; //怎么提前知道要读取的大小呢
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;

	sleep(3);

	if (recvmsg(socks[1], &msg, 0) == -1) {
		printf("recv child msg failed");
		return 0;
	}

	printf("recv msg from child: %s\n", (char *) msg.msg_iov[0].iov_base);
	return 0;
}
