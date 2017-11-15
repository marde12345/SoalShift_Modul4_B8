#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

int main(int argc, char *argv[]){
	umask(0);
	return fuse_main(argc,argv,&lala_oper,NULL);
}