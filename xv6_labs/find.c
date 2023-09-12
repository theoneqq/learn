#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include <user/user.h>

static const int DIRSIZE = 14;
void scan(char *path) {
	char buf[512];
	if (strlen(path) + 1 + DIRSIZE + 1 > sizeof(buf)) {
		printf("too long path: %s\n", path);
		return;
	}
	int fd;
	if ((fd = open(path, 0)) < 0) {
		printf("can not open path: %s\n", path);
		return;
	}

	struct dirent d;
	struct stat st;

	strcpy(buf, path);
	char *p = buf + strlen(buf);
	*p++ = '/';
	//char *sub_path;
	while(read(fd, &d, sizeof(d)) == sizeof(d)) {
		if (d.inum == 0) continue;
		if (strcmp(".", d.name) != 0 && strcmp("..", d.name)) {
			memmove(p, d.name, DIRSIZE);
			p[DIRSIZE] = 0;			
			if (stat(buf, &st) < 0) {
				printf("can not stat: %s\n", buf);
				continue;
			}
			switch(st.type) {
				case T_DEVICE:
				case T_FILE:
					printf("%s\n", buf);
					break;
				case T_DIR:
					//sub_path = (char*) malloc((strlen(buf) + 1) * sizeof(char));
					//strcpy(sub_path, buf);
					//scan(sub_path);
					scan(buf);
					break;
			}
		}
	}
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("bad params!\n");
	} else {
		scan(argv[1]);
	}
	exit(0);
}
