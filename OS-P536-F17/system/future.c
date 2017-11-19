#include <xinu.h>
#include <future.h>
#include <stdio.h>


int de_Q(Future_queue*); 
void en_Q(Future_queue*, pid32);
void print_Q(Future_queue*);
//Allocate memmory to the future.
future_t* future_alloc(future_mode_t mode) {
	future_t *future_element;
        future_element	=	(future_t *)getmem(sizeof(future_t));
	switch(mode) {
		case FUTURE_EXCLUSIVE :
        	future_element->mode = mode;
			future_element->state = FUTURE_EMPTY;
			break;
      case FUTURE_SHARED :
	future_element->get_queue = (Future_queue *)getmem(sizeof(Future_queue));
	future_element->mode = mode;
			future_element->state = FUTURE_EMPTY;
			future_element->get_queue->qfirst = NULL;
			future_element->get_queue->qlast = NULL;
         break;
      case FUTURE_QUEUE :
	future_element->set_queue = (Future_queue *)getmem(sizeof(Future_queue));
	future_element->get_queue = (Future_queue *)getmem(sizeof(Future_queue));
	
      	future_element->mode = mode;
			future_element->state = FUTURE_EMPTY;
			
			future_element->set_queue->qfirst = NULL;
			future_element->set_queue->qlast = NULL;
			
			future_element->get_queue->qfirst = NULL;
			future_element->get_queue->qlast = NULL;
			//future_element->get_queue = NULL;
         break;
    }

	


	return future_element;
}

//Freeing the future
syscall future_free(future_t *future_element) {
	
	int status;

	if ((status = freemem((char *)future_element, sizeof(future_t))) != OK )
	{
		return SYSERR;
	}
	return OK;
}

//Getting the future value
syscall future_get(future_t *future_element , int* value) {
	int status;
	   	
	switch(future_element->mode) {
      case FUTURE_EXCLUSIVE :
		/* This will wait till the state of the future is changed to Ready.*/	
        	while (future_element->state!=FUTURE_READY) {
				printf("");
				continue;
			}
	
			*value = future_element->value;
	
			if ((status=future_free(future_element)) != OK) {	
				return SYSERR;
			}

			break;
      case FUTURE_SHARED :
      /* The consumer will check if the state of the future is ready. It will consume if and only if the state of the future is ready.*/
      	if (future_element->state!=FUTURE_READY)
      	{	
      		pid32 cons_proc_id = getpid();
      		en_Q(future_element->get_queue, cons_proc_id);
      		suspend(cons_proc_id);
      	} 
			*value = future_element->value;
			break;
      case FUTURE_QUEUE :
		/* The consumer will check if there are any producers waiting to produce the value.*/
			if(future_element->set_queue->qlast != NULL) {
         	resume(de_Q(future_element->set_queue));
         	*value = future_element->value;
         } else {
				future_element->state = FUTURE_WAITING;         	
         	pid32 cons_proc_id = getpid();
         	en_Q(future_element->get_queue, cons_proc_id);
         	
         	suspend(cons_proc_id);
         	*value = future_element->value;
        	}
        	break;
   }


	return OK;

}

/*Setting the future value*/
syscall future_set(future_t *future_element, pid32 value) {
	
	switch(future_element->mode) {
      	case FUTURE_EXCLUSIVE :
		/* The producer will produce the value if and only if the state of the future is EMPTY.*/
        	if (future_element->state == FUTURE_EMPTY) {
				future_element->value = value;
				future_element->state = FUTURE_READY;
			} else {
				printf("Only one thread can produce in Exclusive mode. ");
				return SYSERR;
			}
			break;
      	case FUTURE_SHARED :
	
		/* The producer will produce the value if and only if the state of the future is EMPTY.*/
      		if (future_element->state == FUTURE_EMPTY) {
					future_element->value = value;
					future_element->state = FUTURE_READY;
					
				} else { 
					printf("Only one thread can produce in Shared Mode. ");
					return SYSERR;
				}
				while(future_element->get_queue->qlast != NULL) {
					resume(de_Q(future_element->get_queue));		
				}
				
				break;
      	case FUTURE_QUEUE :
		/* The producer will check of there are any consumers waiting to consume the value.*/	
         	if(future_element->get_queue->qlast != NULL) {
         		future_element->value = value;
					future_element->state = FUTURE_READY;
					resume(de_Q(future_element->get_queue));
         	} else {
         		pid32 prod_proc_id = getpid();
         		en_Q(future_element->set_queue, prod_proc_id);
         		suspend(prod_proc_id);
					future_element->value = value;
					future_element->state = FUTURE_READY;
         	}
         	break;
    }


	
	return OK;
}
