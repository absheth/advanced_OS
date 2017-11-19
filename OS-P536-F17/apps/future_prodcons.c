#include <xinu.h>
#include <future.h>
#include <stdio.h>

uint future_prod(future_t* fut,int n) {
	int status;    
   printf("Produced %d\n",n);
	status = future_set(fut, n);	
	
	if (status < 1) {
      printf("future_set failed for value %d \n", n);
      return SYSERR;
   } 
  	
   
   
   return OK;
}

uint future_cons(future_t* fut) {
	//printf("future_cons :: start :: PID --> %d \n", getpid());
	//printf("future_cons :: start :: MODE --> %d \n", fut->mode);
	//printf("future_cons :: start :: STATE --> %d \n", fut->state);
   int i, status;
   status = (int)future_get(fut, &i);
   if (status < 1) {
   	printf("future_get failed\n");
     	return SYSERR;
   }
    
      
   printf("Consumed %d\n", i);
   
   
   //switch(fut->mode) {
   //     	case FUTURE_EXCLUSIVE :
   //     	break;
   //   case FUTURE_SHARED :
   //   	if(fut->get_queue->qlast == NULL) {
   // 			//status = future_free(fut);
   // 			//printf("Status after freeing the memory: %d \n", status);
   //		}
   //     		
   //     		break;
   //   case FUTURE_QUEUE :
   //   	//if(fut->set_queue->qlast == NULL && fut->get_queue->qlast == NULL) {
   // 			//status = future_free(fut);
   // 			//printf("Status after freeing the memory: %d \n", status);
   //		//}
   //      break;
   //}
 	
   return OK;
}
