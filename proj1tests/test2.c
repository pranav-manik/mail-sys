// This test attempts to run functions without initializeing anything

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


    // send message
    char *msg = (char *) malloc((4)*sizeof(char));
    strcpy(msg, "msg1");
    char *msg2 = (char *) malloc((4)*sizeof(char));
    strcpy(msg2, "msg2");
    char *msg3 = (char *) malloc((4)*sizeof(char));
    strcpy(msg3, "msg3");
    long send;
    send = send_msg(3, msg, 4);
    // send_msg(3, "msg2", 4);
    // send_msg(3, "msg3", 4);

    if(send == 0) {
        
        printf("send syscall ran successfully, check dmesg output\n");
    }
    else {
        perror("send syscall failed");
    }

    // gets count of the specified mailbox queue
    long count;
    count = count_msg(3);

    if(count >= 0) {
        
        printf("count syscall ran successfully, count: %ld\n", count);
    }
    else {
        perror("count syscall failed");
    }


    // gets length of the first message
    long length;
    length = length_msg(3);

    if(length >= 0) {
        
        printf("length syscall ran successfully, length: %u\n", length);
    }
    else {
        perror("length syscall failed");
    }

    // shutdown system call
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