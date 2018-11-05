/** @file
 *
 * minimal example filesystem using high-level API
 *
 * Compile with:
 *
 *     gcc -Wall ASE-fuse.c -o ASE-fuse -D_FILE_OFFSET_BITS=64 -lfuse
 *
 */

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include <dirent.h>

struct file {
	char filename[MAXNAMLEN];
	char contents[4096];
	int size;
} file;

struct directory {
	struct file files[10];
	int entry_count;
} dir;

static void *simple_init()
{
	/* create/fill directory */
	dir.entry_count=2;

	/* file 1 */
	strcpy(dir.files[0].filename, "test1");
	dir.files[0].size=10;
	memcpy(dir.files[0].contents, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 10);

	/* file 2 */
	strcpy(dir.files[1].filename, "test2");
	dir.files[1].size=5;
	memcpy(dir.files[1].contents, "01234", 5);

	return NULL;
}

static int simple_getattr(const char *path, struct stat *stbuf)
{
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else {
		unsigned int i;
		for (i = 0 ; i < dir.entry_count ; i++) {
			if (strcmp(path+1, dir.files[i].filename) == 0) {
				stbuf->st_mode = S_IFREG | 0444;
				stbuf->st_nlink = 1;
				stbuf->st_size = dir.files[i].size;
				break ;
			}
		}
		if (i == dir.entry_count) {
			res = -ENOENT;
		}
	}

	return res;
}

static int simple_getdir(const char *path, fuse_dirh_t h, fuse_dirfil_t filler)
{
	if (strcmp(path, "/") != 0)
		return -ENOENT;

	filler(h, ".", 0);
	filler(h, "..", 0);
	unsigned int i;
	for (i = 0 ; i < dir.entry_count ; i++) {	
		filler(h, dir.files[i].filename, 0);
	}

	return 0;
}

static int simple_open(const char *path, int flags)
{
	unsigned int i;
	for (i = 0 ; i < dir.entry_count ; i++) {
		if (strcmp(path+1, dir.files[i].filename) == 0) {
			return (flags & O_ACCMODE) != O_RDONLY ? -EACCES : 0;
		}
	}
	return -ENOENT;
}

static int simple_read(const char *path, char *buf, size_t size, off_t offset)
{
	unsigned int i;
	for (i = 0 ; i < dir.entry_count ; i++) {
		if (strcmp(path+1, dir.files[i].filename) == 0) {
			size_t len = dir.files[i].size;
			if (offset < len) {
				if (offset + size > len)
					size = len - offset;
				memcpy(buf, dir.files[i].contents + offset, size);
				return size;
			}
			return 0;
		}
	}
	return -ENOENT;
}

static struct fuse_operations simple_oper = {
	.getattr	= simple_getattr,
	.getdir		= simple_getdir,
	.open		= simple_open,
	.read		= simple_read,
//	.mkdir		=
//	.unlink		=
// 	.rmdir		=
//	.rename		=
//	.chmod		=
};

int main(int argc, char *argv[])
{
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

	simple_init();
	return fuse_main(args.argc, args.argv, &simple_oper);
}
