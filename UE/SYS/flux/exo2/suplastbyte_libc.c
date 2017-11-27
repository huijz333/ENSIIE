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
	FILE * f = fopen(filepath, "r");
	if (f == NULL) {
		fprintf(stderr, "%s:%s (errno %d)\n", filepath, strerror(errno), errno);
		return (1);
	}
	
	//get file size
	//seek to the end of file
	if (fseek(f, 0L, SEEK_END) == -1) {
		fprintf(stderr, "%s:%s (errno %d)\n", filepath, strerror(errno), errno);
		fclose(f);
		return (1);
	}
	//current cursor position (== filesize)
	unsigned int fsize = ftell(f);

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

	//reset cursor, to read
	fseek(f, 0L, SEEK_SET);
	//read fsize - 1 first bytes
	fread(buffer, fsize - 1, 1, f);
	
	//close it, no longer need it
	fclose(f);

	//open file, and truncate, write
	f = fopen(filepath, "w");
	if (f == NULL) {
		fprintf(stderr, "%s:%s (errno %d)\n", filepath, strerror(errno), errno);
		return (1);
	}
	//write buffer to newly truncated files
	fwrite(buffer, fsize - 1, 1, f);
	fclose(f);
	
	//no longer need the allocated buffer
	free(buffer);
	return (0);
}
