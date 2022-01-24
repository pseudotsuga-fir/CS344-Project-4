#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv) {
	(void)argc;

	int pfd[2];
	pipe(pfd);

	if (fork() == 0) {
		dup2(pfd[0], 0);
		close(pfd[1]);
		execlp("wc", "wc", "-l", NULL);
		perror("exec");
		exit(1);
	}

	dup2(pfd[1], 1);
	close(pfd[0]);
	execlp("ls", "ls", "-1a", argv[1], NULL);
}
