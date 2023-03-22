#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



int main(int argc, char *argv[]){
	pid_t pid, parent;
	parent = getpid();
	int lvl = 0;
	printf("Main process id: %d, level: %d\n", getpid(), lvl++);
	for(int i = 0; i<atoi(argv[1]); i++){
		pid = fork();
		if(pid>0){
			wait(NULL);
		}
		else if(pid == 0){
			printf("Process id: %d, parent id: %d, level: %d\n", getpid(), parent, lvl++);
			parent = getpid();
		}

	}
	return 0;
}
