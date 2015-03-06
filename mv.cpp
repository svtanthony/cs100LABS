#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
int main(int argc, char** argv){
	struct stat st;
	if (argc == 3){
		if( access( argv[2], F_OK ) != -1 ) {			
			if(-1 == stat(argv[2], &st))
				perror("stat error");
			if(S_ISREG(st.st_mode) ==  1){
				printf("error");
				exit(1);
			}
		}
		if(-1 == rename(argv[1], argv[2]))
			perror("rename error");
	}
	else
		printf("missing argument");
	return 0;
}

