#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>


char line[200];
//array for the arguments				
char *argv[100]; 			
char * history[10];

//child process
pid_t pid;
char i;												
			
// signal handler								
void log_handler(int sig);			


void log_handler(int sig){
	//creates audit log file
	FILE *pFile;
        pFile = fopen("audit.log", "w");
        
        for (int i = 0; i < 10; i++)
         {
        	fprintf(pFile, "%s\n", history[i]);
         }
       
        //closes the file
        fclose(pFile);
        
        
        // Stop the program
        exit(0);
        
        
        
}

int main()
{
	signal(SIGCHLD, log_handler);

	// starts the shell with while
	while(1){
		
        //gets command from user in console/terminal
	printf("Shell:");
	fgets(line, 200, stdin);
	// removes the newline
	if ((strlen(line) > 0) && (line[strlen (line) - 1] == '\n'))
	{
        	line[strlen (line) - 1] = '\0';
        	
        }


		// checks from empty command lines
		if(!strcmp("", line))
		{
		 	continue;
		}

                if(!strcmp("exit", line))
                {
                 	break;
                }
                
                

       //pointer
	char *ptr;
	i = 0;
	ptr = strtok(line, " ");
	
	while(ptr != NULL)
	{
		argv[i] = ptr;
		i++;
		ptr = strtok(NULL, " ");
	}


	if(!strcmp("&", argv[i-1]))
	{
		argv[i-1] = NULL;
		argv[i] = "&";
	}else
	{
		argv[i] = NULL;
	}

		// fork process
		pid = fork();
		if(-1 == pid)
		{
			printf("error child process\n");
		}
		else if(0 == pid)
		{
			execvp(argv[0], argv);
		}
		else{
			// waits for the command 
			if(NULL == argv[i]) waitpid(pid, NULL, 0);
		}
	}

	return 0;
}




