// This file tries calling things out of order as tehy dont work

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <linux/kernel.h>
#include <sys/syscall.h>

#define __NR_MBX421_INIT 434
#define __NR_MBX421_SHUTDOWN 435
#define __NR_MBX421_CREATE 436
#define __NR_MBX421_DESTROY 437
#define __NR_MBX421_COUNT 438
#define __NR_MBX421_SEND 439
#define __NR_MBX421_RECV 440
#define  __NR_MBX421_LENGTH 441
#define  __NR_MBX421_ACL_ADD 442
#define  __NR_MBX421_ACL_REMOVE 443

long init_syscall(unsigned int ptrs, unsigned int prob) {
    return syscall(__NR_MBX421_INIT, ptrs, prob);
}

long shutdown_syscall(void) {
    return syscall(__NR_MBX421_SHUTDOWN);
}

long create_skip_node(unsigned long id) {
    return syscall(__NR_MBX421_CREATE, id);
}

long send_msg(unsigned long id, const unsigned char *msg, long len) {
    return syscall(__NR_MBX421_SEND, id, msg, len);
}

long recv_msg(unsigned long id, const unsigned char *msg, long len) {
    return syscall(__NR_MBX421_RECV, id, msg, len);
}

long count_msg(unsigned long id) {
    return syscall(__NR_MBX421_COUNT, id);
}

long length_msg(unsigned long id) {
    return syscall(__NR_MBX421_LENGTH, id);
}

long add_acl(unsigned long id, pid_t process_id) {
    return syscall(__NR_MBX421_ACL_ADD, id, process_id);
}

long remove_acl(unsigned long id, pid_t process_id) {
    return syscall(__NR_MBX421_ACL_REMOVE, id, process_id);
}



int main(int argc, char *argv[]) {

    long init;

    // initialize mailbox
    init_syscall(16, 2);
    init = init_syscall(16, 4);

    if(init == 0) {
        
        printf("init syscall ran successfully, check dmesg output\n");
    }
    else {
        perror("init syscall failed");
    }


    // try sending messages before creating nodes
    char *msg3 = (char *) malloc((5)*sizeof(char));
    strcpy(msg3, "msg3");
    long send;
    send = send_msg(3, msg3, 5);
    // send_msg(3, "msg2", 5);
    // send_msg(3, "msg3", 5);

    if(send == 0) {
        
        printf("send syscall ran successfully, check dmesg output\n");
    }
    else {
        perror("send syscall failed");
    }

    long create;

    create = create_skip_node(3);

    if(create == 0) {
        
        printf("create syscall ran successfully, check dmesg output\n");
    }
    else {
        printf("create syscall failed, recieved: %s\n", create);
    }
    char *msg4 = (char *) malloc((4)*sizeof(char));
    strcpy(msg4, "msg4");
    send_msg(4, msg4, 4);


    // gets count of the specified mailbox queue
    long count;
    count = count_msg(3);

    if(count >= 0) {
        
        printf("count syscall ran successfully, count for id 3: %ld\n", count);
    }
    else {
        perror("count syscall failed");
    }


    // gets length of the first message
    long length;
    length = length_msg(3);

    if(length >= 0) {
        
        printf("length syscall ran successfully, length for id 3: %u\n", length);
    }
    else {
        perror("length syscall failed");
    }


    char *msg10 = (char *) malloc((5)*sizeof(char));
    // msg10 = "null";

    // recieve message
    long recv;
    recv = recv_msg(3, msg10, 5);

    if(recv == 0) {
        
        printf("recv syscall ran successfully, check dmesg output %s\n", msg10);
    }
    else {
        perror("recv syscall failed");
    }

    count = count_msg(3);

    if(count >= 0) {
        
        printf("count syscall ran successfully, count for id 3: %ld\n", count);
    }
    else {
        perror("count syscall failed");
    }

    // shutdown mailbox system skiplist
    long shtdwn;
    shtdwn = shutdown_syscall();

    if(shtdwn == 0) {
        
        printf("shtdwn syscall ran successfully, check dmesg output\n");
    }
    else {
        perror("shtdwn syscall failed");
    }


    return 0;
}
