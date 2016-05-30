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
#include <stdlib.h>

#include "lp_message.h"
#include "timer.h"
#include "buffer.h"
#include "kvlist.h"
#include "proto_elements.h"
#include "proto_defs.h"
#include "stringunit.h"

static int udpsockfd;
extern list<ZCtrlMsg*> wait_list;
int udp_listen_lpmessage()
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
    server.sin_port=htons(LP_MESSAGE_PORT);
    server.sin_addr.s_addr= htonl (INADDR_ANY);
    if(bind(udpsockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("Bind()error.");
        return -1;
    }

    return udpsockfd;
}

void * lpmessage_callback(int fd, int event, void *arg)
{
    if (event & EPOLLIN)
    {
        buffer *buf = new buffer;
        do {
            string value;
            int len = read(fd, buf->GetHead(), buf->GetSize());
            buf->SetLength(len);

            if (0 >= len)
            {
                break;
            }
            syslog(LOG_DEBUG, "[%s:%d] recv %d bytes \n%s\n", __func__, __LINE__,
                   len, buf->GetHead());
            cout << "local message "<< len <<" bytes : " << endl;
            cout << buf->GetHead() << endl;

            string localmessage = "";
            localmessage.assign(buf->GetHead(), buf->GetLength());

            kvlist kv;
            kv.GetKvFromStr(localmessage);

            value = kv.getValue("msgtype");
            if ("" != value)
            {
                unsigned int msgtype;
                string2int(value, msgtype);
                cout << "msgtype = " << msgtype << endl;

                switch(msgtype)
                {
                case LP_MESSAGE_STAEVENT:
                    {
                        {
                            StaEventInfoRequest *staevent = new StaEventInfoRequest;
                            buffer *buf = new buffer;

                            staevent->qostlv.setValid(true);
                            staevent->msgtypetlv.setValid(true);
                            staevent->wlanidtlv.setValid(true);
                            staevent->radioidtlv.setValid(true);

                            staevent->loadFromCM(kv);
                            staevent->assemble(buf);

                            apzit_send_msg(buf->GetHead(), buf->GetLength());

                            delete buf;
                            delete staevent;
                        }

                        {
                            StaAssosicInfoRequest req;
                            buffer buf;

                            req.macaddrtlv.setValid(true);
                            req.ipaddrtlv.setValid(true);

                            req.loadFromCM(kv);
                            if (0 != *(int*)req.ipaddrtlv.ipaddr)
                            {
                                req.assemble(&buf);
                                apzit_send_msg(buf.GetHead(), buf.GetLength());
                            }
                        }

#if 0
                        {
                            // set init station state
                            string sWlanId = kv.getValue("wlanid");
                            string sRadioId = kv.getValue("radioid");
                            string mac = MacNoColo2Colo(kv.getValue("stamac"));

                            unsigned int WlanId = 0;
                            unsigned int RadioId = 0;
                            string2int(sWlanId, WlanId);
                            string2int(sRadioId, RadioId);
                            string ifname = get_wlan_ifname(RadioId, WlanId);

                            string cmd = "/usr/sbin/wconf portal " + ifname + " " + mac + " 0";
                            system(cmd.c_str());
                            cout << "init station state unauth : " << cmd << endl;
                            syslog(LOG_DEBUG, "[%s:%d] init station state unauth [%s]\n", __func__, __LINE__,
                                   cmd.c_str());

                        }
#endif
                        break;
                    }
                case LP_MESSAGE_WLANUP:
                    {
#if 0
                        KeyEventsRequest *req = new KeyEventsRequest;
                        buffer *reqbuf = new buffer;

                        req->wlanup.setValid(true);

                        req->loadFromCM(kv);
                        req->assemble(reqbuf);

                        apzit_send_msg(reqbuf->GetHead(), reqbuf->GetLength());

                        delete reqbuf;
                        delete req;
#else

                        AddWlanResponse *rsp = new AddWlanResponse;
                        buffer *rspbuf = new buffer;

                        rsp->loadFromCM(kv);
                        rsp->assemble(rspbuf);
                        apzit_send_msg(rspbuf->GetHead(), rspbuf->GetLength());

                        delete rspbuf;
                        delete rsp;
#endif

                        // 此处不需要break。addwlan时，同时需要上报wlan up事件
                    }
                case LP_MESSAGE_ADDWLAN:
                    {
                        AddWlanResponse *rsp = new AddWlanResponse;
                        buffer *rspbuf = new buffer;

                        list<ZCtrlMsg*>::iterator it = wait_list.begin();
                        AddWlanRequest *req = NULL;

                        rsp->loadFromCM(kv);
                        for (; it != wait_list.end(); it++)
                        {
                            req = (AddWlanRequest*)(*it);
                            if (req->WlanId == rsp->results[0].wlanId &&
                                req->RadioId == rsp->results[0].radioId)
                            {
                                break;
                            }
                        }
                        if (it != wait_list.end())
                        {
                            rsp->assemble(rspbuf);
                            apzit_send_msg(rspbuf->GetHead(), rspbuf->GetLength());

                            wait_list.erase(it);
                            delete req;
                        }

                        delete rspbuf;
                        delete rsp;
                        break;
                    }

                default:break;
                }
            }

        }while (0);
        delete buf;
    }
    //读消息回应队列，处理回应报文
    return 0;
}

int epoll_add_lpmessage(int epoll_fd)
{
    int ret = -1;
    int fd = udp_listen_lpmessage();
    if (0 > fd)
    {
        return -1;
    }
    struct epoll_event ev;
    static struct epoll_callback ec;
    ec.func = lpmessage_callback;
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




