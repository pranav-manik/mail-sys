// Pranav Maniktala
// CMSC 421 Project 1
// skiplist.c
// This will be the implementation of the skiplist (in case name wasnt clear)

#include "skiplist.h"


// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>
// #include <stdbool.h>
// #include <math.h>

#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <linux/cred.h>
#include <linux/errno.h>
#include <linux/mutex.h>

skip_list *my_skiplist;

static unsigned int MAX_LEVEL = 16;
static unsigned int next_random = 9001;

unsigned int probability;

unsigned int lock_state = 0;
struct mutex lock;

static unsigned int generate_random_int(void) {
    next_random = next_random * 1103515245 + 12345;
    return (next_random / 65536) % 32768;
}

static void seed_random(unsigned int seed) {
    next_random = seed;
}

unsigned int random_level(void) {

    unsigned int level = 1;

    // keep flipping coin till 0 or reaches Max level
    while ((generate_random_int() < 32767 / probability) && (level < MAX_LEVEL)) {
        level++;
    }

    return level;
}

// System Calls

// bool isRoot(){
//     return true;
//     // if (getuid() != 0){
//     //     printf("you is not a root!\n");
//     //     return false;
//     // } else {
//     //     printf("you is a root!\n");
//     //     return true;
//     // }

// }

int checkProbability(unsigned int prob){
    int probabilities[5] = {2, 4, 6, 8, 16};

    int i;
    for (i=0; i < 5; i++) {
        if (probabilities[i] == prob) {
            probability = prob;
            return 0;
        }
    }
    return 1;
}



SYSCALL_DEFINE2(mbx421_init, unsigned int, ptrs, unsigned int, prob) {
// long mbx421_init(unsigned int ptrs, unsigned int prob) {
    MAX_LEVEL = ptrs;
    // uid_t uid=getuid();
    // get_current_user_id()
    // euid=geteuid();
    // if (uid<0 || uid!=euid) {
    //     // initialize
    // }
    // else {
    //     return -EPERM;
    // }
    
    if (my_skiplist) {
        return -EEXIST;
        // printf("%s\n", "-EEXIST");
        // return 1;
    }

    // check if root
    if (!current_uid().val) {
        
        // check probabilities
        if (checkProbability(prob) == 0) {

            my_skiplist = kmalloc(sizeof(skip_list), GFP_KERNEL);

            if (my_skiplist) {

                // initializes and unlocks lock
                if (lock_state == false) {
                    mutex_init(&lock);
                    lock_state = 1;
                }

                // initialize skiplist 
                mutex_lock(&lock);
                init_skiplist(my_skiplist, 1, prob);
                mutex_unlock(&lock);

                return 0;

            }

            else {
                return -ENODEV;
                // printf("%s\n", "-ENODEV");
                // return 1;
            }
        }

        else {
            return -EINVAL;
            // printf("%s\n", "-EINVAL");
            // return 1;
        }
    }

    else {
        return -EPERM;
        // printf("%s\n", "-EPERM");
        // return 1;
    }
}

SYSCALL_DEFINE0(mbx421_shutdown) {
// long mbx421_shutdown(void) {
    // checks if root
    if (!current_uid().val) {
        
        // checks if skiplist there
        if (my_skiplist) {

            free_skiplist(my_skiplist);
            my_skiplist = NULL;
            return 0;

        }

        else {
            return -ENODEV;
        }

    }

    else {
        return -EPERM;
    }

}

SYSCALL_DEFINE1(mbx421_create, unsigned long, id) {
// long mbx421_create(unsigned long id) {

    if (!current_uid().val) {

        // check if skip_list initialized
        if (my_skiplist) {

            // check if valid id
            if (id == 0 || id >= ULONG_MAX ) {
                return -EPERM;
                // printf("%s\n", "-EPERM");
                // return 1;
            }

            // check if there isnt a skiplist with that id
            if (!search_skiplist(my_skiplist, id)) {

                mutex_lock(&lock);
                add_skiplist_node(my_skiplist, id);
                mutex_unlock(&lock);

                return 0;
            }

            else {
                return -EEXIST;
                // printf("%s\n", "-EEXIST");
                // return 1;
            }

        }

        else {
            return -ENODEV;
            // printf("%s\n", "-ENODEV");
            // return 1;
        }
    }

    else {
        return -EPERM;
        // printf("%s\n", "-EPERM");
        // return 1;
    }

}

