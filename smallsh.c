#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void changeDirCmd(char*[], const size_t*);
void parseCmd(char**, char*[], char[], char[], size_t*, const size_t*, const size_t*, const pid_t*);
void executeCmd(char*[], char[], char[], const size_t*, pid_t*, int*, int*, int);
void exitShellCmd(char**, pid_t*);
void printStatusCmd(const int*);

int main(int argc, char* argv[]) {
  pid_t shellPid = getpid();
  int foregroundOnly = 0, exitReceived = 0, exitMethod = -5, charsEntered = -5;
  char bg = '&';
  char comment = '#';
  char* cdCmd = "cd";
  char* exitCmd = "exit";
  char* statusCmd = "status";
  char stdInPath[256] = "", stdOutPath[256]= "";
  char* args[512];
  char* command = NULL;
  size_t argsCount = 0, argsMax = 512, commandSize = 0, commandMax = 2049;

  /* Create a signal handler to prevent SIGINT and one to catch SIGTSTP so we can toggle whether or not the user
   * can execute a process in the background */
  struct sigaction sigint = {0}, sigtstp = {0};


  do {
    argsCount = 0;
    printf(": ");
    fflush(stdout);
    charsEntered = getline(&command, &commandSize, stdin);
    command[charsEntered - 1] = '\0';

    /* Begin to process the string the user entered only if it didn't start with a # or have a zero-length string */
    if (command[0] != comment && strlen(command) != 0) {

      if (command[strlen(command) - 1] == bg) {
        /* Parse the command into an array and send the array to executeCmd(), with a special flag indicating that it
         * should be a background process since the last character in the command was an ampersand. */
        command[strlen(command) - 1] = '\0';
        parseCmd(&command, args, stdInPath, stdOutPath, &argsCount, &argsMax, &commandMax, &shellPid);
        executeCmd(args, stdInPath, stdOutPath, &argsCount, &shellPid, &exitMethod, &foregroundOnly, 0);
      } else {

        if (strncmp(command, cdCmd, strlen(cdCmd)) == 0) {
          /* Parse the command into an array (in case the new directory uses $$), then pass the arguments into our
           * homemade cd function */
          parseCmd(&command, args, stdInPath, stdOutPath, &argsCount, &argsMax, &commandMax, &shellPid);
          changeDirCmd(args, &argsCount);
        } else if (strncmp(command, statusCmd, strlen(statusCmd)) == 0) {
          /* Print the exit method and associated signal or exit code of the most recently completed command. */
          printStatusCmd(&exitMethod);
        } else if (strncmp(command, exitCmd, strlen(exitCmd)) == 0) {
          /* Set a flag to break out of the do-while loop and print a command to indicate that the shell is being
           * terminated. */
          exitReceived = 1;
          exitShellCmd(&command, &shellPid);
        } else {
          /* Parse the user's command and call executeCmd() with a flag to indicate that this is a foreground process. */
          parseCmd(&command, args, stdInPath, stdOutPath, &argsCount, &argsMax, &commandMax, &shellPid);
          executeCmd(args, stdInPath, stdOutPath, &argsCount, &shellPid, &exitMethod, &foregroundOnly, 1);
        }

      }

    }

    free(command);
    command = NULL;
  } while (!exitReceived);


  return(0);
}

void changeDirCmd(char* args[], const size_t* argsCount) {
  int result = -5;
  if(*argsCount == 1) {
    result = chdir(getenv("HOME"));
  } else {
    result = chdir(args[1]);
  }
  if (result != 0) {
    perror("Could not change the working directory to the specified path");
  }

  /* Free memory that was allocated for the arguments that were passed to the child. */
  for (size_t i = 0; i < *argsCount; i++) {
    free(args[i]);
    args[i] = NULL;
  }

}

