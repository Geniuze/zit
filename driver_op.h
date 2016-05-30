#ifndef __DRIVER_OP_H__
#define __DRIVER_OP_H__


// int get_stainfo(const char *ifname, struct ieee80211req_sta_stats *stats, unsigned char *macaddr);
string GetifnameByStation(unsigned char *macaddr);

#endif
