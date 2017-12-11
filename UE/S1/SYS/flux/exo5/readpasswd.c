#include <unistd.h> //write
#include <fcntl.h> //open
#include <stdlib.h> //system
#include <stdio.h> //printf

# define BACKSPACE ((char)127)

int readpasswd(char pwd[16]) {
	char * ttypath = ttyname(0);
	if (ttypath == NULL) {
		ttypath = ttyname(1);
	}
	if (ttypath == NULL) {
		ttypath = ttyname(2);
	}
	if (ttypath == NULL) {
		return (-1);
	}
	int fd = open(ttypath, O_RDWR);
	if (fd == -1) {
		return (-1);
	}
	system("stty raw -echo");
	write(1, "passwd: ", 8);
	int i;
	while (1) {
		char c;
		read(fd, &c, 1);
		if (c == '\n' || c == '\r') {
			break ;
		}
		if (c == BACKSPACE && i > 0) {
			--i;
			write(1, "\b \b", 3);
		} else if (i < 15) {
			write(1, "X", 1);
			pwd[i++] = c;
		}
	}
	pwd[i] = 0;
	write(1, "\n\r", 2);
	close(fd);
	system("stty -raw echo");
	return (i);
}

int main() {
	char buf[16];
	if (readpasswd(buf) != -1) {
		printf("password is: %s\n", buf);
		return (0);
	}
	return (1);
}
