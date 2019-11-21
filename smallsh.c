#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  int shellPid = getpid();
  int forkCount = 0, charsEntered = -5;
  char bg = '&';
  char comment = '#';
  char* exit = "exit";
  char* command = NULL;
  size_t commandSize = 0, commandMax = 2049;

  command = malloc(commandMax * sizeof(char));
  if (command == NULL)
    perror("Allocating memory for a command string failed");

  do {
    printf(": ");
    fflush(stdout);
    charsEntered = getline(&command, &commandSize, stdin);
    command[charsEntered - 1] = '\0';

    if (command[0] == comment || strlen(command) == 0) {
      printf("Comment or empty string\n");
    } else if (command[strlen(command) - 1] == bg)
      printf("Background\n");
    else {
      printf("Foreground\n");
    }
    fflush(stdout);
  } while (strcmp(command, exit) != 0);

  free(command);
  command = NULL;
  return(0);
}
