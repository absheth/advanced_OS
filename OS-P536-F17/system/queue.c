/* queue.c - enqueue, dequeue */

#include <xinu.h>
#include <stdbool.h>
#include <future.h>
struct qentry	queuetab[NQENT];	/* Table of process queues	*/
void print_Q(Future_queue *); 

/*------------------------------------------------------------------------
 *  enqueue  -  Insert a process at the tail of a queue
 *------------------------------------------------------------------------
 */
pid32	enqueue(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q		/* ID of queue to use		*/
	)
{
	qid16	tail, prev;		/* Tail & previous node indexes	*/

	if (isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	tail = queuetail(q);
	prev = queuetab[tail].qprev;

	queuetab[pid].qnext  = tail;	/* Insert just before tail node	*/
	queuetab[pid].qprev  = prev;
	queuetab[prev].qnext = pid;
	queuetab[tail].qprev = pid;
	return pid;
}

/*------------------------------------------------------------------------
 *  dequeue  -  Remove and return the first process on a list
 *------------------------------------------------------------------------
 */
pid32	dequeue(
	  qid16		q		/* ID queue to use		*/
	)
{
	pid32	pid;			/* ID of process removed	*/

	if (isbadqid(q)) {
		return SYSERR;
	} else if (isempty(q)) {
		return EMPTY;
	}

	pid = getfirst(q);
	queuetab[pid].qprev = EMPTY;
	queuetab[pid].qnext = EMPTY;
	return pid;
}

/*Generalized queue implemenation (Assignment 5)*/
//-------------------------------------------------------------------------
void en_Q(Future_queue *f_queue, int proc_id) {
	struct F_qnode	*new_node = (struct F_qnode *)getmem(sizeof(struct F_qnode));
	new_node->value = proc_id;
	new_node->next = NULL;
	
	//Means that the queue is null;
	if (f_queue->qlast == NULL) {
		f_queue->qfirst = f_queue->qlast = new_node;
		//printf("Element enqueued --> %d\n", f_queue->qfirst->value);		
		return;
	}

	f_queue->qlast->next = new_node;
	f_queue->qlast = new_node;
	//printf("Element enqueued --> %d\n", f_queue->qlast->value);
	//print_Q(f_queue);
}
//-------------------------------------------------------------------------
pid32 de_Q(Future_queue *f_queue) {
	//print_Q(f_queue);
	struct F_qnode *new_node =  f_queue->qfirst;
	f_queue->qfirst =f_queue->qfirst->next;

	if (f_queue->qfirst == NULL) {
		f_queue->qlast = NULL;
	}

	//printf("Element dequeued --> %d\n", new_node->value);
	//print_Q(f_queue);
	return new_node->value;
}
//-------------------------------------------------------------------------
bool is_queue_empty(Future_queue *f_queue) {
	bool test = true;
	if (f_queue->qfirst == NULL && f_queue->qlast == NULL)
		test = false;
	return test;
}
//-------------------------------------------------------------------------
void print_Q(Future_queue *f_queue) {
	struct F_qnode *current_node = f_queue->qfirst;
	while (current_node != NULL){
		printf("%d ", current_node->value);
		current_node = current_node->next;
	}
	printf("\n");

}

//-------------------------------------------------------------------------
