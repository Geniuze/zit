#include <iostream>

using namespace std;
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <list>
#include <string.h>
#include <syslog.h>

#include "timer.h"

list<timer_list> timers;

int pipefd[2];
void signal_func(int signo)
{
    static int a;
    write(pipefd[1], &a, sizeof(int));
    a ++;
}
void *timeout_func(int fd, int events, void *arg)
{
    if (events & EPOLLIN)
    {
        int a = 0;
        read(fd, &a, sizeof(int));
        for (list<timer_list>::iterator it = timers.begin();
             it != timers.end();
             it ++)
        {
            it->count--;
            if (0 >= it->count)
            {
                cout << "timer " << it->timerId << " timeout " << endl;
                //syslog(LOG_DEBUG, "[%s:%d] timeout ID %d \n", __func__, __LINE__, it->timerId);

                (it->func)(it->arg);
                list<timer_list>::iterator it_e = it++;
                timers.erase(it_e);
                it--;
            }
        }
    }
    return NULL;
}
int epoll_add_timer(int epoll_fd)
{
    int ret = -1;
    ret = socketpair(AF_UNIX, SOCK_STREAM, 0, pipefd);
    if (ret < 0)
    {
        perror("socketpair");
        return -1;
    }

    struct epoll_event ev;
    static struct epoll_callback ec;
    ec.func = timeout_func;
    ec.fd = pipefd[0];
    ev.data.ptr = (void*)&ec;
    ev.events = EPOLLIN;
    cout << "pipefd[0] = " << pipefd[0] << ",pipefd[1] = " << pipefd[1]  << endl;
    ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, pipefd[0], &ev);
    if (ret < 0)
    {
        perror("epoll_ctl");
        return ret;
    }

    struct sigevent sev;
    timer_t timer_id;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGUSR1;
    sev.sigev_value.sival_int = TIMER_ID;
    ret = timer_create(CLOCK_MONOTONIC, &sev, &timer_id);
    if (ret < 0)
    {
        perror("timer_create");
        return ret;
    }

    signal(SIGUSR1,signal_func);

    struct itimerspec its;
    memset(&its, 0, sizeof(its));
    its.it_interval.tv_sec = 1;
    its.it_value.tv_sec = 1;
    ret = timer_settime(timer_id, 0, &its, NULL);
    if (ret < 0)
    {
        perror("timer_settime");
        return ret;
    }
    return ret;
}
int add_timer(int timerId, int sec, timer_func *func, void *arg)
{
    timer_list timer;
    timer.timerId = timerId;
    timer.count = sec/1;
    timer.func = func;
    timer.arg = arg;

    cout << "timer add ID " << timerId << endl;
    //syslog(LOG_DEBUG, "[%s:%d] timer add ID %d \n", __func__, __LINE__, timerId);
    timers.push_back(timer);
    return 0;
}
int stop_timer(int timerId)
{
    for (list<timer_list>::iterator it = timers.begin();
         it != timers.end();
         it ++)
    {
        if (it->timerId == timerId)
        {
            list<timer_list>::iterator it_e = it++;
            cout << "timer delete ID " << it_e->timerId << endl;
            //syslog(LOG_DEBUG, "[%s:%d] timer del ID %d \n", __func__, __LINE__, timerId);
            timers.erase(it_e);
        }
    }
    return 0;
}

