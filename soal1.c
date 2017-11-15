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

	if(strcmp(rpath,"/")!=0)sprintf(rfpath, "%s%s",dirpath,rpath);
	else{
		rpath=dirpath;
		sprintf(rfpath,"%s",rpath);
	}

	int rres=0,rfd=0;
	(void) rffi;

	rfd=open(rpath,O_RDONLY);
	if(!(rfd+1)) return -errno;

	rres=pread(rfd,rbuff,rsize,roffset);
	if(!(rres+1)) rres=-errno;

	close(rfd);
	return rres;
}

static int lala_readdir(const char *rdpath, void *rdbuff, fuse_fill_dir_t rdfiller, off_t rdoffset, struct fuse_file_info *rdffi){
	char rdfpath[1000];
	
	if(strcmp(rdpath,"/")!=0)sprintf(rdfpath, "%s%s",dirpath,rdpath);
	else{
		rdpath=dirpath;
		sprintf(rdfpath,"%s",rdpath);
	}
	
	int rdres = 0;
	DIR *rddp;
	struct dirent *rdde;

	(void) rdoffset;
	(void) rdffi;

	rddp = opendir(rdpath);
	if (!rddp) return -errno;

	while ((rdde = readdir(rddp))) {
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

static struct fuse_operations st_fuse_op = {
	.getattr	= lala_getattr,
	.readdir	= lala_readdir,
	.read 		= lala_read,
};

int main(int argc, char *argv[]){
	umask(0);
	return fuse_main(argc, argv, &st_fuse_op, NULL);
}