void parseCmd(char** command, char* args[], char stdInPath[], char stdOutPath[], size_t* argsCount, const size_t* argsMax, const size_t* commandMax, const pid_t* shellPid) {
  char* pidExpansion = "$$";
  char* inOperator = "<";
  char* outOperator = ">";
  char* delimiter = " ";
  char* pidVarLocation;

  strcpy(stdInPath, "");
  strcpy(stdOutPath, "");

  for (size_t i = 0; i < *argsMax; i++) {
    args[i] = NULL;
  }

  char* buffer = strtok(*command, delimiter);

  while (buffer != NULL) {
    if (strcmp(buffer, inOperator) == 0) {
      buffer = strtok(NULL, delimiter);
      strcpy(stdInPath, buffer);
    } else if (strcmp(buffer, outOperator) == 0) {
      buffer = strtok(NULL, delimiter);
      strcpy(stdOutPath, buffer);
    } else {
      args[*argsCount] = malloc(*commandMax * sizeof(char));
      if (args[*argsCount] == NULL)
        perror("Allocating memory for a command argument failed.");
      memset(args[*argsCount], '\0', *commandMax);
      pidVarLocation = strstr(buffer, pidExpansion);
      if (pidVarLocation != NULL) {
        char temp[2049] = "";
        strncpy(args[*argsCount], buffer, (int) (pidVarLocation - buffer));
        sprintf(temp, "%d", *shellPid);
        strcat(args[*argsCount], temp);
        strcat(args[*argsCount], (pidVarLocation + 2));
      } else {
        strcpy(args[*argsCount], buffer);
      }

      *argsCount += 1;
    }

    buffer = strtok(NULL, delimiter);
  }

}

void executeCmd(char* args[], char stdInPath[], char stdOutPath[], const size_t* argsCount, pid_t* shellPid, int* exitMethod, int* foregroundOnly, int foreground) {
  pid_t spawnPid = -5, actualPid = -5;
  int childStdIn = -5, childStdOut = -5, result = -5;

  spawnPid = fork();
  switch (spawnPid) {
    case -1:
      perror("Could not create a process.");
      break;
    case 0:
      if (strlen(stdInPath) != 0 || !foreground) {
        if (strlen(stdInPath) != 0)
          childStdIn = open(stdInPath, O_RDONLY);
        else
          childStdIn = open("/dev/null", O_RDONLY);

        if (childStdIn == -1) {
          perror("Couldn't access the specified location for standard input");
          exit(1);
        }

        result = dup2(childStdIn, 0);
        if (result == -1) {
          perror("Couldn't redirect standard input");
          exit(1);
        }

      }

      if (strlen(stdOutPath) != 0 || !foreground) {
        if (strlen(stdOutPath) != 0)
          childStdOut = open(stdOutPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else
          childStdOut = open("/dev/null", O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (childStdOut == -1) {
          perror("Couldn't access the specified location for standard output");
          exit(1);
        }

        result = dup2(childStdOut, 1);
        if (result == -1) {
          perror("Couldn't redirect standard output");
          exit(1);
        }
      }

      result = execvp(args[0], args);
      if (result == -1)  {
        printf("Command not found: %s\n", args[0]);
        fflush(stdout);
        exit(1);
      }

    default:
      /* Check if the flags to send foreground command  were set when executeCmd() was called, then call waitpid with
       * the correct flags, sending the process to the background and printing the ID if a background was requested. */
      if (foreground || *foregroundOnly) {
        actualPid = waitpid(spawnPid, exitMethod, 0);
      } else {
        actualPid = waitpid(spawnPid, exitMethod, WNOHANG);
        printf("Process ID: [%d]\n", spawnPid);
        fflush(stdout);
      }

      /* Check for a return value from waitpid that's greater than 0 to indicate that any background child process
       * may have terminated, and output a message letting the user know the process ID that completed, along with
       * the exit code or terminating signal. */
      while ((actualPid = waitpid(-1, exitMethod, WNOHANG)) > 0) {
        printf("[%d]: ", actualPid);
        fflush(stdout);
        printStatusCmd(exitMethod);
      }

      /* Free memory that was allocated for the arguments that were passed to the child. */
      for (size_t i = 0; i < *argsCount; i++) {
        free(args[i]);
        args[i] = NULL;
      }

  }
}

void exitShellCmd(char** command, pid_t* shellPid) {
  printf("\n[Exiting smallsh]\n");
  fflush(stdout);
}

void printStatusCmd(const int* exitMethod) {
  /* Check if the exit method was via a signal or normal termination stored from the most recent call to waitpid().
   * Print the appropriate status (either the signal or exit code) based on whether it was a signal or normal
   * termination of a process.*/
  if (WIFEXITED(*exitMethod)) {
    printf("Process terminated with exit code %d.\n", WEXITSTATUS(*exitMethod));
  } else if (WIFSIGNALED(*exitMethod)){
    printf("Process terminated by signal %d.\n", WTERMSIG(*exitMethod));
  }
  fflush(stdout);
}
