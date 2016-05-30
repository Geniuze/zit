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
#include <map>
#include <vector>
#include <list>
#include <sstream>
#include <syslog.h>

#include "private_message.h"
#include "timer.h"
#include "buffer.h"
#include "kvlist.h"
#include "proto_elements.h"
#include "proto_defs.h"
#include "stringunit.h"
#include "wp_common.h"

unsigned int scanRadioId = 1;

static int udpsockfd;
int udp_listen_privatemessage()
{
    udpsockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > udpsockfd)
    {
        perror("socket");
        return -1;
    }
    struct sockaddr_in server;

    bzero(&server,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=htons(PRIVATE_MESSAGE_PORT);
    server.sin_addr.s_addr= htonl (INADDR_ANY);
    if(bind(udpsockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("Bind()error.");
        return -1;
    }

    return udpsockfd;
}

void * private_message_callback(int fd, int event, void *arg)
{
    if (event & EPOLLIN)
    {
        buffer buf;
        struct private_message_t *pm = NULL;
        int recvlen = 0;
        int datalen = 0;

        recvlen = read(fd, buf.GetHead(), buf.GetSize());
        if (0 > recvlen)
        {
            syslog(LOG_ERR, "[%s:%d] recv private message error\n", __func__, __LINE__);
            return NULL;
        }
        buf.SetLength(recvlen);

        pm = (struct private_message_t*)buf.GetHead();
        datalen = recvlen - 8;

        cout << "recv private message type = " << pm->type << " length = " << recvlen << ":" << pm->length << endl;
        syslog(LOG_DEBUG, "[%s:%d] recv private message type[%d] length[%d] datalength[%d]\n", __func__, __LINE__,
               pm->type, recvlen, pm->length);

        switch(pm->type)
        {
        case PRIVATE_MESSAGE_TYPE_SCANRESULT:
            {
                size_t resultCount = (datalen)/sizeof(message_drive);
                struct message_drive *results = (struct message_drive*)pm->data;
                cout << "resultCount : " << resultCount << endl;
                SetScanResultRequest res;

                while(resultCount > 0)
                {
                    res.loadFromCMPtr((void*)results);

                    if (res.count >= 32)
                    {
                        buffer resbuf;
                        res.assemble(&resbuf);
                        apzit_send_msg(resbuf.GetHead(), resbuf.GetLength());
                        res.count = 0;
                    }

                    results += 1;
                    resultCount -- ;
                }
                if (res.count > 0)
                {
                    buffer resbuf;
                    res.assemble(&resbuf);
                    apzit_send_msg(resbuf.GetHead(), resbuf.GetLength());
                    res.count = 0;
                }

                break;
            }
        default : break;
        }

    }

    return 0;
}

int epoll_add_privatemessage(int epoll_fd)
{
    int ret = -1;
    int fd = udp_listen_privatemessage();
    if (0 > fd)
    {
        return -1;
    }
    struct epoll_event ev;
    static struct epoll_callback ec;
    ec.func = private_message_callback;
    ec.fd = fd;
    ev.data.ptr = (void*)&ec;
    ev.events = EPOLLIN;
    ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
    if (ret < 0)
    {
        perror("epoll_ctl");
        return ret;
    }

    return 0;
}



