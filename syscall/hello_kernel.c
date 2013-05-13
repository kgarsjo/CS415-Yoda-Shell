#include <linux/linkage.h>
#include <linux/kernel.h>

asmlinkage long sys_hello_kernel(int i) {
	printk(KERN_EMERG "Hello Kernel! This is for CIS 415!");
	return i;
}
