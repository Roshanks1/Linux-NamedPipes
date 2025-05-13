#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

#define MAX_CMDS 5
#define MIN_CMDS 2

int main(int argc, char* argv[]) {
	if (argc < MIN_CMDS + 1 || argc > MAX_CMDS + 1) {
		cerr << "Usage: " << argv[0] << " cmd1 cmd2 [cmd3 ... cmd5]" << endl;
		cout << "You must have at least 2 commands, and no more than 5..." << endl;
		return 1;
	}

	int num_cmds = argc - 1;
	int pipes[MAX_CMDS - 1][2]; //Max 4 pipes for 5 commands 
	
	//Create Pipes
	for (int i = 0; i < num_cmds - 1; ++i) {
		if (pipe(pipes[i]) == -1) {
			cout << "Pipe Error"; 
			exit(1);
		}
	}

	//Fork Child Processes
	for (int i = 0; i < num_cmds; ++i) {
		pid_t pid = fork();
		if (pid == 0) {
			//Redirect Input from previous pipe, IF not first
			if (i > 0) {
				dup2(pipes[i - 1][0], STDIN_FILENO);
			}
			
			//Redirect output to next pipe, IF not last
			if (i < num_cmds - 1) {
				dup2(pipes[i][1], STDOUT_FILENO);
			}
			
			//Close Pipes
			for (int j = 0; j < num_cmds - 1; ++j) {
				close(pipes[j][0]);
				close(pipes[j][1]);
			}
			
			//Prepare Array for Execution (utilizes bash shell)
			char* args[] = {(char*)"bash", (char*)"-c", argv[i + 1], NULL};
			execvp("bash", args);
			
			//If Something messed up
			cout << "exec failed";
			exit(1);
		} //If Children not forked properly
		else if (pid < 0) {
			cout << "fork failed";
			exit(1);
		}
	}

	//Parent closes all pipes
	for (int i = 0; i < num_cmds - 1; ++i) {
	close(pipes[i][0]);
	close(pipes[i][1]);
	}

	//Parent waits for all children to finish execution before return 0
	for (int i = 0; i < num_cmds; ++i) {
	wait(NULL);
	}

	return 0;
}

