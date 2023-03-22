#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>


struct timeval begin, end;

int main(int argc, char *argv[]) {
    
    	char* arg_list[] = {argv[2], argv[3], NULL};
    	int n = atoi(argv[1]);
	pid_t pid[n], pid2;
    	int pipes[n][2];
    	
	for (int i = 0; i < n; i++) {
        	if (pipe(pipes[i]) == -1) {
            	perror("pipe");
            	exit(1);
        	}
    	}	

    	for (int i = 0; i < n; i++) {
        	pid[i] = fork();
        	if (pid[i] == -1) {
            		perror("fork");
            		exit(1);
        	} else if (pid[i] == 0) {
            		close(pipes[i][0]);
            		gettimeofday(&begin, NULL);
            		pid2 = fork();
            		if(pid2 == 0){
                		execvp(argv[2] > "/dev/null", arg_list);
                		exit(0);
            		}
            		gettimeofday(&end, NULL);
            		double runtime = (end.tv_sec - begin.tv_sec) * 100.0 + 
		    			 (end.tv_usec - begin.tv_usec) / 1000.0;
            		write(pipes[i][1], &runtime, sizeof(runtime));
            		close(pipes[i][1]);
            		exit(0);
        	}
    	}
    
	double max = 0, min = 5.0, total = 0.0;

    	for (int i = 0; i < n; i++) {
		waitpid(pid[i],NULL,NULL);
        	close(pipes[i][1]);
        	double runtime;
        	read(pipes[i][0], &runtime, sizeof(runtime));
        	printf("Child %d Executed in %.2f millis\n", i + 1, runtime);

        	if (runtime > max) max = runtime;
        	else if (runtime < min) min = runtime;
        	total += runtime;

        	close(pipes[i][0]);
    	}

    	printf("Max: %.2f millis\n", max);
    	printf("Min: %.2f millis\n", min);
    	printf("Average: %.2f millis\n", total / n);

    	return 0;
}
