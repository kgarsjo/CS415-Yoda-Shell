/*--------------------------------------------------//
  Yoda Shell, a basic shell for UNIX
	
	Author:		Kevin Garsjo
	Course:		CS 415, Operating Systems, UO
	Last Modified:	4/23/2012	
//--------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


/*--------------------------------------------------//
  :: Definitions	
//--------------------------------------------------*/
#define TRUE 1
#define FALSE 0


/*--------------------------------------------------//
  :: Constants	
//--------------------------------------------------*/
const size_t BUFSIZE = 1024;
const char NULL_TERM = '\0';
const int EXIT_NOCMD = 127;

const char *EXIT_FLAG = "exit";
const char *SPLASH_FILE = "splash";

const char *NO_KILL_MSG = "  [Yoda]::> Away put your weapon, I mean you no harm.\n";
const char *EXIT_MSG = "  [Yoda]::> Soon I will rest, yes. Forever sleep. Earned it I have.\n";
const char *KILL_MSG_PREF = "  [Yoda]::> When ";
const char *KILL_MSG_SUFF = " seconds old YOU reach, look as good you will not, hmm?\n";
const char *PROMPT = "yoda-sh# ";
const char *FORK_FAIL = "Fork failed!\n";


/*--------------------------------------------------//
  :: Globals
//--------------------------------------------------*/
char *newArg[] = {"", NULL};
pid_t pid;
char *killStr;


/*--------------------------------------------------//
  :: Function Prototypes
//--------------------------------------------------*/
void handler(int signum);
int getLen(const char *str);
int compareStr(const char *str1, const char *str2);
int a_to_i(const char *str);
char **parseArgs(char *buf);
void printSplash();


/*--------------------------------------------------//
  :: Program Entry
//--------------------------------------------------*/
int main(int argc, char **argv) {

	// Set up bufer and initialize variables
	char *buf = (char *) malloc(BUFSIZE * sizeof(char));
	int childResult, ret;

	// Convert the argument to int if it exists
	int killTime = 0;
	if (argc >= 2) {
		killStr = argv[1];
		killTime = a_to_i(argv[1]);
	}

	printSplash();

	while (TRUE) {
		// Write out shell prompt:
		ret = write(STDOUT_FILENO, PROMPT, getLen(PROMPT));
		if (ret == -1) { perror("write"); }

		// Read in the shell commands:
		int numRead = read(STDIN_FILENO, buf, BUFSIZE);
		if (numRead == -1) { perror("readCmd"); }
		buf[numRead - 1] = NULL_TERM;
		char **args = parseArgs(buf);

		// Check for exit flag
		if ( compareStr(buf, EXIT_FLAG) ) {
			ret =  write(STDOUT_FILENO, EXIT_MSG, getLen(EXIT_MSG));
			if (ret < 0) { perror("write"); }
		
			free(args);
			free(buf);
			break;
		}

		// Register Signal Handler
		signal(SIGALRM, handler);
		alarm(killTime);
		
		// Start the child process and wait
		pid = fork();
		if (pid < 0) {		// Error case for fork
			ret = write(STDOUT_FILENO, FORK_FAIL, getLen(FORK_FAIL) );
			if (ret == -1) { perror("write"); }
		} else if (pid == 0) {	// Child case
			execve(buf, args, newArg);
			perror(buf);
			_exit(EXIT_NOCMD);
		} else {	//Parent case
			wait(&childResult);
			alarm(0);
			if ( childResult == 0 ) {
				write(STDOUT_FILENO, NO_KILL_MSG, getLen(NO_KILL_MSG));
			}
		}  // end fork checks

		free(args);
	}  // end while loop
	
	return 0;
}

/*--------------------------------------------------//
  void handler(int signum) - Handles all signals it's
	registered to catch.
  int signum - the signal caught by this handler
//--------------------------------------------------*/
void handler(int signum) {
	int ret = write(STDOUT_FILENO, KILL_MSG_PREF, getLen(KILL_MSG_PREF) );
	if (ret == -1) { perror("write"); }

	ret = write(STDOUT_FILENO, killStr, getLen(killStr) );
	if (ret == -1) { perror("write"); }

	ret = write(STDOUT_FILENO, KILL_MSG_SUFF, getLen(KILL_MSG_SUFF) );
	if (ret == -1) { perror("write"); } 

	kill(pid, SIGKILL);
}

/*--------------------------------------------------//
  int compareStr(const char *str1, const char *str2)
	- Compares two c-strings for equality
  char *str1, str2 - The strings to compare
  returns TRUE on equality, FALSE otherwise
//--------------------------------------------------*/
int compareStr(const char *str1, const char *str2) {
	int len1 = getLen(str1), len2 = getLen(str2);
	if (len1 != len2) { return FALSE; }

	int i;
	for (i = 0; i < len1; i++) {
		if ( str1[i] != str2[i] ) {
			return FALSE;
		}
	}
	return TRUE;
}

/*--------------------------------------------------//
  int getLen(const char *str) - Returns the str len
//--------------------------------------------------*/
int getLen(const char *str) {
	int size;
	for (size = 0; str[size] != NULL_TERM; size++);
	return size;
}

/*--------------------------------------------------//
  int a_to_i(const char *str) - Converts the str to int
  Returns valid number, or 0 on bad input
//--------------------------------------------------*/
int a_to_i(const char *str) {
	const int ASCII_NUMERIC_START = 48;

	int total = 0, len = getLen(str);
	int i;
	for (i = 0; i < len; i++) {
		total *= 10;	// Set up for next single digit
	
		// Check if current digit is valid, add it to total
		int currDigit = str[i] - ASCII_NUMERIC_START;
		if (currDigit / 10 != 0 || currDigit < 0) {
			return 0;
		}
		total += currDigit;
	}
	return total;
}

char **parseArgs(char *buf) {
	// Count whitespaces in buf; num whitespaces will
	// be >= num total args
	int numSpaces = 2, i;
	for (i = 0; buf[i] != NULL_TERM; i++) {
		if (buf[i] == ' ') {
			numSpaces ++;
		}
	}

	// Allocate new char**
	char **args = (char **) calloc(numSpaces, sizeof(char**));

	args[0] = buf;	

	int currPtr = 1;
	for (i = 0; buf[i] != NULL_TERM; i++) {
		if (buf[i] == ' ') {
			buf[i] = NULL_TERM;
			args[currPtr] = buf + i + 1;
			currPtr ++;
		}
	}
	return args;
}

/*--------------------------------------------------//
  void printSplash() - Runs the splash program
	Note: The splash program utilizes the open()
	system call not allowed by this assignment,
	which is why splash is its own program and not
	incorporated into this shell. 
//--------------------------------------------------*/
void printSplash() {
	pid_t pid_splash = fork();
	if (pid_splash < 0) {

	} else if (pid_splash == 0) {
		execve(SPLASH_FILE, newArg, newArg);
		perror("splash");
	} else {
		wait(NULL);
	}
}
