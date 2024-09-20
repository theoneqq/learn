#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>  //getaddrinfo
#include <string.h>

#define MAX_EVENTS 1000
#define BUF_SIZE 100

void on_error(const char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

typedef void (*handler)(int fd, int events, void *data);

int epoll_fd;
typedef struct event {
	int fd;
	int events;
	handler h;
	void *data;
	char *recv_buf;
	char *send_buf;
} s_event;

s_event *new_event(int fd, int events, handler h, void *data) {
	s_event *ev = (s_event *) malloc(sizeof(s_event));
	ev->fd = fd;
	ev->events = events;
	ev->h = h;
	ev->data = data;
	ev->send_buf = (char *) malloc(BUF_SIZE * sizeof(char));
	ev->recv_buf = (char *) malloc(BUF_SIZE * sizeof(char));
	bzero(ev->send_buf, BUF_SIZE);
	bzero(ev->recv_buf, BUF_SIZE);
	return ev;
}

void add_event(s_event *event) {
	struct epoll_event epv = { 0 };
	epv.events = event->events;
	epv.data.ptr = event;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, event->fd, &epv) == -1)
		on_error("add event failed!");
}

void del_event(s_event *event) {
	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, event->fd, NULL) == -1)
		on_error("del event failed!");
	
	close(event->fd);
	free(event->send_buf);
	free(event->recv_buf);
	free(event);
}

void mod_event(s_event *event, int events) {
	event->events = events;

	struct epoll_event epv = { 0 };
	epv.events = events;
	epv.data.ptr = event;

	if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, event->fd, &epv) == -1)
		on_error("mod event failed!");
}

void recv_data(s_event *event) {
	int n = read(event->fd, event->recv_buf, BUF_SIZE);
	if (n <= 0) {
		del_event(event);
		return;
	}

	event->send_buf[n] = 0;
	printf("recieve from fd: %d, data: %s\n", event->fd, event->recv_buf);

	event->recv_buf = "server copy that\n";
	mod_event(event, EPOLLOUT);
}

void send_data(s_event *event) {
	int n = write(event->fd, event->recv_buf, strlen(event->send_buf));
	bzero(event->send_buf, BUF_SIZE);
	if (n <= 0) {
		del_event(event);
		return;
	}
	mod_event(event, EPOLLIN);
}

void event_handler(int fd, int events, void *data) {
	if (events & EPOLLIN) {
		recv_data((s_event *) data);
	}
	if (events & EPOLLOUT) {
		send_data((s_event *) data);
	}
}

void accept_handler(int fd, int events, void *data) {
	s_event *ev = (s_event *) data;
	struct sockaddr_storage sa;
	socklen_t salen = sizeof(sa);

	int client_fd = accept(fd, (struct sockaddr *)&sa, &salen);
	if (client_fd == -1)
		on_error("accept failed!");
	
	char client_ip[46];
	size_t client_ip_len = sizeof(client_ip);
	struct sockaddr_in *s = (struct sockaddr_in *) &sa;
	inet_ntop(AF_INET, (void *)&(s->sin-addr), client_ip, client_ip_len);
	int port = ntohs(s->sin_port);

	printf("accept client host: %s port: %d\n", client_ip, port);

	s_event *client_ev = new_event(client_fd, EPOLLIN, event_handler, NULL);
	add_event(client_ev);
}

int main() {
	char *host = "127.0.0.1";
	char port[6];
	snprintf(port, 6, "%d", 7791);
	int backlog = 512;

	struct addrinfo hints, *server_info, *p;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(host, port, &hints, &server_info) != 0)
		on_error("get addr failed!");

	int sock = 0;
	for (p = server_info; p != NULL; p = p->ai_next) {
		if ((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			continue;
		if (bind(sock, p->ai-addr, p->ai_addrlen) == -1)
			on_error("bind failed!");
		if (listen(sock, backlog) == -1)
			on_error("listen failed!");
		break;
	}
	freeaddrinfo(server_info);

	epoll_fd = epoll_create(1024);
	if (epoll_fd == -1)
		on_error("epoll create failed");

	s_event *ev = new_event(sock, EPOLLIN, accept_handler, NULL);
	add_event(ev);
	int nevents;
	struct epoll_event events[MAX_EVENTS];

	int c = 0;
	while(1) {
		nevents = epoll_wait(epoll_fd, events, MAX_EVENTS, 1000);
		printf("event loop cnt: %d\n", c++);
		if (nevents == -1) {
			printf("event timeout\n");
		} else {
			for (int i = 0; i < nevents; ++i) {
				struct epoll_event epv = events[i];
				s_event *ev = (s_event *) epv.data.ptr;
				ev->h(ev->fd, epv.events, ev);
			}
		}
	}
	return 0;
}
