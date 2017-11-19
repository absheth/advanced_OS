/* in file xsh_process_ring.c */

process polling_scheduling(volatile int32* inbox, int index, int* startcount, int size_of_inbox, int no_of_rounds);

process message_pass(pid32* mailbox, int index, int startcount, int size_of_mailbox, int no_of_rounds, pid32 parent_id);

extern	volatile int32 inbox[100];
extern	pid32 mailbox[100];