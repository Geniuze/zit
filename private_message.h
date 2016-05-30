#ifndef __PRIVATE_MESSAGE_H__
#define __PRIVATE_MESSAGE_H__

#define PRIVATE_MESSAGE_PORT 18003
#define SCANRESULT_PACKAGE_MAXCOUNT 32

#define MIN(a,b) (((a)<(b))?(a):(b))
#define IS2GCHENNEL(channel) \
    ((((channel) >= 1) && ((channel) <= 13))?1:0)

enum PRIVATE_MESSAGE_TYPE_SCANRESULT
{
    PRIVATE_MESSAGE_TYPE_SCANRESULT = 1,
};

struct private_message_t
{
    unsigned int type;
    unsigned int length;
    unsigned char data[0];
};

extern unsigned int scanRadioId;
int epoll_add_privatemessage(int epoll_fd);

#endif
