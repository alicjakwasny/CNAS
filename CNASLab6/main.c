#include <iostream>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
struct my_message
                    
{
    int val1;
    float val2;
    double val3;
    char txt[100];
};
int main(int argc, char **argv)
                    
{
    // man mq_overview
    // -lrt
    // gcc -Wall code.c -o task3 -lrt
    // man 3 mq_open
    // O_RDONLY or O_WRONLY or O_RDWR
    // O_CLOEXEC
    // O_CREAT
    // O_EXCL
    // O_NOBLOCK
    int oflag = O_RDWR | O_CREAT;
    struct mq_attr attrs;
    attrs.mq_flags = 0;
    attrs.mq_maxmsg = 8;
    attrs.mq_msgsize = sizeof( struct my_message ); //   100;
    attrs.mq_curmsgs = 0;
    //mq_unlink("/myqueue");
    mqd_t qfd = mq_open("/myqueue", oflag, 0777, &attrs);
    if (qfd == (mqd_t) -1)
    {
        perror ("mq_open: ");
        abort();
    }
    struct my_message m1, m2;
    m1.val1 = 10;
    m1.val2 = 3.1415;
    int rv = mq_send (qfd, (const char *)&m1, sizeof(m1), 0);
    // int rv = mq_send (qfd, reinterpret_cast<const char *>(&m1), sizeof(m1), 0);
    if (rv == -1)
    {
        perror ("mq_send: ");
        abort();
    }
    char buf[100];
    rv = mq_receive(qfd, (char *)&m2, sizeof(struct my_message), NULL);
    if (rv == -1)
    {
        perror ("mq_receive: ");
        abort();
    }
    std::cout << "We got " << rv << " bytes" << std::endl;
    std::cout << m2.val1 << " " << m2.val2 << std::endl;
    // buf[rv] = 0; // UB
    // std::cout << buf << std::endl;
    std::cout << "END" << std::endl;
    return 0;
}
// int     mq_send   (mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio);
// ssize_t mq_receive(mqd_t mqdes, char *msg_ptr,       size_t msg_len, unsigned int *msg_prio);
//struct mq_attr {
//    long mq_flags;       /* Flags (ignored for mq_open()) */
//    long mq_maxmsg;      /* Max. # of messages on queue */
//    long mq_msgsize;     /* Max. message size (bytes) */
//    long mq_curmsgs;     /* # of messages currently in queue
//                            (ignored for mq_open()) */
//};
// 0753
// 111 101 011
// $ ls -la /etc/passwd
//  - rw- r-- r-- 1 root root 3925 sty  5 17:21 /etc/passwd
//    110 100 100
//SYNOPSIS
//       #include <fcntl.h>           /* For O_* constants */
//       #include <sys/stat.h>        /* For mode constants */
//       #include <mqueue.h>
//
//       mqd_t mq_open(const char *name, int oflag);
//       mqd_t mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr);
//
//       Link with -lrt.
void foo()
                    
{
    int val = 5;
    float * p;
    p = reinterpret_cast<float *>(& val);
    *p = 10;
}