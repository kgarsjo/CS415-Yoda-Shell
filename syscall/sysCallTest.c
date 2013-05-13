#define _GNU_SOURCE
#include <linux/errno.h>
#include <unistd.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define __NR_hello_kernel 337

int main() {
	printf("The result is %lu!\n", syscall(__NR_hello_kernel, 1));
	return 0;
}
