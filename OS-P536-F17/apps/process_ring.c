#include <xinu.h>
#include <process_ring.h>
#include <prototypes.h>

//Scheduling by polling
process polling_scheduling(volatile int32 *inbox, int index, int* count,int size_of_inbox, int no_rounds){
  int32 latest; 
  int round_no = 0;
  
  //Setting 0th process inbox to initial countdown number
  if (index == 0)
  {
    inbox[index] = *count;
  }

  while(1) {

    //Loop for the number of rounds.
    while(round_no < no_rounds){
      if(inbox[index]!=-1){
        latest = inbox[index];
        printf("Ring Element: %d Round : %d Value : %d \n", index, round_no, latest);
        
        //Once the last element is consumed, set count < -1 so that parent process will know to stop waiting.
        if(latest == 0){
          *count = -2;
        }

        //Assigning value to next element of the array in sequential manner.
        inbox[(index+1)%size_of_inbox]=latest-1;
        
        inbox[index] = - 1;
        round_no = round_no + 1;
        
      }
    }
    
  }
  //printf("\n");
  return OK;
}

//Scheduling by message passing
process message_pass(pid32 *mailbox, int index, int startcount, int size_of_mailbox, int no_rounds, pid32 parent_pid){
  int32 latest; 
  int round_no = 0;
  
  while (round_no < no_rounds){
    
    latest = receive();
    printf("Ring Element: %d Round : %d Value : %d \n", index, round_no, latest);

    if (latest == 0)
    {
        send(parent_pid, OK);
        return OK;
    }
    send(mailbox[(index+1)%size_of_mailbox], latest-1);
    round_no = round_no + 1;
  }

  return OK;
}
