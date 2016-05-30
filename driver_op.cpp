#include <iostream>
using namespace std;

#include <vector>
#include <string>

#include <sys/ioctl.h>
#include "string.h"
#include "cint.h"
// #include "driver_defs.h"
#include "stringunit.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>

#if 0

int get_stainfo(const char *ifname, struct ieee80211req_sta_stats *stats, unsigned char *macaddr)
{
	int	s;
	struct iwreq iwr;

	memset(&iwr, 0, sizeof(iwr));
	memset(stats, 0, sizeof(struct ieee80211req_sta_stats));

	s = socket(AF_INET, SOCK_DGRAM, 0);

	if (s < 0)
		return -1;

	strncpy(iwr.ifr_name, ifname, sizeof(iwr.ifr_name));
	iwr.u.data.pointer = (void *) stats;
	iwr.u.data.length = sizeof(struct ieee80211req_sta_stats);
	memcpy(stats->is_u.macaddr, macaddr, IEEE80211_ADDR_LEN);

	if (ioctl(s, IEEE80211_IOCTL_STA_STATS, &iwr) < 0)
	{
		close(s);
		return -2;
	}

	close(s);

	return 0;
}


string GetifnameByStation(unsigned char *macaddr)
{
    for (int i = 0;i<16; i++)
    {
        string ifname = "wlan";
        if (i > 7)
        {
            ifname  = "awlan2";
        }

        string value = "";
        int2string(i>>3, value);
        ifname += value;
        cout << "ifname = " << ifname << endl;
        struct ieee80211req_sta_stats stat;
        int ret = get_stainfo(ifname.c_str(), &stat, macaddr);
        if (0 == ret)
        {
            cout << "find station ifname " << ifname << endl;
            return ifname;
        }
    }
    return "";
}
#endif

