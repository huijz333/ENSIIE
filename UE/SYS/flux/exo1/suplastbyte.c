#include <unistd.h> //read, write
#include <stdio.h> //printf
#include <fcntl.h> //open
#include <errno.h> //errno
#include <string.h> //strerror
#include <sys/types.h> //fstat
#include <sys/stat.h> //fstat
#include <stdlib.h> //malloc

int main(int argc, char ** argv) {

	//check arguments
	if (argc != 2) {
		fprintf(stderr, "error usage: %s [FIlEPATH]\n", argv[0]);
		return (1);
	}
	char * filepath = argv[1];

	//open file
	int fd = open(filepath, O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "%s:%s (errno %d)\n", filepath, strerror(errno), errno);
		return (1);
	}
	
	//get file size
	struct stat buf;
	if (fstat(fd, &buf) == -1) {
		fprintf(stderr, "%s:%s (errno %d)\n", filepath, strerror(errno), errno);
		close(fd);
		return (1);
	}
	//file size in bytes
	unsigned int fsize = buf.st_size;
	if (fsize == 0) {
		fprintf(stderr, "%s:file of size 0, can't remove last byte.\n", argv[0]);
		return (1);
	}
	char * buffer = (char *)malloc(sizeof(char) * (fsize - 1));
	if (buffer == NULL) {
		//TODO : create another buffer with a smaller size, instead of roughly return error
		fprintf(stderr, "%s:not enough memory\n", argv[0]);
		return (1);
	}
	//read fsize - 1 first bytes
	read(fd, buffer, fsize - 1);
	
	//close it, no longer need it
	close(fd);

	//open file, and truncate, write
	fd = open(filepath, O_TRUNC | O_WRONLY);
	if (fd == -1) {
		fprintf(stderr, "%s:%s (errno %d)\n", filepath, strerror(errno), errno);
		return (1);
	}
	//write buffer to newly truncated files
	write(fd, buffer, fsize - 1);
	close(fd);
	
	//no longer need the allocated buffer
	free(buffer);
	return (0);
}
