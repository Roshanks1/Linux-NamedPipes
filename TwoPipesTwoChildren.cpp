// TwoPipesTwoChildren.cpp
// author: Roshan Katta
// date: 4/20/2025
// purpose: Execute ls -ltr | grep 3377 | wc -l using Two Children & Parent

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
	int pipe1[2]; //ls & grep Pipe
	int pipe2[2]; //grep & wc Pipe

	pipe(pipe1); //Create Pipes
	pipe(pipe2);

	pid_t child1 = fork(); //ls -ltr
	if (child1 == 0) {
		dup2(pipe1[1], STDOUT_FILENO); //First Process in Pipe, so write to pipe 1
		close(pipe1[0]);
		close(pipe1[1]);
		close(pipe2[0]);
		close(pipe2[1]);
		
		// Create array for ls -ltr command & Execute
		char *args[] = {(char *)"ls", (char *)"-ltr", NULL};
		execvp(args[0], args);

		//If Fail
		perror("execution of ls fail");
		exit(1);
	}
	
	pid_t child2 = fork(); //grep 3377
	if (child2 == 0) {
		dup2(pipe1[0], STDIN_FILENO); //Read from pipe1 for grep input
		dup2(pipe2[1], STDOUT_FILENO); //Write grep output to pipe2
		close(pipe1[1]);
		close(pipe1[0]);
		close(pipe2[0]);
		close(pipe2[1]);
		
		// Create array for grep command & execute
		char *args[] = {(char *)"grep", (char *)"3377", NULL};
		execvp(args[0], args);

		//If Fail
		perror("execution of grep fail");
		exit(1);
	}
	
	//Parent Process
	dup2(pipe2[0], STDIN_FILENO); //Read from pipe 2 and use for wc input
	close(pipe1[0]);
	close(pipe1[1]);
	close(pipe2[0]);
	close(pipe2[1]);
	
	// Create array for wc -l command & execute
	char *args [] = {(char *)"wc", (char *)"-l", NULL};
	execvp(args[0], args);

	//If Fail
	perror("execution of wc fail");
	exit(1);
}
