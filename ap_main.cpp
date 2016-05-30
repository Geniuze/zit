#include <iostream>
using namespace std;

#include <sys/epoll.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include <string>
#include <syslog.h>
#include <stdlib.h>
#include "timer.h"
#include "buffer.h"
#include "stringunit.h"
#include "kvlist.h"
#include "proto_elements.h"
#include "proto_defs.h"
#include "lp_message.h"
#include "private_message.h"

#define EPOLL_MAX 256
#define EPOLL_EVENTS_COUNT 20

static int make_socket_non_blocking(int sfd)
{
    int flags, s;
    flags = fcntl(sfd, F_GETFL,0);
    if(flags == -1)
    {
        perror("fcntl");
        return -1;
    }

    flags|= O_NONBLOCK;
    s =fcntl(sfd, F_SETFL, flags);
    if(s ==-1)
    {
        perror("fcntl");
        return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    int epoll_fd = epoll_create(EPOLL_MAX);
    if (epoll_fd < 0)
    {
        perror("epoll_create");
        return -1;
    }
    struct epoll_event events[20];

    epoll_add_timer(epoll_fd);
    epoll_add_udp_server(epoll_fd);
    epoll_add_lpmessage(epoll_fd);
    epoll_add_privatemessage(epoll_fd);

    openlog((const char*)"apzit", LOG_NDELAY | LOG_PID, 0);

    //begin echo
    add_timer(ECHO_TIMERID, FIRST_ECHO_RELAY, echo_request_send, NULL);
    add_timer(MONITOR_TIMERID, FIRST_ECHO_RELAY, monitor_info, NULL);

    //system("whitedomain add 51awifi.com 0");
    //system("iwpriv wfm cache_portal 1");
    while (true)
    {
        int nfds = epoll_wait(epoll_fd, events, EPOLL_EVENTS_COUNT, -1);
        for (int i=0; i<nfds; ++i)
        {
            struct epoll_callback *pec = (struct epoll_callback*)events[i].data.ptr;
            pec->func(pec->fd, events[i].events, pec);
        }
    }
    closelog();
    return 0;
}