SYSCALL_DEFINE1(mbx421_destroy, unsigned long, id) {
// long mbx421_destroy(unsigned long id) {
// 
    // check if root running
    if (!(current_uid().val)) {

        // check if skip_list initialized
        if (my_skiplist) {

            mutex_lock(&lock);
            if (del_skiplist_node(my_skiplist, id) == 1) {
                mutex_unlock(&lock);
                return 0;
            }
            else {
                mutex_unlock(&lock);
                return -EPERM;
            }

        }

        else {
            return -ENODEV;
        }
    }

    else {
        return -EPERM;
    }

}

SYSCALL_DEFINE1(mbx421_count, unsigned long, id) {
// long mbx421_count(unsigned long id) {

    if (my_skiplist) {
        
        // finds node based on id
        skip_list_node *curr_mailbox;
        curr_mailbox = search_skiplist(my_skiplist, id);

        // skiplist node found
        if (curr_mailbox != NULL) {


            return curr_mailbox->mailbox->size;

        }

        else {
            return -ENOENT;
        }

    }
    
    else {
        return -ENODEV;
    }

}

SYSCALL_DEFINE1(mbx421_length, unsigned long, id) {
// long mbx421_length(unsigned long id) {

    if (my_skiplist) {
    
        skip_list_node *curr_mailbox;
        curr_mailbox = search_skiplist(my_skiplist, id);
        
        // if mailbox was found
        if (curr_mailbox != NULL) {

            // if has message
            if (curr_mailbox->mailbox->head) {
                return curr_mailbox->mailbox->head->length;
            }
            // // finds length of data
            // unsigned int length = 0;
            // while (curr_mailbox->mailbox->head->data[length] != '\0')
            // {
            //     length++;
            // }
            // return length;

        }

        // mailbox not found
        else {
            return -ENOENT;
            // printf("%s\n", "-ENOENT");
            // return 1;
        }

    }

    else {
        return -ENODEV;
        // printf("%s\n", "-ENODEV");
        // return 1;
    }
}

SYSCALL_DEFINE3(mbx421_send, unsigned long, id, const unsigned char __user *, msg, long, len) {
    // if the skiplist is there
    if (my_skiplist) {

        skip_list_node *curr_mailbox;
        curr_mailbox = search_skiplist(my_skiplist, id);

        // if queue is found
        if (curr_mailbox != NULL) {

            // // checks if user has priviledges 
            // if (find_acl_pid(curr_mailbox->acl_list, (int) current_uid().val) != 0) {
            //     return -EPERM;
            // }

            // if message handed
            if (msg) {

                mutex_lock(&lock);

                unsigned int end_char;
                end_char = len;
                unsigned char *buff = kmalloc(sizeof(unsigned char) * len, GFP_KERNEL);

                // checks if buffer initialized
                if (buff) {

                    // copies from user to kernel space
                    if (copy_from_user(buff, msg, len) != 0) {
                        kfree(buff);
                        mutex_unlock(&lock);
                        return -EFAULT;
                    }
                    // copy_from_user(buff, msg, len);

                    // queues message into skiplist
                    enqueue(curr_mailbox->mailbox, buff, len);
                    mutex_unlock(&lock);

                    return 0;

                }
                else {
                    return -ENOMEM;
                }

            }

            else {
                return -EFAULT;
            }

        }

        else {
            return -ENOENT;
        }

    }

    else {
        return -ENODEV;
    }

}



