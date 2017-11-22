#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static const char *dirpath = "/home/marde/Downloads";

static int lala_getattr(const char *, struct stat *);
int lala_cek_eks(const char *, const char *);
static int lala_read(const char *, char *, size_t , off_t , struct fuse_file_info *);
static int lala_readdir(const char *, void *, fuse_fill_dir_t , off_t , struct fuse_file_info *);
static int lala_chmod(const char *, mode_t );
static int lala_rename(const char *, const char *);
static int lala_link(const char *, const char *);
static int lala_unlink(const char *);
static int lala_mkdir(const char *, mode_t );
char *getnamefile(const char *);
static int lala_write(const char *, const char *, size_t , off_t , struct fuse_file_info *);

static struct fuse_operations st_fuse_op = {
	.getattr	= lala_getattr,
	.readdir	= lala_readdir,
	.read 		= lala_read,
	.rename		= lala_rename,
	.chmod		= lala_chmod,
	.link		= lala_link,
	.unlink		= lala_unlink,
	.mkdir 		= lala_mkdir,
	.write 		= lala_write,
};

int main(int argc, char *argv[]){
	umask(0);
	return fuse_main(argc, argv, &st_fuse_op, NULL);
}

static int lala_getattr(const char *gattrpath, struct stat *gastbuff){
	char gafpath[1001];
	sprintf(gafpath,"%s%s",dirpath,gattrpath);
	int gatemp;
	gatemp = lstat(gafpath, gastbuff);

	if (!(gatemp+1)) return -errno;
	return 0;
}

int lala_cek_eks(const char *cepath, const char *ceeks){
	const char *point = strrchr(cepath,'.');

	if(point!=NULL){
		return strncmp(point+1,ceeks,3)==0;
	}
	return 0;
}

static int lala_read(const char *rpath, char *rbuff, size_t rsize, off_t roffset, struct fuse_file_info *rffi ){
	char rfpath[1001];
	if(strcmp(rpath,"/"))sprintf(rfpath, "%s%s",dirpath,rpath);
	else{
		rpath=dirpath;
		sprintf(rfpath,"%s",rpath);
	}

	int rres=0,rfd=0;
	(void) rffi;

	rfd=open(rfpath,O_RDONLY);
	if(!(rfd+1)) return -errno;

	rres=pread(rfd,rbuff,rsize,roffset);
	if(!(rres+1)) rres=-errno;

	close(rfd);
	return rres;

}

static int lala_readdir(const char *rdpath, void *rdbuff, fuse_fill_dir_t rdfiller, off_t rdoffset, struct fuse_file_info *rdffi){
	char rdfpath[1000];
	
	if(strcmp(rdpath,"/"))sprintf(rdfpath, "%s%s",dirpath,rdpath);
	else{
		rdpath=dirpath;
		sprintf(rdfpath,"%s",rdpath);
	}
	
	int rdres = 0;
	DIR *rddp;
	struct dirent *rdde;

	(void) rdoffset;
	(void) rdffi;

	rddp = opendir(rdfpath);
	if (rddp==NULL) return -errno;

	while ((rdde = readdir(rddp))!=NULL) {
		struct stat rdst;
		memset(&rdst, 0, sizeof(rdst));
		rdst.st_ino = rdde->d_ino;
		rdst.st_mode = rdde->d_type << 12;

		rdres = (rdfiller(rdbuff, rdde->d_name, &rdst, 0));
			if(rdres) break;
	}

	closedir(rddp);
	return 0;
}

static int lala_rename(const char *from, const char *to){
	int res;

	res = rename(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int lala_chmod(const char *path, mode_t mode)
{
	int res;

	res = chmod(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int lala_link(const char *from, const char *to)
{
	int res;

	res = link(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int lala_unlink(const char *path)
{
	int res;

	res = unlink(path);
	if (res == -1)
		return -errno;

	return 0;
}

static int lala_mkdir(const char *path, mode_t mode)
{
	int res;

	res = mkdir(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

char *getnamefile(const char *input){
	char *point = strrchr(input,'/');

	return (point+1);
}

static int lala_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	int fd;
	int res;

	(void) fi;
	fd = open(path, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}