#ifndef __LP_MESSAGE_H__
#define __LP_MESSAGE_H__

#define LP_MESSAGE_PORT 18002

enum lp_message_type
{
    LP_MESSAGE_STAEVENT = 1,
    LP_MESSAGE_WLANUP = 2,
    LP_MESSAGE_ADDWLAN = 3,
};

struct lpmessge_list
{
    // apcomm发送到ap模块的报文消息
    char *msg;
    char *msg_length;

    // 配置主体
    string cbody;
};

int epoll_add_lpmessage(int epoll_fd);

#endif
