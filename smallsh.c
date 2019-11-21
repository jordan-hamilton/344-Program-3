#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void changeDirCmd(char**);
void execCmd(char**, int);
void exitShellCmd();
void printStatusCmd();

int main(int argc, char* argv[]) {
  pid_t shellPid = getpid();
  pid_t spawnPid = -5;
  int charsEntered = -5;
  char bg = '&';
  char comment = '#';
  char* cdCmd = "cd";
  char* exitCmd = "exit";
  char* statusCmd = "status";
  char* command = NULL;
  size_t commandSize = 0, argCount = 0, commandMax = 2049, argMax = 512, forkCount = 0;

  command = malloc(commandMax * sizeof(char));
  if (command == NULL)
    perror("Allocating memory for a command string failed.");

  do {
    printf(": ");
    fflush(stdout);
    charsEntered = getline(&command, &commandSize, stdin);
    command[charsEntered - 1] = '\0';

    if (command[0] == comment || strlen(command) == 0) {
      printf("Comment or empty string\n");
      fflush(stdout);
    } else if (command[strlen(command) - 1] == bg) {
      printf("Background\n");
      fflush(stdout);
      execCmd(&command, 0);
    } else {

      printf("Foreground\n");
      fflush(stdout);
      if (strncmp(command, cdCmd, strlen(cdCmd)) == 0) {
        changeDirCmd(&command);
      } else if (strncmp(command, statusCmd, strlen(statusCmd)) == 0) {
        printStatusCmd();
      } else if (strncmp(command, exitCmd, strlen(exitCmd)) == 0) {
        exitShellCmd();
      } else {
        printf("Time to do a thing\n");
        execCmd(&command, 1);
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

void execCmd(char** command, int foreground) {
  printf("Doing a thing.\n");
  fflush(stdout);
}

void exitShellCmd() {
  printf("Exiting the shell.\n");
  fflush(stdout);
}

void printStatusCmd() {
  printf("Printing the status.\n");
  fflush(stdout);
}