SYSCALL_DEFINE3(mbx421_recv, unsigned long, id, const unsigned char __user *, msg, long, len) {
    
    // if the skiplist is there
    if (my_skiplist) {

        skip_list_node *curr_mailbox;
        curr_mailbox = search_skiplist(my_skiplist, id);

        // if queue is found
        if (curr_mailbox) {

            // // checks if user has priviledges 
            // if (find_acl_pid(curr_mailbox->acl_list, (int) current_uid().val) != 0) {
            //     return -EPERM;
            // }

            // if message is valid ptr
            if (msg) {

                // checks if buffer initialized
                if (curr_mailbox->mailbox->size > 0) {

                    mutex_lock(&lock);
                    unsigned char *buff = kmalloc(sizeof(unsigned char) * len, GFP_KERNEL);
                    buff = curr_mailbox->mailbox->head->data;
                    // copies from user to kernel space
                    // if (copy_to_user(buff, msg, len)) {
                    //     int bytes = copy_from_user(buff, msg, len);
                    //     len = len - bytes;
                    // }


                    // copies from kernel to user space
                    if (copy_to_user(msg, buff, len) != 0) {
                        mutex_unlock(&lock);
                        return -EFAULT;
                    }
                    // copy_to_user(msg, buff, len);

                    // queues message into skiplist
                    kfree(buff);
                    kfree(dequeue(curr_mailbox->mailbox));
                    // kfree(curr_mailbox->mailbox);

                    mutex_unlock(&lock);

                    return 0;

                }
                else {
                    return -ENOMEM;
                }

            }

            else {
                return -EFAULT;
            }

        }

        else {
            return -ENOENT;
        }

    }

    else {
        return -ENODEV;
    }

}

// system calls 

SYSCALL_DEFINE2(mbx421_acl_add, unsigned long, id, pid_t, process_id) {
    // // check if root
    // if (current_uid().val != 0) {
    //     return -EPERM;
    // }

    // // check if skiplist initialized
    // if (!my_skiplist) {
    //     return -ENODEV;
    // }
    // mutex_lock(&lock);
    // skip_list_node *curr_mailbox;
    // curr_mailbox = search_skiplist(my_skiplist, id);

    // // check if skiplist node not found
    // if (!curr_mailbox) {
    //     mutex_unlock(&lock);
    //     return -ENOENT;
    // }

    // // checks if valid id
    // if (!id) {
    //     mutex_unlock(&lock);
    //     return -EFAULT;
    // }

    // // if pid exists
    // if (find_acl_pid(curr_mailbox->acl_list, process_id) == 0) {
    //     mutex_unlock(&lock);
    //     return -EEXIST;
    // }

    // add_acl_pid(curr_mailbox->acl_list, process_id);
    // mutex_unlock(&lock);
    return 0;

}

SYSCALL_DEFINE2(mbx421_acl_remove, unsigned long, id, pid_t, process_id) {

    // // check if root
    // if (current_uid().val != 0) {
    //     return -EPERM;
    // }

    // // check if skiplist initialized
    // if (!my_skiplist) {
    //     return -ENODEV;
    // }

    // mutex_lock(&lock);
    // skip_list_node *curr_mailbox;
    // curr_mailbox = search_skiplist(my_skiplist, id);

    // // see if pid already exists

    // // check if skiplist node not found
    // if (!curr_mailbox) {
    //     mutex_unlock(&lock);
    //     return -ENOENT;
    // }

    // // checks if valid id
    // if (!id) {
    //     mutex_unlock(&lock);
    //     return -EFAULT;
    // }
    // // if pid exists
    // if (find_acl_pid(curr_mailbox->acl_list, process_id) != 0) {
    //     mutex_unlock(&lock);
    //     return -ENOENT;
    // }

    // remove_acl_pid(curr_mailbox->acl_list, id);
    // mutex_unlock(&lock);

    return 0;
}

// End of System Calls

