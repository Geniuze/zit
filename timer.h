#ifndef __TIMER_H__
#define __TIMER_H__

#define TIMER_ID 100

typedef void *EpollCBFunc(int fd, int events, void *arg);
typedef int timer_func(void *arg);

struct epoll_callback
{
    int fd;
    int timeout;
    EpollCBFunc *func;
};

struct timer_list
{
    int timerId;
    int count;
    void *arg;
    timer_func *func;
};

int epoll_add_timer(int epoll_fd);
int add_timer(int timerId, int sec, timer_func *func, void *arg);
int stop_timer(int timerId);

#endif
