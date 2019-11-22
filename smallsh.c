#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void changeDirCmd(char**);
void parseCmd(char**, char*[], char[], char[], const pid_t*);
pid_t executeCmd(char*[], char[], char[], const pid_t*, int);
void exitShellCmd(char**, const pid_t*);
void printStatusCmd();

int main(int argc, char* argv[]) {
  pid_t shellPid = getpid();
  int charsEntered = -5;
  char bg = '&';
  char comment = '#';
  char* cdCmd = "cd";
  char* exitCmd = "exit";
  char* statusCmd = "status";
  char stdInPath[256] = "", stdOutPath[256]= "";
  char* args[512];
  char* command = NULL;
  size_t commandSize = 0, commandMax = 2049;

  command = malloc(commandMax * sizeof(char));
  if (command == NULL)
    perror("Allocating memory for a command string failed.");

  do {
    memset(command, '\0', commandMax);
    printf(": ");
    fflush(stdout);
    charsEntered = getline(&command, &commandSize, stdin);
    command[charsEntered - 1] = '\0';

    if (command[0] != comment && strlen(command) != 0) {

      if (command[strlen(command) - 1] == bg) {
        command[strlen(command) - 1] = '\0';
        parseCmd(&command, args, stdInPath, stdOutPath, &shellPid);
        executeCmd(args, stdInPath, stdOutPath, &shellPid, 0);
      } else {

        if (strncmp(command, cdCmd, strlen(cdCmd)) == 0) {
          changeDirCmd(&command);
        } else if (strncmp(command, statusCmd, strlen(statusCmd)) == 0) {
          printStatusCmd();
        } else if (strncmp(command, exitCmd, strlen(exitCmd)) == 0) {
          exitShellCmd(&command, &shellPid);
        } else {
          parseCmd(&command, args, stdInPath, stdOutPath, &shellPid);
          executeCmd(args, stdInPath, stdOutPath, &shellPid, 1);
        }

      }

    }
  } while (strcmp(command, exitCmd) != 0);

  free(command);
  command = NULL;
  return(0);
}

void changeDirCmd(char** command) {
  if(strcmp(*command, "cd") == 0) {
    chdir(getenv("HOME"));
  } else {
    chdir(*command + 3);
  }
}

void parseCmd(char** command, char* args[], char stdInPath[], char stdOutPath[], const pid_t* shellPid) {
  char* shellVar = "$$";
  char* inOperator = "<";
  char* outOperator = ">";
  char* delimiter = " ";
  size_t argsMax = 512;

  strcpy(stdInPath, "");
  strcpy(stdOutPath, "");

  for (size_t i = 0; i < argsMax; i++) {
    args[i] = NULL;
  }

  char* buffer = strtok(*command, delimiter);
  size_t i = 0;

  while (buffer != NULL) {
    if (strcmp(buffer, inOperator) == 0) {
      buffer = strtok(NULL, delimiter);
      strcpy(stdInPath, buffer);
    } else if (strcmp(buffer, outOperator) == 0) {
      buffer = strtok(NULL, delimiter);
      strcpy(stdOutPath, buffer);
    } else {

      args[i] = strdup(buffer);
      i++;
    }

    buffer = strtok(NULL, delimiter);
  }

}

pid_t executeCmd(char* args[], char stdInPath[], char stdOutPath[], const pid_t* shellPid, int foreground) {
  pid_t spawnPid = -5, actualPid = -5;
  int exitMethod = -5;
  int childStdIn = -5, childStdOut = -5, result = -5;

  spawnPid = fork();
  switch (spawnPid) {
    case -1:
      perror("Could not create a process.");
      return(-1);
    case 0:
      if (strlen(stdInPath) != 0 || !foreground) {
        if (strlen(stdInPath) != 0)
          childStdIn = open(stdInPath, O_RDONLY);
        else
          childStdIn = open("/dev/null", O_RDONLY);

        if (childStdIn == -1)
          perror("Couldn't open the file specified for standard input");

        result = dup2(childStdIn, 0);
        if(result == -1)
          perror("Couldn't redirect standard input");
      }

      if (strlen(stdOutPath) != 0 || !foreground) {
        if (strlen(stdOutPath) != 0)
          childStdOut = open(stdOutPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else
          childStdOut = open("/dev/null", O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (childStdOut == -1)
          perror("Couldn't open the file specified for standard output");

        result = dup2(childStdOut, 1);
        if(result == -1)
          perror("Couldn't redirect standard output");
      }

      execvp(args[0], args);
      return(0);
    default:
      if(foreground) {
        actualPid = waitpid(spawnPid, &exitMethod, 0);
      }
      return(spawnPid);
  }
}

void exitShellCmd(char** command, const pid_t* shellPid) {
  free(command);
  command = NULL;
  printf("Exiting the shell.\n");
  fflush(stdout);
}

void printStatusCmd() {
  printf("Printing the status.\n");
  fflush(stdout);
}