skip_list *init_skiplist(skip_list *sl, unsigned int key, unsigned int prob) {
    
    struct skip_list_node *head = kmalloc(sizeof(struct skip_list_node), GFP_KERNEL);
    sl->head = head;
    
    head->key = -1;
    head->fwd = kmalloc(((MAX_LEVEL + 1) * sizeof(struct skip_list_node*)), GFP_KERNEL);
    
    int i;
    for (i = 0; i <= MAX_LEVEL; i++) {
        head->fwd[i] = sl->head;
    }

    sl->level = 1;
    sl->size = 0;

    return sl;

}

// insert node into skiplist
void add_skiplist_node(skip_list *sl, unsigned int key) {

    mailbox *my_mailbox;
    my_mailbox = kmalloc(sizeof(mailbox), GFP_KERNEL);
    init_mailbox(my_mailbox);

    acl_list *my_acl;
    my_acl = kmalloc(sizeof(acl_list), GFP_KERNEL);
    init_acl(my_acl);

    skip_list_node *update[MAX_LEVEL + 1];
    skip_list_node *curr;
    curr = sl->head;

    // traverse through skiplist until key handed is greater than 
    int i;
    for (i = sl->level; i >= 1; i--) {
        while (curr->fwd[i]->key < key) {
            curr = curr->fwd[i];
        }
        // update level
        update[i] = curr;
    }
    
    // next node must be where inserted
    curr = curr->fwd[1];

    // insert node into skiplist if there is a matching key
    if (key == curr->key) {
        curr->mailbox = my_mailbox;
        curr->acl_list = my_acl;
    }

    //creates key if not found
    else {
        // finds out max level for that node
        int level;
        level = random_level();

        // keeps setting highest level as the head
        if (level > sl->level) {
            for (i = sl->level + 1; i <= level; i++) {
                update[i] = sl->head;
            }
            sl->level = level;
        }

        // initialize node data
        curr = kmalloc(sizeof(skip_list_node), GFP_KERNEL);
        curr->key = key;
        curr->mailbox = my_mailbox;
        curr->acl_list = my_acl;
        curr->fwd = kmalloc(sizeof(skip_list_node *) * (level + 1), GFP_KERNEL);

        for (i = 1; i <= level; i++) {
            curr->fwd[i] = update[i]->fwd[i];
            update[i]->fwd[i] = curr;
        }

    }
}

// deletes skiplist node
int del_skiplist_node(skip_list *sl, unsigned int key) {

    skip_list_node *update[MAX_LEVEL + 1];
    skip_list_node *curr;
    curr = sl->head;

    // traverse through skiplist until key handed is greater than 
    int i;
    for (i = sl->level; i >= 1; i--) {
        while (curr->fwd[i]->key < key) {
            curr = curr->fwd[i];
        }
        // update level
        update[i] = curr;
    }
 
    // fwd key must be key handed
    curr = curr->fwd[1]; 

    // verify key matches and free
    if (curr->key == key) {

        // move nodes to head level
        for (i = 1; i <= sl->level; i++) {
            if (update[i]->fwd[i] != curr) {
                break;
            }
            update[i]->fwd[1] = curr->fwd[i];
        }

        // frees node
        if (curr) {
            kfree(curr->fwd);
            curr->fwd = NULL;
            del_mailbox(curr->mailbox);
            del_acl_list(curr->acl_list);
            kfree(curr);
            curr = NULL;
        }

        // move down levels til head points to level 1
        while (sl->level > 1 && sl->head->fwd[sl->level] == sl->head) {
            sl->level--;
        }

        // successfully removed
        return 0;
    }

    // return if not removed
    return 1;

}


// search skiplist based on mailbox_node given
skip_list_node *search_skiplist(skip_list *sl, unsigned int key) {

    skip_list_node *curr = sl->head;


    // traverse skiplist
    int i;
    for (i = sl->level; i >= 1; i--) {
        while (curr->fwd[i]->key < key) {
            curr = curr->fwd[i];
        }
    }
    
    // return if key found
    if (curr->fwd[1]->key == key) {
        return curr->fwd[1];
    }

    // key not found
    return NULL;

}

