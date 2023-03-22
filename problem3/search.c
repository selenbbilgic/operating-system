#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

typedef struct {
	int x;
	int starting; 
	int ending;
	int *array;
} Child;

int search(Child *child){
	for(int i = child->starting; i< child->ending; i++){
		if(child->array[i] == child->x){
			printf("The number %d is at index %d\n", child->x, i);
			exit(0);
		}
	}
	exit(1);
}



int main(int argc, char *argv[]){
	int array[1000];
	int j, status;
	pid_t pid[1000];

	if(argc != 3){
		printf("entered wrong number of inputs.");
		return 1;
	}
	int x = atoi(argv[1]);
	int n = atoi(argv[2]);
	int length;

	for(length = 0; length<1000; length++){
		if(scanf("%d", &array[length]) == EOF){
			break;
		}
	}


	int size = length / n;
	int rem = length % n;

	for(int i = 0; i<n; i++){
		Child child;
		child.array = array;
		child.x = x;
		child.starting = size * i;
		child.ending = child.starting + size;
		if(i == n-1){
			child.ending += rem;
		}

		pid[i] = fork();

		if(pid[i] == 0){
			search(&child);
		}
	}
		
		int found = 0;
		while(!found){
			for(int i = 0; i<n; i++){
				if(kill(pid[i],0) != 0) continue;
				if(pid[i] == 0) continue;
				if(waitpid(pid[i], &status) == pid[i]){
					if(status == 0){
						found = 1;
						for(j = 0; j<n; j++){
							if(j!=i){
								kill(pid[j],0);
							}
						}
						break;
					}
					else{
						pid[i] =0;
					}
				}
			}
			usleep(10);
		}
	return 0;
}
