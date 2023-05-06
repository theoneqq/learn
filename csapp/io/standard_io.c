#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


void file_format_io() {
	char s[40];
	char ss[40];
	FILE *f = fopen("s.txt", "r+");

	//output after input
	fscanf(f, "%1s", s);
	fflush(f);
	fprintf(f, "%s\n", "ok"); // write also can change file pos

	//input after output
	fseek(f, 2, SEEK_SET); //key point
	fscanf(f, "%2s", ss);
	printf("read data: %s", ss);

	fclose(f);
}

void keyboard_io() {
	char line[1];
	scanf("%1s", line);
	
	int fd = open("s.txt", O_RDWR, 0);
	dup2(fd, 1); // standard output redirection to fd
	printf("hello,world!");
	close(fd);
	return;
}

void p_error(const char *msg) {
	printf("%s failed!\n", msg);
}

int file_io() {
	FILE *in, *out;
	#pragma pack(4) //align = 4byte
	struct pirate {
		char name[8];
		unsigned long booty;
		unsigned int len;
	} p, black = { "jack ma", 99, 100 };
	#pragma pack() //align = defalut

	out = fopen("data.txt", "w");
	if (!out) {
		p_error("open");
		return 1;
	}
	if (fwrite(&black, sizeof(struct pirate), 1, out) == EOF) {
		p_error("write");
		return 1;
	}

	if (fclose(out) == EOF) {
		p_error("close");
		return 1;
	}

	in = fopen("data.txt", "r");
	if (!in) {
		p_error("open");
		return 1;
	}
	if (fread(&p, sizeof(struct pirate), 1, in) == EOF) {
		p_error("read");
		return 1;
	}
	if (fclose(in) == EOF) {
		p_error("close");
		return 1;
	}

	printf("name = %s booty = %lu len = %u\n", p.name, p.booty, p.len);
	return 0;
}

int main() {
	//file_format_io();
	//keyboard_io();
	file_io();
	return 0;
}
