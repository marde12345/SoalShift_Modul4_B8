#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static const char *dirpath = "/home/marde/Documents";

static int lala_getattr(const char *gattrpath, struct stat *gastbuff){
	char gafpath[1001];
	sprintf(gafpath,"%s%s",dirpath,gattrpath);
	int gatemp;
	gatemp = lstat(gattrpath, gastbuff);
	if (!(gatemp+1)) return -errno;
	return 0;
}

static int lala_read(const char *rpath, char *rbuff, size_t rsize, off_t roffset, struct fuse_file_info *rffi ){
	char rfpath[1001];
	sprintf(rfpath,"%s%s",dirpath,rpath);

	int rres=0,rfd=0;
	(void) rffi;

	rfd=open(rfpath,O_RDONLY);
	if(!(rfd+1)) return -errno;

	rres=pread(rfd,rbuff,rsize,roffset);
	if(!(rres+1)) rres=-errno;

	close(rfd);
	return rres;
}

static struct fuse_operations st_fuse_op = {
	.getattr	= lala_getattr,
	.readdir	= lala_readdir,
	.read 		= lala_read,
};

int main(int argc, char *argv[]){
	umask(0);
	return fuse_main(argc, argv, &st_fuse_op, NULL);
}