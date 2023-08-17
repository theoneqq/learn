#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main() {
	struct addrinfo hints, *server_info, *p;
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo("127.0.0.1", 12345, &hints, &server_info) != 0) {
		printf("get addr info error!\n");
		return 0;
	}
	
	int s = -1;
	int backlog = 16;
	for (p = server_info; p != NULL; p = p -> ai_next) {
		if ((s = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			continue;
		if (bind(s, p->ai_addr, p->ai_addrlen, backlog) == -1) {
			printf("bind error!\n")
			return 0;
		} else {
			if (listen(s, backlog) == -1) {
				printf("listen error!\n");
				return 0;
			} else {
				while(true) {
					int client_socket_id;
					struct sockaddr_storage sa;
					socklen_t salen = sizeof(sa);
					do {
						client_socket_id = accept(s, (struct sockaddr*)&sa, &salen);
					} while(client_socket_id == -1 && errno == EINTER);

					if (client_socket_id != -1) {
						char ip[46];
						size_t ip_len = 46;
						int *port;
						struct sockaddr_in *s = (struct sockaddr_in *)&sa;
						inet_ntop(AF_INET, (void *)&(s->sin_addr), ip, ip_len);
						*port = ntohs(s->sin_port);
						printf("client ip: %s port: %d\n", ip, *port);
						close(client_socket_id);
					}
				}
			}
		}
	}

	return 0;
}
