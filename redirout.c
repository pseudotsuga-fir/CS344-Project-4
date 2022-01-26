#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {
	if (argc < 3) {
		printf("Usage: redirout [OUTPUT_FILE] [COMMAND] [ARGS]\n");
		exit(1);
	}
	
	int fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0644);
	int pfd[2];
	pipe(pfd);

	if(fork() == 0) {
		dup2(pfd[1], 1);
		close(pfd[0]);
		char** argv_args = argv + 2;
		execvp(argv[2], argv_args);
		perror("execvp");
		exit(1);
	}
	close(pfd[1]);
	char fileContents[2048];
	int count = 1;
	while(count > 0) {
		count = read(pfd[0], fileContents, 2048);
		write(fd, fileContents, count);
	}
	close(pfd[0]);
	close(fd);

}
