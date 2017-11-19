#ifndef _FUTURE_H_
#define _FUTURE_H_  


/* Future States Constants*/
typedef enum {
  FUTURE_EMPTY,
  FUTURE_WAITING,
  FUTURE_READY
} future_state_t;


/* Future Mode Constants */
typedef enum {
  FUTURE_EXCLUSIVE,
  FUTURE_SHARED,
  FUTURE_QUEUE
} future_mode_t;



struct F_qnode{
  pid32 value;
  struct F_qnode *next;
} ;

typedef struct {
  struct F_qnode *qfirst;
  struct F_qnode *qlast;
} Future_queue;


typedef struct {
  int value;
  future_state_t state;
  future_mode_t mode;
  Future_queue *set_queue;
  Future_queue *get_queue;
  pid32 pid;

} future_t;

/* Interface for the Futures system calls */
future_t* future_alloc(future_mode_t mode);
syscall future_free(future_t*);
syscall future_get(future_t*, int*);
syscall future_set(future_t*, int);

uint future_prod(future_t* ,int);
uint future_cons(future_t*);
 
#endif /* _FUTURE_H_ */