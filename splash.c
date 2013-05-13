#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

const char *SPLASH = "yodaSplash.txt";

// Open, read in, and write to STDOUT the given text
int main() {
	int fd = open(SPLASH, O_RDONLY);
	if (fd == -1) { return -1; }
	
	char prompt[1024];
	int ret = read(fd, prompt, 1024);
	if (ret == -1) { return -1; }

	ret = write(STDOUT_FILENO, prompt, ret);
	if (ret == -1) { return -1; }

	return 0;
}

