/* xsh_prodcons.c - xsh_prodcons */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <future.h>
/*------------------------------------------------------------------------
 * xsh_prodcons - Take input and print the prodcons message.
 *------------------------------------------------------------------------
 */


shellcmd xsh_prodcons(int nargs, char *args[]) {
	
	if(nargs==1){
		printf("No input from command line.\n");
		return SHELL_ERROR;
	}
	if(nargs > 2){
		printf("Invalid number of parameters.\n");
		return SHELL_ERROR;
	}	
	if (0 != strncmp("-f", args[1], 3) ) {
		printf("Invalid flag. \n");
		return SHELL_ERROR;
	} 
	int x;
	future_t	*f_exclusive, *f_shared, *f_queue;
	
  	f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
	f_shared    = future_alloc(FUTURE_SHARED);
  	f_queue     = future_alloc(FUTURE_QUEUE);

  	// Test FUTURE_EXCLUSIVE
  	printf("EXCLUSIVE\n");
  	resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive) );
	resume( create(future_prod, 1024, 20, "fprod1", 2, f_exclusive, 2) );	
  	resume( create(future_prod, 1024, 20, "fprod1", 2, f_exclusive, 1) );
		
	sleepms(10);
	
	printf("\n");	
	
  	// Test FUTURE_SHARED
	printf("SHARED\n");  	
	resume( create(future_prod, 1024, 20, "fprod2", 2, f_shared, 2) );
  	resume( create(future_cons, 1024, 20, "fcons2", 1, f_shared) );
  	resume( create(future_cons, 1024, 20, "fcons3", 1, f_shared) );
	resume( create(future_prod, 1024, 20, "fprod2", 2, f_shared, 3) );  	
  	resume( create(future_cons, 1024, 20, "fcons4", 1, f_shared) ); 
  	resume( create(future_cons, 1024, 20, "fcons5", 1, f_shared) );
  	
	x = future_free(f_shared);
	sleepms(10);
	printf("\n");	
  	// Test FUTURE_QUEUE
	
	printf("FUTURE_QUEUE\n");	
	resume( create(future_prod, 1024, 20, "fprod5", 2, f_queue, 5) );	
	resume( create(future_prod, 1024, 20, "fprod3", 2, f_queue, 3) );
  	resume( create(future_cons, 1024, 20, "fcons6", 1, f_queue) );
	resume( create(future_prod, 1024, 20, "fprod6", 2, f_queue, 6) );	
	resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
	
	resume( create(future_cons, 1024, 20, "fcons8", 1, f_queue) );
		
	resume( create(future_cons, 1024, 20, "fcons9", 1, f_queue) );
	resume( create(future_prod, 1024, 20, "fprod4", 2, f_queue, 4) );  	
  	sleepms(10);
	future_free(f_queue);
	sleepms(50);	
	return 0;
}
