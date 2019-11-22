#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void changeDirCmd(char**);
pid_t execCmd(char**, const pid_t*, int);
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
        printf("Background\n");
        fflush(stdout);
        command[strlen(command) - 1] = '\0';
        execCmd(&command, &shellPid, 0);
      } else {

        if (strncmp(command, cdCmd, strlen(cdCmd)) == 0) {
          changeDirCmd(&command);
        } else if (strncmp(command, statusCmd, strlen(statusCmd)) == 0) {
          printStatusCmd();
        } else if (strncmp(command, exitCmd, strlen(exitCmd)) == 0) {
          exitShellCmd(&command, &shellPid);
        } else {
          execCmd(&command, &shellPid, 1);
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

pid_t execCmd(char** command, const pid_t* shellPid, int foreground) {
  char* args[512];
  char stdInPath[256] = "";
  char stdOutPath[256] = "";
  pid_t spawnPid = -5, actualPid = -5;
  int exitMethod = -5;
  char* shellVar = "$$";
  char* inOperator = "<";
  char* outOperator = ">";
  char* delimiter = " ";
  size_t argsMax = 512;

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

  printf("Infile: %s\n", stdInPath);
  printf("Outfile: %s\n", stdOutPath);
  for (size_t q = 0; q < i; q++) {
    printf("Args[%zu]: %s\n", q, args[q]);
  }

  spawnPid = fork();
  switch (spawnPid) {
    case -1:
      perror("Could not create a process.");
      return(-1);
    case 0:
      if (strlen(stdInPath) != 0) {
        childstdIn = open(stdInPath, O_RDONLY);

      }
      execvp(args[0], args);
      return(0);
    default:
      if(foreground) {
        actualPid = waitpid(spawnPid, &exitMethod, 0);
      }
      printf("In the parent - Child is %d\n", spawnPid);
      fflush(stdout);
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
