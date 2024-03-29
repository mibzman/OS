// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed:_____________________________________ Date:_____________

// 3460:426 Lab 4A - Basic C shell

/* Basic shell */

/*
 * This is a very minimal shell. It finds an executable in the
 * PATH, then loads it and executes it (using execv). Since
 * it uses "." (dot) as a separator, it cannot handle file
 * names like "minishell.h"
 *
 * The focus on this exercise is to use fork, PATH variables,
 * and execv. 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

// #include <fcntl.h>

// #include <sys/types.h>
// #include <errno.h>
// #include <dirent.h>
// #include <grp.h>
// #include <pwd.h>
// #include <time.h>

#define MAX_ARGS  64
#define MAX_ARG_LEN 16
#define MAX_LINE_LEN  80
#define WHITESPACE  " ,\t\n"

struct command_t {
   char *name;
   int argc; //arg count
   char *argv[MAX_ARGS]; //arg values
};

/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);

int main(int argc, char *argv[]) {
   int pid;
   int status;
   char cmdLine[MAX_LINE_LEN];
   struct command_t command;
   struct command_t command2;
   int twoCommands = 0;

   while (1) {
      printPrompt();
      /* Read the command line and parse it */
      readCommand(cmdLine);
      // ...
      parseCommand(cmdLine, &command);
      // ...
      // printf(command.name);
      command.argv[command.argc] = NULL;

      if(strcmp(command.name, "C") == 0) {
        char* file1 = command.argv[1];
        char* file2 = command.argv[2];

        if(command.argc > 2 && strlen(file1) > 0 && strlen(file2) > 0) {
          command.name = "cp";
        } else {
          printf("insufficient parameters\n");
        }
      } else if(strcmp(command.name, "D") == 0) {
        char* file = command.argv[1];
        if(command.argc > 1 && strlen(file) > 0) {

          command.name = "rm";

        } else {
          printf("insufficient parameters\n");
        }
      } else if(strcmp(command.name, "E") == 0) {
        printf(command.argv[1]);
        printf("\n");
      } else if(strcmp(command.name, "H") == 0) {
        printf("C file1 file2 Copy; create file2, copy all bytes of file1 to file2 without deleting file1. \n D file Delete; the named file. \n E comment Echo; display comment on screen followed by a new line\n H Help; display the user manual\n L List; the contents of the current directory; see below. \n M file Make; create the named text file by launching a text editor. \n P file Print; display the contents of the named file on screen. \n Q Quit; the shell. \n S Surf; the web by launching a browser as a background process. \n W Wipe; clear the screen. \n X program Execute; the named program. \n");
      } else if(strcmp(command.name, "M") == 0) {
        char* file = command.argv[1];
        if(command.argc > 1 && strlen(file) > 0) {

          command.name = "nano";

        } else {
          printf("insufficient parameters\n");
        }
      }  else if(strcmp(command.name, "P") == 0) {
        char* file = command.argv[1];
        if(command.argc > 1 && strlen(file) > 0) {

          command.name = "more";

        } else {
          printf("insufficient parameters\n");
        }
      } else if(strcmp(command.name, "S") == 0) {
        command.name = "firefox&";
      } else if(strcmp(command.name, "W") == 0) {
        command.name = "clear";
      } else if(strcmp(command.name, "Q") == 0) {
        command.name = "clear";
      } else if(strcmp(command.name, "L") == 0) {
        printf("\n");
        command.name = "pwd";

        command2 = command;
        command2.name = "ls";
        command2.argv[1] = "-l";

        twoCommands = 1;
      } else if(strcmp(command.name, "X") == 0) {
        printf("\n");

        command.name = command.argv[1];

        command.argc -= 1;
        for (int i = command.argc; i > i; i--){        
            command.argv[i]=command.argv[i-1];
        }
        command.argv[command.argc] = NULL;
      }  else {
      }

      /* Create a child process to execute the command */
      if ((pid = fork()) == 0) {
         /* Child executing command */
         execvp(command.name, command.argv);
      }
      /* Wait for the child to terminate */
      wait(&status);

      if (twoCommands == 1) {
        printf("\n");
        if ((pid = fork()) == 0) {
           /* Child executing command */
           execvp(command2.name, command2.argv);
        }
        wait(&status);
      }

   }

   /* Shell termination */
   printf("\n\n shell: Terminating successfully\n");
   return 0;
}

/* End basic shell */

/* Parse Command function */

/* Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char *). The last
 * element in argv[] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[0], the second in
 * argv[1], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char *cLine, struct command_t *cmd) {
  int argc;
  char **clPtr;
  /* Initialization */
  clPtr = &cLine; /* cLine is the command line */
  argc = 0;
  cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
  /* Fill argv[] */
  while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
    cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
  }

  /* Set the command name and argc */
  cmd->argc = argc-1;
  cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
  strcpy(cmd->name, cmd->argv[0]);

  return 1;
}

/* End parseCommand function */

/* Print prompt and read command functions - pp. 79-80 */

void printPrompt() {
   /* Build the prompt string to have the machine name,
    * current directory, or other desired information
    */
   char* promptString = "linux sb205 |>";
   printf("%s ", promptString);
}

void readCommand(char *buffer) {
   /* This code uses any set of I/O functions, such as those in
    * the stdio library to read the entire command line into
    * the buffer. This implementation is greatly simplified,
    * but it does the job.
    */
   fgets(buffer, 80, stdin);
}

/* End printPrompt and readCommand */