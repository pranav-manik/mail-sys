// Pranav Maniktala
// CMSC 421 Project 1
// skiplist.h
// This will be the implementation of the skiplist

#ifndef SKIPLIST_H
#define SKIPLIST_H

// #include <stdbool.h>
// #include <stdint.h>

// #include <linux/kernel.h>
// #include <linux/syscalls.h>

// structs


// define acl node
typedef struct acl_node {
	int pid;
	struct acl_node *next;
} acl_node;

// define acl list
typedef struct acl_list {
	struct acl_node *head;
	int size;
} acl_list;

//define node of mailbox
 typedef struct mailbox_node {
 	char *data;
 	struct mailbox_node *next;
 	int length;
 } mailbox_node;

 //define mailbox holding nodes
 typedef struct mailbox {
 	struct mailbox_node *head;
 	struct mailbox_node *tail;
 	unsigned int size;
 } mailbox;

// define skiplist node
typedef struct skip_list_node {
	unsigned int key;
	// unsigned int uid;
	struct skip_list_node **fwd;
	struct mailbox *mailbox;
	struct acl_list *acl_list;
} skip_list_node;

// define skiplist
typedef struct skip_list {
	skip_list_node *head;
	int level;
	unsigned int prob;
	int size;
} skip_list;




// end of structs

// skiplist methods
skip_list *init_skiplist(skip_list *sl, unsigned int key, unsigned int prob);

void add_skiplist_node(skip_list *sl, unsigned int key);

int del_skiplist_node(skip_list *sl, unsigned int key);

skip_list_node *search_skiplist(skip_list *sl, unsigned int key);

void free_skiplist(skip_list *sl);

void print_skiplist(skip_list *sl);


// mailbox methods

// initialize mailbox
mailbox *init_mailbox(mailbox *mailbox);

//create node
mailbox_node *create_node(char *data, int length);

//enmailbox node (push)
void enqueue(mailbox *mailbox, char *data, int length);

//demailbox node (pop)
mailbox_node *dequeue(mailbox *mailbox);

void del_mailbox(mailbox *mailbox);

void print_mailbox(mailbox *mailbox);


// acl functions
acl_list *init_acl(acl_list *acl);

void add_acl_pid(acl_list *acl, int pid);

void *remove_acl_pid(acl_list *acl, int pid);

acl_node *find_acl_pid(acl_list *acl, int pid);

void del_acl_list(acl_list *acl_list);

void print_acl(acl_list *acl);

// system calls and functions

// bool isRoot();

int checkProbability(unsigned int prob);

// long mbx421_init(unsigned int ptrs, unsigned int prob);

// long mbx421_shutdown(void);

// long mbx421_create(unsigned long id);

// long mbx421_destroy(unsigned long id);

// long mbx421_count(unsigned long id);

// long mbx421_send(unsigned long id, const unsigned char __user *msg, long len);

// long mbx421_recv(unsigned long id, unsigned char __user *msg, long len);

// long mbx421_length(unsigned long id);

// long mbx421_acl_add(unsigned long id, pid_t process_id);

// long mbx421_acl_remove(unsigned long id, pid_t process_id);


// SYSCALL_DEFINE2(mbx421_init, unsigned int, ptrs, unsigned int, prob);
// SYSCALL_DEFINE0(mbx421_shutdown);
// SYSCALL_DEFINE1(mbx421_create, unsigned long, id);
// SYSCALL_DEFINE1(mbx421_destroy, unsigned long, id);
// SYSCALL_DEFINE1(mbx421_count, unsigned long, id);
// SYSCALL_DEFINE3(mbx421_send, unsigned long, id, const unsigned char __user*, msg, long, len);
// SYSCALL_DEFINE1(mbx421_length, unsigned long, id);



#endif /*SKIPLIST_H*/