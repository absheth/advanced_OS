/* xsh_process_ring.c - xsh_process_ring */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <process_ring.h>
#include <prototypes.h>

/*------------------------------------------------------------------------
 * xsh_process_ring - Command to run multiple process in a synchronized fashion.
 *------------------------------------------------------------------------
 */

void print_usage(void);

shellcmd xsh_process_ring (int nargs, char *args[]) {
	
	int no_of_process 	= 	2; // Default number of processes
	int no_of_rounds	=	3; // Default number of rounds
	char *implementation_type = "poll"; // Default implementation type
	char *endp;
  pid32 mailbox[100]; // It hold PIDs of the processes
  uint32 start_time;
  uint32 end_time;
  volatile int32 inbox[100];
  	int i;
    for(i = 1; i < nargs; i++){
    	
    	if (0 == strncmp("-p", args[i], 3) || 0 == strncmp("--processes", args[i], 12)){
      		/* parse numeric argument to -p */
      		if (! (i + 1 < nargs)) {
        		/* No argument after flag */
        		print_usage();
        		printf("-p or --processes flag expected an argument\n");
        		return SHELL_ERROR;
      		}
      		
          no_of_process = atoi(args[i+1]);
      		if (endp == args[i + 1]) {
        		/* The end pointer hasn't advanced,
           			No number was parsed.*/
        		print_usage();
        		printf("-p or --processes flag expected an integer\n");
        		return SHELL_ERROR;
      		} else if (!isdigit(*args[i+1])){
       			 //There was trailing garbage in the string that
          		//wasn't converted to an integer.
        		print_usage();
        		printf("-p or --processes recieved invalid integer\n");
        		return SHELL_ERROR;
      		} else if (( no_of_process < 0 || no_of_process > 64)) {
        		/* The number is out of range */
        		print_usage();
        		printf("-p or --processes flag expected a number between 0 - 64\n");
        		return SHELL_ERROR;
      		}
      		/* Skip pass the numeric argument,
         	that was successfully parsed */
      		i += 1; 
    	} else  if (0 == strncmp("-r", args[i], 3) || 0 == strncmp("--rounds", args[i], 9))  { 
      
      	/* parse numeric argument to -p */
      		if (! (i + 1 < nargs)) {
        		/* No argument after flag */
        		print_usage();
        		printf("-r or -- rounds flag expected an argument\n");
        		return SHELL_ERROR;
      		}
      		no_of_rounds = atoi(args[i+1]);
      		if (endp == args[i + 1]) {
        		/* The end pointer hasn't advanced,
           			No number was parsed.*/
        		print_usage();
        		printf("-r or --rounds flag expected an integer\n");
        		return SHELL_ERROR;
      		} else if (!isdigit(*args[i+1])){
       			/* There was trailing garbage in the string that
          		wasn't converted to an integer. */
        		print_usage();
        		printf("-r or --rounds recieved invalid integer\n");
        		return SHELL_ERROR;
      		} else if (no_of_rounds < 0) {
            /* The number is out of range */
            print_usage();
            printf("-r or --rounds flag expected a number greater than 0.\n");
            return SHELL_ERROR;
          }
      		/* Skip pass the numeric argument,
         	that was successfully parsed */
      		i += 1;
      	} else  if (0 == strncmp("-i", args[i], 3))  {
      
      		if (! (i + 1 < nargs)) {
        		print_usage();
        		printf("-i flag expected an argument\n");
        		return SHELL_ERROR;
      		}
      		
      		implementation_type = args[i+1];

      		/*Check if the input for implementation type is correct.*/
      		if (endp == args[i + 1]) {
        		/* The end pointer hasn't advanced,
           			No number was parsed.*/
        		print_usage();
        		printf("-r flag expected an input\n");
        		return SHELL_ERROR;
      		} else if(strncmp(implementation_type, "poll",4) != 0 && strncmp(implementation_type, "sync", 4) != 0){
      			print_usage();
      			printf("-i received invalid input. Expected 'poll' or 'sync' ");
      			printf("\n");
      			return SHELL_ERROR;
      		} 
      		
      		i += 1;
      	} else if (0 == strncmp("--help", args[i], 6) || 0 == strncmp("-h", args[i], 3) )  {
          print_usage();
          printf("Command format: process_ring -p <2-64> -r <0-100> -i <poll or sync>\n");
          printf("Commands -p (--processes), -r(--rounds), -i are optional.\n");
          printf("Default process: 2 || Default rounds: 3 || Default implementation: poll \n");
          return SHELL_ERROR;

        } else {
          printf("Please use --help or -h for the command format. \n" );
          return SHELL_ERROR;
        }
  	}
  	printf("Number of Processes: %d\n", no_of_process);
  	printf("Number of Rounds: %d\n", no_of_rounds);
  	//printf("Implementation Type: %s\n", implementation_type);

    if (strncmp(implementation_type, "poll",4) == 0) {
      volatile int32 consume = (no_of_process*no_of_rounds)-1;
      printf("\n");
      
      gettime(&start_time);
      for (i = 0; i < no_of_process; i = i+1)
      {
        
        //process in suspended state
        mailbox[i] = create(polling_scheduling, 1024, 20, "polling scheduling", 5, inbox, i, &consume, no_of_process, no_of_rounds);
        
        //Putting it in ready state.
        resume(mailbox[i]);

        
      }
      //Parent process waiting for the child process signal.
      while(consume > -1);
      
      
      printf("Time elapsed: %d\n",  (start_time-gettime(&end_time)));
      
    } else {
      
      int index;
      
      int32 start_count = (no_of_process*no_of_rounds)-1;
      
      pid32 parent_id = getpid();
      
      for (index = 0; index < no_of_process; ++index)
      {
        //Processes in suspended state
        mailbox[index] = create(message_pass, 1024, 20, "message passing", 6, mailbox, index, start_count, no_of_process, no_of_rounds, parent_id);
        
        //Putting processes in the ready state
        resume (mailbox[index]);
      }

      gettime(&start_time);
      //Initiating the message passing via parent process
      send(mailbox[0], start_count);

      //Blocked till the child processes sends the OK message.
      receive();
      printf("Time elapsed: %d\n",  (start_time-gettime(&end_time)));
    }
	
  return SHELL_OK;

}

void print_usage(void){
  printf("Usage: ...\n");
}

