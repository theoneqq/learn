#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>

#include <sys/socket.h>

#define EVENTS_NUM 2
#define TIMEOUT 2

void p_error(const char *msg) {
	printf("%s errno: %s\n", msg, strerror(errno));
}

void do_epoll() {
	struct epoll_event *r_events;
	int nr_events, i;
	//create epoll instance
	int epfd = epoll_create(1);

	//int fd = open("f.txt", O_RDWR, 0);
	//int fd = STDOUT_FILENO;
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	int ret;

	struct epoll_event read_event, write_event;
	read_event.data.fd = fd;
	read_event.events = EPOLLIN;
	write_event.data.fd = 99; // will copy to r_events if event triggered
	write_event.events = EPOLLOUT; //event on fd

	printf("event EPOLLIN: %d\n", EPOLLIN);
	printf("event EPOLLOUT: %d\n", EPOLLOUT);

	//add event
	/*ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &read_event);
	if (ret) {
		p_error("add read event failed");
		return;
	}*/	

	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &write_event);
	if (ret) {
		p_error("add write event failed");
		return;
	}

	/*struct epoll_event new_read_event;
	new_read_event.data.fd = fd;
	new_read_event.events = EPOLLIN | EPOLLOUT;
	//modify event
	ret = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &new_read_event);
	if (ret) {
		p_error("mod read event failed");
		return;
	}

	//delete event
	ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &new_read_event);
	if (ret) {
		p_error("del event failed");
		return;
	}*/


	//wait event
	r_events = (struct epoll_event*) malloc(sizeof(struct epoll_event) * EVENTS_NUM);
	nr_events = epoll_wait(epfd, r_events, EVENTS_NUM, TIMEOUT * 1000);
	if (nr_events < 0 && errno != EINTR) {
		printf("epoll error errno: %d", errno);
	} else {
		for (int i = 0; i < nr_events; ++i) {
			struct epoll_event event = r_events[i];
			printf("event = %d fd = %d\n", event.events, event.data.fd);
			if (event.events & EPOLLOUT) {
				printf("event EPOLLPRI\n");
			}	
			if (event.events & EPOLLHUP) {
				printf("event ERROR\n");
			}
		}
	}
	free(r_events);

	//close epoll instance
	close(epfd);

	return;
}

int main() {
	do_epoll();
	return 0;
}