void free_skiplist(skip_list *sl) {

    skip_list_node *curr;
    curr = sl->head->fwd[1];

    while (curr != sl->head) {

        skip_list_node *temp = curr->fwd[1];

        del_mailbox(curr->mailbox);
        del_acl_list(curr->acl_list);

        kfree(curr->fwd);
        curr->fwd = NULL;
        kfree(curr);
        curr = temp;

    }

    kfree(curr->fwd);
    curr->fwd = NULL;
    kfree(curr);
    curr = NULL;
    kfree(sl);
    sl = NULL;

}

void print_skiplist(skip_list *sl) {

    skip_list_node *curr = sl->head;
    while (curr && curr->fwd[1] != sl->head) {
        printk("skip %d->", curr->fwd[1]->key);
        print_mailbox(curr->fwd[1]->mailbox);
        curr = curr->fwd[1];
    }
    printk("NIL\n");

}


// mailbox methods 

// initialize mailbox
mailbox *init_mailbox(mailbox *mailbox) {

    mailbox->head = NULL;
    mailbox->tail = NULL;
    mailbox->size = 0;

    return mailbox;
}


//create node
mailbox_node *create_node(char *data, int length) {

    struct mailbox_node *temp;
    temp = kmalloc(sizeof(mailbox), GFP_KERNEL);
    temp->data = data;
    temp->length = length;
    temp->next = NULL;

     return temp;

}

//retrieve end of mailbox
mailbox_node *end_node(mailbox *mailbox) {

    return mailbox->tail;

}

//enqueue mailbox_node (push)
void enqueue(mailbox *mailbox, char *data, int length) {

    mailbox_node *temp;
    temp = create_node(data, length);
    // printf("data: %c\n", temp->data);
    // if mailbox is empty
    if (mailbox->head == NULL) {
        mailbox->head = temp;
        mailbox->tail = temp;
        mailbox->tail->next = NULL;
    }

    // if mailbox has nodes
    else {
        mailbox->tail->next = temp;
        temp->next = NULL;
        mailbox->tail = mailbox->tail->next;
    }

    mailbox->size++;

}

//enqueue mailbox_node (pop)
mailbox_node *dequeue(mailbox *mailbox) {

    // checks if mailbox is empty
    if (mailbox->head == NULL) {
        return NULL;
    }

    //if mailbox not empty
    else {

        mailbox_node *front;
        front = mailbox->head;
        mailbox->head = mailbox->head->next;

        // checks if head is empty again, and make tail same as head if is
        if (mailbox->head == NULL) {
            mailbox->tail = mailbox->head;
        }

        // free(front);
        mailbox->size--;
        return front;
    }

}

void del_mailbox(mailbox *mailbox) {

    // while (mailbox->size > 0) {
    //     dequeue(mailbox);
    // }

    mailbox_node *temp;
    temp = mailbox->head;

    // traverse through the queue until all nodes are freed
    while (temp) {

        mailbox->head = mailbox->head->next;

        if (!mailbox->head) {
            mailbox->tail = NULL;
        }

        kfree(temp->data);
        temp->data = NULL;
        kfree(temp);
        temp = mailbox->head;


    }

    kfree(mailbox);
    mailbox = NULL;

}

void print_mailbox(mailbox *mailbox) {

    if (mailbox->head == NULL) {
        printk("NULL\n");
    }

    else {
        mailbox_node *temp;
        temp = mailbox->head;

        while (temp != NULL) {
            // printf("called");
            printk("->%s", temp->data);
            temp = temp->next;
        }
        printk("\n");
    }
}

// acl functions

// initialize acl list
acl_list *init_acl(acl_list *acl) {
    acl->head = NULL;
    acl->size = 0;

    return acl;
}

