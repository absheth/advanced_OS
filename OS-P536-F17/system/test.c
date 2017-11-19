/*#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
struct F_qnode{
  int value;
  struct F_qnode *next;
} ;

typedef struct {
  struct F_qnode *qfirst;
  struct F_qnode *qlast;
} Future_queue;


//-------------------------------------------------------------------------
void en_Q(Future_queue *f_queue, int proc_id) {
	struct F_qnode	*new_node = (struct F_qnode *)malloc(sizeof(struct F_qnode));
	new_node->value = proc_id;
	new_node->next = NULL;
	
	//Means that the queue is null;
	if (f_queue->qlast == NULL) {
		f_queue->qfirst = f_queue->qlast = new_node;
		printf("Element enqueued --> %d\n", new_node->value);		
		return;
	}

	f_queue->qlast->next = new_node;
	f_queue->qlast = new_node;
	printf("Element enqueued --> %d\n", new_node->value);
	
}
//-------------------------------------------------------------------------
int de_Q(Future_queue *f_queue) {
	if (f_queue->qfirst == NULL){
		return -1;
	}

	struct F_qnode *new_node =  f_queue->qfirst;
	f_queue->qfirst =f_queue->qfirst->next;

	if (f_queue->qfirst == NULL) {
		f_queue->qlast = NULL;
	}

	printf("Element dequeued --> %d\n", new_node->value);
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

int main() {
	Future_queue *q = (Future_queue*)malloc(sizeof(Future_queue));
	//q->qfirst = null;
	//q->qlast = null;
	
	en_Q(q, 1);
	print_Q(q);
	en_Q(q, 2);
	en_Q(q, 3);
	en_Q(q, 4);
	en_Q(q, 5);
	print_Q(q);
	de_Q(q);
	print_Q(q);
	en_Q(q, 2);
	print_Q(q);
	de_Q(q);
	print_Q(q);	
	en_Q(q, 4);
	print_Q(q);	
	en_Q(q, 5);
	print_Q(q);
	de_Q(q);
	print_Q(q);
	
}




*/










