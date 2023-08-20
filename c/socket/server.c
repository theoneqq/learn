#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int listen_on(char *port) {
	struct addrinfo hints, *server_infos, *p;
	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV;

	int getaddrinfo_err;
	if ((getaddrinfo_err = getaddrinfo(NULL, port, &hints, &server_infos)) != 0) {
		printf("get addr info error: %s\n", gai_strerror(getaddrinfo_err));
		return -1;
	}
	
	int s = -1;
	int backlog = 16;
	for (p = server_infos; p != NULL; p = p -> ai_next) {
		if ((s = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
			continue;
		if (bind(s, p->ai_addr, p->ai_addrlen) == 0) {
			break;
		} else {
			close(s);
		}
	}

	//avoid memory leak
	freeaddrinfo(server_infos);
	if (!p) return -1;

	if (listen(s, backlog) < 0) {
		close(s);
		return -1;
	}

	return s;
}

void init_server(char *port) {
	int listen_fd, conn_fd;
	socklen_t client_len;

	int MAX_LINE = 8192;
	struct sockaddr_storage client_addr;
	char client_hostname[MAX_LINE], client_port[MAX_LINE];

	listen_fd = listen_on(port);
	if (listen_fd < 0) {
		printf("listen on %s failed!\n", port);
		exit(0);
	}

	while (1) {
		client_len = sizeof(struct sockaddr_storage);
		conn_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len);

		//NI_NUMERICHOST display host by ip instead of domain name
		int getnameinfo_err = getnameinfo((struct sockaddr *) &client_addr, client_len, client_hostname, MAX_LINE, client_port, MAX_LINE, NI_NUMERICHOST);
		if (getnameinfo_err != 0) {
			printf("get name info error: %s at fd:%d\n", gai_strerror(getnameinfo_err), conn_fd);
			close(conn_fd);
		} else {
			printf("connect to (%s, %s) start to read on fd:%d\n", client_hostname, client_port, conn_fd);
			char client_msg[100];
			while (read(conn_fd, client_msg, 100) > 0) {
				printf("receive client msg: %s\n", client_msg);
			}
		}
	}
	exit(0);
}

//what can do when client close connection initative
int main() {
	init_server("12345");
	return 0;
}
