#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}


int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;


	while(1) {			
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();

		printf("Command entered: %s (remove this debug output later)\n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

       //do whatever you want with the commands, here we just print them
        int length = 0;
		for(i=0;tokens[i]!=NULL;i++){
            length++;
			printf("found token %s (remove this debug output later)\n", tokens[i]);
		}
        pid_t pid = fork();

        if(pid < 0){
            printf("Fork failed");
            return  1;
        }else if (pid == 0){
            if(strcmp(tokens[0], "cd") == 0){
                if(tokens[1] ==NULL){
                    printf("Please provide directory to enter \n");
                }else{
                    if(chdir(tokens[1])!=0){
                        printf("Incorrect command to the display and prompting for the next command\n");
                    }
                }
            }else {
                if (strcmp(tokens[length - 1], "&") == 0) {
                    tokens[length - 1] = NULL;
                    if (execvp(tokens[0], tokens) < 0) {
                        printf("Error: Unable to execute command\n");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    if (execvp(tokens[0], tokens) < 0) {
                        printf("Error: Unable to execute command\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }else{
            int status;

            if(strcmp(tokens[length-1], "&") == 0) {
                waitpid(pid, &status, WNOHANG);
            }  else {
                waitpid(pid, &status, 0);
            }
        }


       
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
