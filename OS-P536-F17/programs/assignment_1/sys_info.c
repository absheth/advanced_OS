#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

char* joinstrings(const char *,const char *); 

int main(int nargs, char *args[]){
	
	pid_t child_or_parent;
	char message_from_parent[20];
	int inout[2], readmessage;

	pipe(inout); //initiate the pipe for communication between the two processes

	child_or_parent = fork(); // Creating a new process

	//in case there is some error while forking
	if(child_or_parent < 0)	
	{  
		perror("Error while forking. Child process not created.");
		exit(-1);
	}

	//in case the process is a child process
	if(child_or_parent == 0){
		printf("This is a Child_Process and ID = %i\n", getpid());
		
		close(inout[1]); // child process closed output side of pipe
		readmessage = read(inout[0], message_from_parent, sizeof(message_from_parent)); //Read message in the pine from parent
        printf("\n");
        
        char* joinedstring = joinstrings("/bin/",message_from_parent);
        execl(joinedstring,message_from_parent,((strcmp(message_from_parent,"echo")==0)?"Hello World!":0),NULL); //system call
		
		free(joinedstring); // relase the allocated memory.
    }
	else {
		printf("\n");
		printf("This is a Parent_Process and ID = %i\n", getpid());
		
		close(inout[0]); // parent process closed input side of pipe
		write(inout[1], args[1], (strlen(args[1])+1));
		//printf("Message passed on to child --> %s\n", args[1]);
		printf("\n");
		wait(NULL); // waiting to be notified that the child has completed its execution

	}
	
	printf("\n");
	
	return 0;
}

//function to concatenate two strings
char* joinstrings(const char *string1,const char *string2){
	char *finalstring = malloc(strlen(string1) + strlen(string2) + 1);
	strcpy(finalstring, string1);
	strcat(finalstring, string2);
	return finalstring;
}