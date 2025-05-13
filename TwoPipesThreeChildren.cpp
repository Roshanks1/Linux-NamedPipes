// TwoPipesThreeChildren
// author: Roshan Katta
// date: 4/20/2025
// ls -ltr | grep 3377 | wc -l using three children processes
// **STDIN/STDOUT --> Same as 0/1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
	int pipe1[2];
	int pipe2[2];

	pipe(pipe1);
	pipe(pipe2);

	pid_t child1 = fork();
	if (child1 == 0) {
		dup2(pipe1[1], STDOUT_FILENO); //Write to pipe1 (Output of ls -ltr)
		close(pipe1[0]);
		close(pipe1[1]);
		close(pipe2[0]);
		close(pipe2[1]);

		//Create Array for child1 --> ls -ltr
		char *args [] = {(char *)"ls", (char *)"-ltr", NULL};
		execvp(args[0], args);

		//If Fail
		perror("execution of ls fail");
		exit(1);
	}

	pid_t child2 = fork();
	if (child2 == 0) {
		dup2(pipe1[0], STDIN_FILENO);
		dup2(pipe2[1], STDOUT_FILENO);
		close(pipe1[0]);
		close(pipe1[1]);
		close(pipe2[0]);
		close(pipe2[1]);

		//Create array for child2 --> grep 3377
		char *args [] = {(char *)"grep", (char *)"3377", NULL};
		execvp(args[0], args);

		//If Fail
		perror("execution of grep fail");
		exit(1);
	}

	pid_t child3 = fork();
	if (child3 == 0) {
		dup2(pipe2[0], STDIN_FILENO);
		close(pipe1[0]);
		close(pipe1[1]);
		close(pipe2[0]);
		close(pipe2[1]);

		//Create array for child3 --> wc -l
		char *args [] = {(char *)"wc", (char *)"-l", NULL};
		execvp(args[0], args);

		//If Fail
		perror("execution of wc fail");
		exit(1);
	}
	
	exit(0);
}
