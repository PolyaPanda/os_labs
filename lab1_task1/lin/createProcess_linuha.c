#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

void signHandler(int signal)
{}

int main()
{
	pid_t cpid, w;
	int status;

	cpid = fork();
	if (cpid == -1) {
		perror("fork\n");
		exit(EXIT_FAILURE);
	}

	if (cpid == 0) {
		sleep(7);
		return 1;
	}
	else {
		signal(SIGINT, signHandler);
		signal(SIGTERM, signHandler);
		w = wait(&status);
		if (w == -1) {
			perror("waitpid\n");
			exit(EXIT_FAILURE);
		}

		if (WIFEXITED(status))
			printf("\nNormal exit with status %d\n", WEXITSTATUS(status));
		else if (WIFSIGNALED(wstatus))
			printf("\nKilled by signal %d\n", WTERMSIG(status));
		else if (WIFSTOPPED(status))
			printf("\nStopped by signal %d\n", WSTOPSIG(status));
		else if (WIFCONTINUED(status))
			printf("\nContinued\n");
	}
}
