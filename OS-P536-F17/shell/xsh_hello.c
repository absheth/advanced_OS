/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <process_ring.h>
/*------------------------------------------------------------------------
 * xsh_hello - Take input and print the hello message.
 *------------------------------------------------------------------------
 */

void test();
void test1();
shellcmd xsh_hello(int nargs, char *args[]) {
	
	int a;
    
    /* Printing 'Hello <string>, Welcome to the world of Xinu!!' */
	if(nargs==1){
		printf("No input from command line.\n");
	}

	printf("Hello");

	//Formatting	
	if(nargs!=1){
		printf(" ");
	}
	
	for( a = 1; a < nargs; a = a + 1 ){
      printf("%s", args[a]);
      if ((a+1)!=nargs){
      	printf(" ");
      }
	}
	printf(", Welcome to the world of Xinu!!\n");
	printf("\n");
	
	//ASSIGNMENT 4 - QUESTION 3 
	pid32 p1;
	int prio1;
	p1 = create(test, 1024, 30, "process1", 1, 100);

	//Create a new process.
	prio1 = resume(p1);

	printf("Main: P1 Priority --> %d \n", prio1);
	
    printf("This is xsh_hello.\n");
    //ASSIGNMENT 4 - QUESTION 3 
	return 0;
}

void test(int x) {
	printf("Value from main: \n", x);
	int prio2;
	int self_pid;
	self_pid = getpid();
	//Create a new process and pass the pid whose priority is to be changed.
	prio2 = resume(create(test1, 1024, 40, "from method test", 1, self_pid));
	printf("test :: P2 Priority --> %d\n", prio2);
}

void test1(pid32 p1) {
	
	int oldprio;
	//Change priority of process P1 and capture the old priority returned by chprio.
	oldprio = chprio(p1, 22);

	printf("test1 :: Old priority from chprio --> %d\n", oldprio);
	
}
