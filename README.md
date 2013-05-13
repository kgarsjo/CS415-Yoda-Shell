Author:	Kevin Garsjo

Sources Files:
- Makefile
- README
- shell.c
- splash.c
- yodaSplash.txti
  syscall [DIR]
	- hello_kernel.c
	- sysCallTest.c
	- syslog
	- syslog_snapshot.txt

Extra Credit:
SIGSTOP shares SIGKILL's non-catchable and non-ignorable property.
SIGHUP, SIGINT, SIGPIPE, SIGALRM, SIGTERM, SIGUSR1, and SIGUSR2 all terminate if not handled.
This info is found in man signal(7)

Work Accomplished:
[X] Shell accepts input
[X] Shell forks child process
[X] Shell executes given input
[X] Shell gains helper functions (getLen, compareStr, etc)
[X] Shell handles SIGALRM and outputs context-correct statements
[X] Shell develops personality (sentience soon to follow)
[X] Shell handles erroneous retuns from syscalls
[X] Shell implements local atoi function
[X] Shell implements argument parsing
[X] Shell passes valgrind full leak checks
[X] System call written
[X] Kernel compiled
[X] System call tested and documented

Code Layout:
 - 'shell.c' serves as the primary workhorse of the shell. It contains the main app loop and the forking.
 - 'splash.c' simply opens, reads contents from, and writes to STDOUT the ascii art image in 'yodaSplash.txt'. This functionality was explicitly omitted from 'shell.c' due to open() not being on the list of allowed shell syscalls. Therefore, splash is forked from the shell just like any other program.
 - 'sysCallTest.c' runs the added system call, 'hello_kernel.'
 - 'syslog_snapshot.txt' contains the truncated kernel log of the system call, and 'syslog' contains the full log.


Hours Spent:
Shell: Roughly 25, maybe 30 at most. The time would have been reduced if not for a VirtualBox vdmk corruption.
Syscall: About 3 hours of actual work, and about 5+ hours of compilation.

Effort:
Shell: Of the 25 hours spent, most were put toward working on the extras and trying out clever or not-so-clever things. The core of the shell was done in about 10 hours.
Syscall: The linked guide to compililng the kernel worked just fine, but the book's system call guide doesn't match up well to linux-2.6.32's files. There was a fair amount of poking around that needed to be done to get anywhere, and there was no assurance that the kernel compile would accept the changes.
