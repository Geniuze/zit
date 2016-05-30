#include <iostream>
using namespace std;
#include <string>
#include <sys/ioctl.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <net/if.h>

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>


int GetIFHwAddr(string ifname, unsigned char* dst, size_t size)
{
    int ret = -1;
    struct ifreq ifr;
    int sockfd = -1;

    if (ifname.length() >= IFNAMSIZ)
    {
        cout << "device name error : " << ifname << endl;
        syslog(LOG_ERR, "[%s:%d]device name error : %s\n", __func__, __LINE__, ifname.c_str());
        return ret;
    }
    strcpy(ifr.ifr_name, ifname.c_str());

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > sockfd)
    {
        cout << "socket error:" << strerror(errno) << endl;
        syslog(LOG_ERR, "[%s:%d]socket error: %s\n", __func__, __LINE__, strerror(errno));
        return ret;
    }


    if (0 == ioctl(sockfd, SIOCGIFHWADDR, &ifr))
    {
        if (6 <= size)
        {
            memcpy(dst, ifr.ifr_hwaddr.sa_data, 6);
            ret = 0;
        }
    }

    close(sockfd);

    return ret;
}