// adds acl pid
void add_acl_pid(acl_list *acl, int pid) {

    // create acl_node
    struct acl_node *temp = kmalloc(sizeof(struct acl_node), GFP_KERNEL);
    temp->pid = pid;
    temp->next = NULL;

    // iterate and add to end of list
    acl_node *curr_node;
    curr_node = acl->head;
    while (curr_node != NULL && curr_node->next != NULL) {
        curr_node = curr_node->next;
    }

    if (curr_node != NULL) {
        curr_node->next = temp;
    }
    else {
        acl->head = temp;
    }
    acl->size++;
};

// removes acl node based on pid handed
void *remove_acl_pid(acl_list *acl, int pid) {

    struct acl_node *curr_node;
    struct acl_node *prev_node;
    curr_node = acl->head;

    // if head is pid
    if (curr_node != NULL && curr_node->pid == pid) 
    { 
        acl->head = curr_node->next; 
        kfree(curr_node); 
    }


    // iterate til pid found
    while (curr_node != NULL || curr_node->pid != pid) {
        prev_node = curr_node;
        curr_node = curr_node->next;
    }

    // skips linking node with pid
    if (curr_node->pid == pid) {
        prev_node->next = curr_node->next;
        kfree(curr_node);
        acl->size--;
    }
}

// finds acl node by pid, return 0 if found 1 if not found
acl_node *find_acl_pid(acl_list *acl, int pid) {

    struct acl_node *curr_node;
    curr_node = acl->head;

    // iterate til pid found
    while (curr_node != NULL || curr_node->pid != pid) {
        curr_node = curr_node->next;
    }

    // skips linking node with pid
    if (curr_node->pid == pid) {
        return 0;
    }
    return 1;
}

void del_acl_list(acl_list *acl_list) {

    acl_node *temp;
    temp = acl_list->head;

    // traverse through the queue until all nodes are freed
    while (temp) {

        acl_list->head = acl_list->head->next;

        kfree(temp->pid);
        temp->pid = NULL;
        kfree(temp);
        temp = acl_list->head;


    }

    kfree(acl_list);
    acl_list = NULL;

}

void print_acl(acl_list *acl) {
    printk("%d\n", acl->head->pid);
    printk("%d\n", acl->size);
}

// int main(void) {

//     mbx421_init(16, 2);

//     mbx421_create(3);
//     mbx421_create(2);
//     mbx421_create(1);
//     mbx421_create(6);
//     mbx421_create(9);
    

//     print_skiplist(my_skiplist);
    
//     mbx421_shutdown();


    // skip_list *sl;
    // sl = malloc(sizeof(skip_list));
    // init_skiplist(sl, 1, 2, true);
    

    // add_skiplist_node(sl, 3);
    // add_skiplist_node(sl, 4);
    // add_skiplist_node(sl, 5);
    // // add_skiplist_node(sl, 5);
    // add_skiplist_node(sl, 6);
    // skip_list_node *mbx;
    // mbx = search_skiplist(sl, 5);
    // char *msg = (char *)malloc((4)*sizeof(char));
    // strcpy(msg, "msg1");

    // char *msg2 = (char *)malloc((4)*sizeof(char));
    // strcpy(msg2, "msg2");
    
    // // free(msg);
    // // printf("%s\n", msg);
    // enqueue(mbx->mailbox, msg);
    // enqueue(mbx->mailbox, msg2);
    // free(dequeue(mbx->mailbox));
    // free(dequeue(mbx->mailbox));
    // // free(dequeue(mbx->mailbox));

    // // enqueue(mbx->mailbox, "msg2");
    // // del_skiplist_node(sl, 1);
    // // del_skiplist_node(sl, 3);
    // // del_skiplist_node(sl, 4);
    // // del_skiplist_node(sl, 5);
    // // del_skiplist_node(sl, 6);
    // free(msg);
    // free(msg2);
    // print_skiplist(sl);
    // free_skiplist(sl);
//     return 0;
// }