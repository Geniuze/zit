#include <iostream>
using namespace std;

#include <string>
#include <vector>
#include <list>
#include <map>
#include <string.h>
#include <arpa/inet.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include "stringunit.h"
#include "kvlist.h"
#include "buffer.h"
#include "proto_elements.h"
#include "platform.h"
#include "driver_op.h"

string get_wlan_ifname(unsigned char RadioId, unsigned char WlanId)
{
    string ifname = (RadioId == 1)?"wlan":"awlan2";
    string strwlanid = "";
    int2string((int)(WlanId-1), strwlanid);
    ifname += strwlanid;
    return ifname;
}

int Element::parse(buffer *buf)
{
    return 0;
}
int Element::assemble(buffer *buf)
{
    return 0;
}
int Element::saveToCM(string &sMsg)
{
    return 0;
}
int Element::loadFromCM(kvlist &kv)
{
    return 0;
}
int Element::parseEH(buffer *buf)
{
    unsigned short _type,_length;
    buf->retrieve16(_type);
    buf->retrieve16(_length);

    if (_type != type)
    {
        buf->BackOffset(4);
        return -1;
    }

    length = _length;
    valid = true;
    return 0;
}
int Element::skipheader(buffer *buf)
{
    buf->Skip(4);
    return 0;
}
int Element::assembleHeader(buffer *buf)
{
    buf->BackOffset(length + 4);
    buf->store16(type);
    buf->store16(length);
    buf->Skip(length);
    return 0;
}

int image_protocoltlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;
    buf->retrieve8(proto);
    return 0;
}
int image_protocoltlv::assemble(buffer *buf)
{
    return 0;
}
int image_protocoltlv::saveToCM(string &sMsg)
{
    if(2 == proto)
    {
        sMsg += "modify /config.ini/loadconf/TRANS_PRO=1;";
    }
    else if (3 == proto )
    {
        sMsg += "modify /config.ini/loadconf/TRANS_PRO=0;";
    }
    else
    {
    }

    return 0;
}
int image_protocoltlv::loadFromCM(kvlist &kv)
{
    return 0;
}
int image_serveripv4tlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieveRawBytes(ipaddr, 4);
    return 0;
}
int image_serveripv4tlv::assemble(buffer *buf)
{
    return 0;
}
int image_serveripv4tlv::saveToCM(string &sMsg)
{
    char ipaddripv4[4];
    sprintf(ipaddripv4,"%u.%u.%u.%u",ipaddr[0],ipaddr[1],ipaddr[2],ipaddr[3]);
    sMsg += string("modify /config.ini/loadconf/SERVER_ADDR=") + ipaddripv4 + ";";

    return 0;
}
int image_serveripv4tlv::loadFromCM(kvlist &kv)
{
    return 0;
}
int image_urltlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    url.assign(buf->GetPtr());
    buf->retrieveRawBytes(NULL, length);
    return 0;
}
int image_urltlv::assemble(buffer *buf)
{
    return 0;
}
int image_urltlv::saveToCM(string &sMsg)
{
    sMsg += "modify /config.ini/loadconf/FILE_NAME=" + url + ";";
    return 0;
}
int image_urltlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int image_usernametlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    username.assign(buf->GetPtr());
    buf->retrieveRawBytes(NULL, length);
    return 0;
}
int image_usernametlv::assemble(buffer *buf)
{
    return 0;
}
int image_usernametlv::saveToCM(string &sMsg)
{
    sMsg += "modify /config.ini/loadconf/USER_NAME=" + username + ";";
    return 0;
}
int image_usernametlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int image_passwdtlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    passwd.assign(buf->GetPtr());
    buf->retrieveRawBytes(NULL, length);
    return 0;
}
int image_passwdtlv::assemble(buffer *buf)
{
    return 0;
}
int image_passwdtlv::saveToCM(string &sMsg)
{
    sMsg += "modify /config.ini/loadconf/PASSWORD=" + passwd + ";";
    return 0;
}
int image_passwdtlv::loadFromCM(kvlist &kv)
{
    return 0;
}


int stainfo_qostlv::parse(buffer *buf)
{
    return 0;
}
int stainfo_qostlv::assemble(buffer *buf)
{
    skipheader(buf);

    buf->store8(qos);
    length += 1;

    assembleHeader(buf);
    return 0;
}
int stainfo_qostlv::saveToCM(string &sMsg)
{
    return 0;
}
int stainfo_qostlv::loadFromCM(kvlist &kv)
{
    qos = 1;
    return 0;
}


int stainfo_msgtypetlv::parse(buffer *buf)
{
    return 0;
}
int stainfo_msgtypetlv::assemble(buffer *buf)
{
    skipheader(buf);

    buf->store8(msgtype);
    length += 1;

    assembleHeader(buf);
    return 0;
}
int stainfo_msgtypetlv::saveToCM(string &sMsg)
{
    return 0;
}
int stainfo_msgtypetlv::loadFromCM(kvlist &kv)
{
    unsigned int iValue;
    string value = kv.getValue("relationType");
    string2int(value, iValue);
    msgtype = (unsigned char)iValue;
    return 0;
}

int stainfo_wlanidtlv::parse(buffer *buf)
{
    return 0;
}
int stainfo_wlanidtlv::assemble(buffer *buf)
{
    skipheader(buf);

    buf->store8(WlanId);
    length += 1;

    assembleHeader(buf);
    return 0;
}
int stainfo_wlanidtlv::saveToCM(string &sMsg)
{
    return 0;
}
int stainfo_wlanidtlv::loadFromCM(kvlist &kv)
{
    unsigned int iValue;
    string value = kv.getValue("wlanid");
    string2int(value, iValue);
    WlanId = (unsigned char) iValue;
    return 0;
}

int stainfo_radioidtlv::parse(buffer *buf)
{
    return 0;
}
int stainfo_radioidtlv::assemble(buffer *buf)
{
    skipheader(buf);

    buf->store8(RadioId);
    length += 1;

    assembleHeader(buf);
    return 0;
}
int stainfo_radioidtlv::saveToCM(string &sMsg)
{
    return 0;
}
int stainfo_radioidtlv::loadFromCM(kvlist &kv)
{
    unsigned int iValue = 0;
    string value = kv.getValue("radioid");
    string2int(value, iValue);
    RadioId = (unsigned char)iValue;
    return 0;
}

int stainfo_leavereasontlv::parse(buffer *buf)
{
    return 0;
}
int stainfo_leavereasontlv::assemble(buffer *buf)
{
    skipheader(buf);

    buf->store16(reason);
    length += 2;

    assembleHeader(buf);
    return 0;
}
int stainfo_leavereasontlv::saveToCM(string &sMsg)
{
    return 0;
}
int stainfo_leavereasontlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int stastatus_addstation::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve8(WlanId);
    buf->retrieve16(AddType);
    buf->retrieveRawBytes(UserMac, 6);

    return 0;
}
int stastatus_addstation::assemble(buffer *buf)
{
    return 0;
}
int stastatus_addstation::saveToCM(string &sMsg)
{
    return 0;
}
int stastatus_addstation::loadFromCM(kvlist &kv)
{
    return 0;
}

int stastatus_delstation::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve8(WlanId);
    buf->retrieve16(DelCode);
    buf->retrieveRawBytes(UserMac, 6);

    return 0;
}
int stastatus_delstation::assemble(buffer *buf)
{
    return 0;
}
int stastatus_delstation::saveToCM(string &sMsg)
{
    string ifname = get_wlan_ifname(RadioId, WlanId);
    char staMac[32] = {0};
    snprintf(staMac, sizeof(staMac), "%02x:%02x:%02x:%02x:%02x:%02x",
             UserMac[0], UserMac[1], UserMac[2], UserMac[3], UserMac[4], UserMac[5]);
    string delcmd = string("/sbin/iwpriv ") + ifname + " kickmac " + staMac;
    system(delcmd.c_str());

    syslog(LOG_DEBUG, "[%s:%d] system : %s\n", __func__, __LINE__, delcmd.c_str());
    return 0;
}
int stastatus_delstation::loadFromCM(kvlist &kv)
{
    return 0;
}

int stastatus_vlanupdate::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieveRawBytes(UserMac, 6);
    buf->retrieve16(vlanTag);

    return 0;
}
int stastatus_vlanupdate::assemble(buffer *buf)
{
    return 0;
}
int stastatus_vlanupdate::saveToCM(string &sMsg)
{
    return 0;
}
int stastatus_vlanupdate::loadFromCM(kvlist &kv)
{
    return 0;
}

int stastatus_statidupdate::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(u.action);
    buf->retrieve8(slotid); //å½“å‰ç‰ˆæœ¬ä¸éœ€è¦ï¼ŒåŽç»­å¯èƒ½éœ?è¦æ”¯æŒ?
    buf->retrieveRawBytes(UserMac, 6);
    buf->retrieve32(stasid);

    return 0;
}
int stastatus_statidupdate::assemble(buffer *buf)
{
    return 0;
}
int stastatus_statidupdate::saveToCM(string &sMsg)
{
    char staMac[32] = {0};
    string strStaSid = "";
    int2string(stasid, strStaSid);
    snprintf(staMac, sizeof(staMac), "%02x:%02x:%02x:%02x:%02x:%02x",
             UserMac[0], UserMac[1], UserMac[2], UserMac[3], UserMac[4], UserMac[5]);
    cout << "action : " << (int)u.s.act << endl;
    cout << "staMac : " << staMac << endl;
    cout << "stasid : " << stasid << endl;

    switch(u.s.act)
    {
    case 0:
    case 2:
        {
            string tconfCmd = string("/usr/sbin/tconf staid ") + staMac + " " + strStaSid;
            cout << "AC command update staid : " << tconfCmd << endl;
            system(tconfCmd.c_str());
            syslog(LOG_DEBUG, "[%s:%d] system : %s\n", __func__, __LINE__, tconfCmd.c_str());
            break;
        }
    case 1:
        {
            string tconfCmd = string("/usr/sbin/tconf del ") + staMac;
            cout << "AC command delete staid : " << tconfCmd << endl;
            system(tconfCmd.c_str());
            syslog(LOG_DEBUG, "[%s:%d] system : %s\n", __func__, __LINE__, tconfCmd.c_str());
            break;
        }
    default:break;
    }
    return 0;
}
int stastatus_statidupdate::loadFromCM(kvlist &kv)
{
    return 0;
}

int stastatus_useragent_stastat::parse(buffer *buf)
{
    if (0 != parseEH(buf))
    {
        return 0;
    }

    buf->retrieve8(value);
    printf("[type:%d]value = %d\n", type, value);
    return 0;
}
int stastatus_useragent_stastat::assemble(buffer *buf)
{
    return 0;
}
int stastatus_useragent_stastat::saveToCM(string &sMsg)
{
    return 0;
}
int stastatus_useragent_stastat::loadFromCM(kvlist &kv)
{
    return 0;
}

int stastatus_useragent_useracl::parse(buffer *buf)
{
    if (0 != parseEH(buf))
    {
        return 0;
    }

    buf->retrieve16(aclGroupId);
    printf("[type:%d]aclGroupId = %d\n", type, aclGroupId);
    return 0;
}
int stastatus_useragent_useracl::assemble(buffer *buf)
{
    return 0;
}
int stastatus_useragent_useracl::saveToCM(string &sMsg)
{
    return 0;
}
int stastatus_useragent_useracl::loadFromCM(kvlist &kv)
{
    return 0;
}

int stastatus_useragent_usertimestamp::parse(buffer *buf)
{
    if (0 != parseEH(buf))
    {
        return 0;
    }

    buf->retrieve32(TimeStamp);
    printf("[type:%d]TimeStamp = %d\n", type, TimeStamp);
    return 0;
}
int stastatus_useragent_usertimestamp::assemble(buffer *buf)
{
    return 0;
}
int stastatus_useragent_usertimestamp::saveToCM(string &sMsg)
{
    return 0;
}
int stastatus_useragent_usertimestamp::loadFromCM(kvlist &kv)
{
    return 0;
}


int stastatus_useragent::parse(buffer *buf)
{
    if (0 != parseEH(buf))
    {
        return 0;
    }

    int parseLength = 0;

    buf->retrieveRawBytes(UserMac, 6);
    parseLength += 6;
    buf->retrieve8(RadioId);
    parseLength += 1;
    buf->retrieve8(WlanId);
    parseLength += 1;

    printf("[type:%d] mac %02x:%02x:%02x:%02x:%02x:%02x\n", type,
           UserMac[0], UserMac[1], UserMac[2], UserMac[3], UserMac[4], UserMac[5]);


    while (parseLength < length)
    {
        int begin = buf->GetOffset();
        int end = begin;

        stastate.parse(buf);
        useracl.parse(buf);
        timerstamp.parse(buf);

        end = buf->GetOffset();
        if (begin == end)
        {
            skiptlv(buf);
        }
        parseLength += buf->GetOffset() - begin;
    }
    return 0;
}
int stastatus_useragent::assemble(buffer *buf)
{
    return 0;
}
int stastatus_useragent::saveToCM(string &sMsg)
{
    string ifname = get_wlan_ifname(RadioId, WlanId);
    if (stastate.isValid())
    {
        char strmac[64] = {0};
        snprintf(strmac, sizeof(strmac), "%02x:%02x:%02x:%02x:%02x:%02x",
                 UserMac[0], UserMac[1], UserMac[2], UserMac[3], UserMac[4], UserMac[5]);

        string strvalue = "";
        int2string(stastate.value, strvalue);
        string cmd = "/usr/sbin/wconf portal " + ifname + " " + strmac + " " + strvalue;
        system(cmd.c_str());
        cout << "set station state : " << cmd << endl;
        syslog(LOG_DEBUG, "[%s:%d] system : %s\n", __func__, __LINE__, cmd.c_str());
    }
    return 0;
}
int stastatus_useragent::loadFromCM(kvlist &kv)
{
    return 0;
}







int RadioModelSettlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve32(RadioModel);

    return 0;
}
int RadioModelSettlv::assemble(buffer *buf)
{
    return 0;
}
int RadioModelSettlv::saveToCM(string &sMsg)
{
    cout << "ModelSet : " << RadioModel << endl;
    string wifi_device = get_wifi_device(RadioId);

    sMsg += string("modify /config.ini/") + wifi_device + "/WMODE=";
    switch(RadioModel)
    {
    case 0x01: //11b
        sMsg += "0";
        break;
    case 0x05: //11bg
        sMsg += "1";
        break;
    case 0x15: //bgn
        sMsg += "2";
        break;
    case 0x14: //gn
        sMsg += "3";
        break;
    case 0x10: //n
        sMsg += "4";
        break;
    case 0x02: //a
        sMsg += "5";
        break;
    case 0x0a: //an
        sMsg += "6";
        break;
    case 0x08: //n
        sMsg += "7";
        break;
    case 0x20: //ac
    case 0x22: //ac + a
    case 0x28: //ac + n
    case 0x2a: //ac + a + n
        sMsg += "8";
        break;

    default:
        if (1 == RadioId)
            sMsg += "2";
        else
            sMsg += "6";
        break;
    }
    sMsg += ";";

    return 0;
}
int RadioModelSettlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int RadioPowertlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve8(PowerType);
    buf->retrieve16(Power);

    return 0;
}
int RadioPowertlv::assemble(buffer *buf)
{
    return 0;
}
int RadioPowertlv::saveToCM(string &sMsg)
{
    string wifi_device = get_wifi_device(RadioId);

    cout << "PowerType = " << (int)PowerType << endl;
    cout << "Power = " << (int)Power << endl;

    if (1 == PowerType)
    {
        // 3/4 --- 2000*3/4
        switch(Power)
        {
        case 2000:
            sMsg += string("modify /config.ini/") + wifi_device + "/TXPOWER=23;";
            break;
        case 1500:
            sMsg += string("modify /config.ini/") + wifi_device + "/TXPOWER=20;";
            break;
        case 1000:
            sMsg += string("modify /config.ini/") + wifi_device + "/TXPOWER=17;";
            break;
        case 500:
            sMsg += string("modify /config.ini/") + wifi_device + "/TXPOWER=11;";
            break;
        case 250:
            sMsg += string("modify /config.ini/") + wifi_device + "/TXPOWER=5;";
            break;
        case 0:
            sMsg += string("modify /config.ini/") + wifi_device + "/TXPOWER=1;";
            break;

        default:
            sMsg += string("modify /config.ini/") + wifi_device + "/TXPOWER=10;";
            break;
        }
    }
    if (5 == PowerType)
    {
        string strPower = "";
        int2string(Power, strPower);
        sMsg += string("modify /config.ini/") + wifi_device + "/TXPOWER=" + strPower + ";";
    }
    if (4 == PowerType)
    {
        string value = "";
        unsigned int wifi_enable = Power?0:1;
        int2string(wifi_enable, value);

        sMsg += string("modify /config.ini/") + wifi_device + "/RF_ENABLE=" + value + ";";
    }
    return 0;
}
int RadioPowertlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int RadioChanneltlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve16(Channel);

    return 0;
}
int RadioChanneltlv::assemble(buffer *buf)
{
    return 0;
}
int RadioChanneltlv::saveToCM(string &sMsg)
{
    string wifi_device = get_wifi_device(RadioId);

    string strChannel = "";
    int2string(Channel, strChannel);
    sMsg += string("modify /config.ini/") + wifi_device + "/CHANNEL=" + strChannel + ";";

    return 0;
}
int RadioChanneltlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int RadioChainMasktlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve8(ChainMask);

    return 0;
}
int RadioChainMasktlv::assemble(buffer *buf)
{
    return 0;
}
int RadioChainMasktlv::saveToCM(string &sMsg)
{
    string wifi_device = get_wifi_device(RadioId);

    string value = "";
    int2string(ChainMask, value);
    sMsg += string("modify /config.ini/") + wifi_device + "/SPACE_RATE_11N=";
    switch(ChainMask)
    {
    case 1:
        sMsg += "1";
        break;
    case 2:
        sMsg += "3";
        break;
    case 3:
        sMsg += "7";
        break;
    default:
        sMsg += "3";
    }

    sMsg += ";";
    return 0;
}
int RadioChainMasktlv::loadFromCM(kvlist &kv)
{
    return 0;
}


int RadioBandWidthtlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve8(BandWidth);

    return 0;
}
int RadioBandWidthtlv::assemble(buffer *buf)
{
    return 0;
}
int RadioBandWidthtlv::saveToCM(string &sMsg)
{
    string wifi_device = get_wifi_device(RadioId);

    string strBandWidth = "";
    int2string(BandWidth, strBandWidth);

    sMsg += string("modify /config.ini/") + wifi_device + "/CHANNEL_BANDWIDTH_11N=" + strBandWidth + ";";
    return 0;
}
int RadioBandWidthtlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int RadioShortGItlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve8(ShortGI);

    return 0;
}
int RadioShortGItlv::assemble(buffer *buf)
{
    return 0;
}
int RadioShortGItlv::saveToCM(string &sMsg)
{
    string wifi_device = get_wifi_device(RadioId);

    string strShortGI = "";
    int2string(ShortGI, strShortGI);

    sMsg += string("modify /config.ini/") + wifi_device + "/GUARD_INTERVAL_11N=" + strShortGI + ";";
    return 0;
}
int RadioShortGItlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int RadioAmsdutlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve8(AMSDUEnable);

    return 0;
}
int RadioAmsdutlv::assemble(buffer *buf)
{
    return 0;
}
int RadioAmsdutlv::saveToCM(string &sMsg)
{
    string wifi_device = get_wifi_device(RadioId);

    string strAMSDUEnable = "";
    int2string(AMSDUEnable, strAMSDUEnable);

    sMsg += string("modify /config.ini/") + wifi_device + "/A_MSDU_11N=" + strAMSDUEnable + ";";
    return 0;
}
int RadioAmsdutlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int RadioAmpdutlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve8(AMPDUEnable);

    return 0;
}
int RadioAmpdutlv::assemble(buffer *buf)
{
    return 0;
}
int RadioAmpdutlv::saveToCM(string &sMsg)
{
    string wifi_device = get_wifi_device(RadioId);

    string value = "";
    int2string(AMPDUEnable, value);

    sMsg += string("modify /config.ini/") + wifi_device + "/A_MPDU_11N=" + value + ";";
    return 0;
}
int RadioAmpdutlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int RadioBeaconIntetlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve16(BeaconInte);

    return 0;
}
int RadioBeaconIntetlv::assemble(buffer *buf)
{
    return 0;
}
int RadioBeaconIntetlv::saveToCM(string &sMsg)
{
    string wifi_device = get_wifi_device(RadioId);

    string value = "";
    int2string(BeaconInte, value);

    sMsg += string("modify /config.ini/") + wifi_device + "/BEACON_INTV=" + value + ";";
    return 0;
}
int RadioBeaconIntetlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int RadioDTIMIntetlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve8(DTIMInte);

    return 0;
}
int RadioDTIMIntetlv::assemble(buffer *buf)
{
    return 0;
}
int RadioDTIMIntetlv::saveToCM(string &sMsg)
{
    string wifi_device = get_wifi_device(RadioId);

    string value = "";
    int2string(DTIMInte, value);

    sMsg += string("modify /config.ini/") + wifi_device + "/DTIM_PERIOD=" + value + ";";
    return 0;
}
int RadioDTIMIntetlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int RadioSliceThresholdtlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve16(SliceThreshold);

    return 0;
}
int RadioSliceThresholdtlv::assemble(buffer *buf)
{
    return 0;
}
int RadioSliceThresholdtlv::saveToCM(string &sMsg)
{
    string wifi_device = get_wifi_device(RadioId);

    string value = "";
    int2string(SliceThreshold, value);

    sMsg += string("modify /config.ini/") + wifi_device + "/FRAG=" + value + ";";
    return 0;
}
int RadioSliceThresholdtlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int RadioRtsThresholdtlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve16(RtsThreshold);

    return 0;
}
int RadioRtsThresholdtlv::assemble(buffer *buf)
{
    return 0;
}
int RadioRtsThresholdtlv::saveToCM(string &sMsg)
{
    string wifi_device = get_wifi_device(RadioId);

    string value = "";
    int2string(RtsThreshold, value);

    sMsg += string("modify /config.ini/") + wifi_device + "/RTS=" + value + ";";
    return 0;
}
int RadioRtsThresholdtlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int RadioShortPreambletlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve8(ShortPreamble);

    return 0;
}
int RadioShortPreambletlv::assemble(buffer *buf)
{
    return 0;
}
int RadioShortPreambletlv::saveToCM(string &sMsg)
{
    string wifi_device = get_wifi_device(RadioId);

    string value = "";
    int2string(ShortPreamble, value);

    sMsg += string("modify /config.ini/") + wifi_device + "/SHORT_PREAMBLE=" + value + ";";
    return 0;
}
int RadioShortPreambletlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int RadioCountrytlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieveRawBytes(Country, 2);

    return 0;
}
int RadioCountrytlv::assemble(buffer *buf)
{
    return 0;
}
int RadioCountrytlv::saveToCM(string &sMsg)
{
    string wifi_device = get_wifi_device(RadioId);

    string value = "";
    value.assign((const char*)Country, 2);

    sMsg += string("modify /config.ini/") + wifi_device + "/COUNTRY_NAME=" + value + ";";
    return 0;
}
int RadioCountrytlv::loadFromCM(kvlist &kv)
{
    return 0;
}











int SetResulttlv::parse(buffer *buf)
{
    return 0;
}
int SetResulttlv::assemble(buffer *buf)
{
    skipheader(buf);

    buf->store8(result);
    length += 1;

    assembleHeader(buf);
    return 0;
}
int SetResulttlv::saveToCM(string &sMsg)
{
    return 0;
}
int SetResulttlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int SetTunnelInfotlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(u.tunnelconf);
    buf->retrieve8(tunnelMode);
    buf->retrieveRawBytes(ipaddr, 4);
    buf->retrieve16(portnumber);

    return 0;
}
int SetTunnelInfotlv::assemble(buffer *buf)
{
    return 0;
}
int SetTunnelInfotlv::saveToCM(string &sMsg)
{
    cout << "I : " << (int)u.s.iptype << endl;
    cout << "Tun T : " << (int)u.s.tunneltype << endl;
    cout << "IPADDR : " << hex << *(int*)ipaddr << dec << endl;
    cout << "PORT : " << portnumber << endl;

    if ((0 == u.s.iptype) && (1 == u.s.tunneltype))
    {
        char staIpaddr[32] = {0};
        snprintf(staIpaddr, sizeof(staIpaddr), "%u.%u.%u.%u",
                 ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3]);
        sMsg += string("modify /config.ini/network/AC_IP=") + staIpaddr + ";";

        string value = "";
        int2string(portnumber, value);
        sMsg += string("modify /config.ini/tunnel/REMOTE_PORT=") + value + ";";
    }
    return 0;
}
int SetTunnelInfotlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int SwitchBackupTunneltlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(u.tunnelconf);
    buf->retrieve8(tunnelMode);
    buf->retrieveRawBytes(ipaddr, 4);
    buf->retrieve16(portnumber);

    return 0;
}
int SwitchBackupTunneltlv::assemble(buffer *buf)
{
    return 0;
}
int SwitchBackupTunneltlv::saveToCM(string &sMsg)
{
    return 0;
}
int SwitchBackupTunneltlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int Generic5G11NPrioritytlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(enable);

    return 0;
}
int Generic5G11NPrioritytlv::assemble(buffer *buf)
{
    return 0;
}
int Generic5G11NPrioritytlv::saveToCM(string &sMsg)
{
#if 0
    string strenable = "";
    int2string(enable, strenable);

    sMsg += string("modify /config.ini/spectrum/SPECTRUM_ENABLE=") + strenable + ";";
#endif
    return 0;
}
int Generic5G11NPrioritytlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int GenericTunnelSliceThresholdtlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve16(threshold);

    return 0;
}
int GenericTunnelSliceThresholdtlv::assemble(buffer *buf)
{
    return 0;
}
int GenericTunnelSliceThresholdtlv::saveToCM(string &sMsg)
{
    return 0;
}
int GenericTunnelSliceThresholdtlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int GenericStationAgetlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve16(stationAgeing);

    return 0;
}
int GenericStationAgetlv::assemble(buffer *buf)
{
    return 0;
}
int GenericStationAgetlv::saveToCM(string &sMsg)
{
    return 0;
}
int GenericStationAgetlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int Generic5G11NPriorityNewtlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(enable5GPririty);
    buf->retrieve8(limitedNum);
    buf->retrieve16(reserve);

    return 0;
}
int Generic5G11NPriorityNewtlv::assemble(buffer *buf)
{
    return 0;
}
int Generic5G11NPriorityNewtlv::saveToCM(string &sMsg)
{
    string value = "";

    int2string(enable5GPririty, value);
    sMsg += string("modify /config.ini/spectrum/SPECTRUM_ENABLE=") + value + ";";

    int2string(limitedNum, value);
    sMsg += string("modify /config.ini/spectrum/LINK_THRESHOLD=") + value + ";";

    return 0;
}
int Generic5G11NPriorityNewtlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int UpdatewlanTunnelIDtlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve8(WlanId);
    buf->retrieve8(DataTunnelMode);

    return 0;
}
int UpdatewlanTunnelIDtlv::assemble(buffer *buf)
{
    return 0;
}
int UpdatewlanTunnelIDtlv::saveToCM(string &sMsg)
{
    string ifname = get_wlan_ifname(RadioId, WlanId);
    string tunmode = "";

    int2string(DataTunnelMode, tunmode);
    switch(DataTunnelMode)
    {
    case 0: // ¿¿¿¿ local mac
        sMsg += string("modify /config.ini/") + ifname + "/TUNNEL_ENABLE=" + tunmode + ";";
        sMsg += string("modify /config.ini/") + ifname + "/PORTAL_ENABLE=0;";
        sMsg += string("modify /config.ini/") + ifname + "/PORTAL_URL=;";
        break;
    case 1: // ¿¿¿¿
        sMsg += string("modify /config.ini/") + ifname + "/TUNNEL_ENABLE=" + tunmode + ";";
        sMsg += string("modify /config.ini/") + ifname + "/PORTAL_ENABLE=0;";
        sMsg += string("modify /config.ini/") + ifname + "/PORTAL_URL=;";
        break;
    case 2: // ¿¿¿¿¿¿
        sMsg += string("modify /config.ini/") + ifname + "/TUNNEL_ENABLE=0;";
        sMsg += string("modify /config.ini/") + ifname + "/PORTAL_ENABLE=0;";
        sMsg += string("modify /config.ini/") + ifname + "/PORTAL_URL=;";
        break;
    case 3: // ¿¿¿¿ ¿¿¿¿ ¿¿URL¿¿¿¿
        sMsg += string("modify /config.ini/") + ifname + "/TUNNEL_ENABLE=0;";
        sMsg += string("modify /config.ini/") + ifname + "/PORTAL_ENABLE=1;";
        break;
    default:
        sMsg += string("modify /config.ini/") + ifname + "/TUNNEL_ENABLE=1;";
        sMsg += string("modify /config.ini/") + ifname + "/PORTAL_ENABLE=0;";
        sMsg += string("modify /config.ini/") + ifname + "/PORTAL_URL=;";
        break;
    }


#ifdef WLAN2G5GSAMECONFIG
    string ifname5G = get_wlan_ifname(RadioId+1, WlanId);
    switch(DataTunnelMode)
    {
    case 0: // ¿¿¿¿ local mac
        sMsg += string("modify /config.ini/") + ifname5G + "/TUNNEL_ENABLE=" + tunmode + ";";
        sMsg += string("modify /config.ini/") + ifname5G + "/PORTAL_ENABLE=0;";
        sMsg += string("modify /config.ini/") + ifname5G + "/PORTAL_URL=;";
        break;
    case 1: // ¿¿¿¿
        sMsg += string("modify /config.ini/") + ifname5G + "/TUNNEL_ENABLE=" + tunmode + ";";
        sMsg += string("modify /config.ini/") + ifname5G + "/PORTAL_ENABLE=0;";
        sMsg += string("modify /config.ini/") + ifname5G + "/PORTAL_URL=;";
        break;
    case 2: // ¿¿¿¿¿¿
        sMsg += string("modify /config.ini/") + ifname5G + "/TUNNEL_ENABLE=0;";
        sMsg += string("modify /config.ini/") + ifname5G + "/PORTAL_ENABLE=0;";
        sMsg += string("modify /config.ini/") + ifname5G + "/PORTAL_URL=;";
        break;
    case 3: // ¿¿¿¿ ¿¿¿¿ ¿¿URL¿¿¿¿
        sMsg += string("modify /config.ini/") + ifname5G + "/TUNNEL_ENABLE=0;";
        sMsg += string("modify /config.ini/") + ifname5G + "/PORTAL_ENABLE=1;";
        break;
    default:
        sMsg += string("modify /config.ini/") + ifname5G + "/TUNNEL_ENABLE=1;";
        sMsg += string("modify /config.ini/") + ifname5G + "/PORTAL_ENABLE=0;";
        sMsg += string("modify /config.ini/") + ifname5G + "/PORTAL_URL=;";
        break;
    }

#endif
    return 0;
}
int UpdatewlanTunnelIDtlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int UpdatewlanUpflowctltlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve8(WlanId);
    buf->retrieve32(SsidUpFlow);
    buf->retrieve32(UserUpFlow);

    return 0;
}
int UpdatewlanUpflowctltlv::assemble(buffer *buf)
{
    return 0;
}
int UpdatewlanUpflowctltlv::saveToCM(string &sMsg)
{
    string ifname = get_wlan_ifname(RadioId, WlanId);
    string strssidup = "";
    string struserup = "";
    int2string(SsidUpFlow, strssidup);
    int2string(UserUpFlow, struserup);

    sMsg += string("modify /config.ini/") + ifname + "/MAX_UP_TRAFFIC=" + strssidup + ";";
    sMsg += string("modify /config.ini/") + ifname + "/USER_UP_BANDWIDTH=" + struserup + ";";

#ifdef WLAN2G5GSAMECONFIG
    string ifname5G = get_wlan_ifname(RadioId+1, WlanId);
    sMsg += string("modify /config.ini/") + ifname5G + "/MAX_UP_TRAFFIC=" + strssidup + ";";
    sMsg += string("modify /config.ini/") + ifname5G + "/USER_UP_BANDWIDTH=" + struserup + ";";
#endif
    return 0;
}
int UpdatewlanUpflowctltlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int UpdatewlanEnableWlantlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve8(WlanId);
    buf->retrieve8(Enable);

    return 0;
}
int UpdatewlanEnableWlantlv::assemble(buffer *buf)
{
    return 0;
}
int UpdatewlanEnableWlantlv::saveToCM(string &sMsg)
{
    string ifname = get_wlan_ifname(RadioId, WlanId);
    string strenable = "";
    unsigned char Disable = Enable?0:1;

    int2string(Disable, strenable);

    sMsg += string("modify /config.ini/") + ifname + "/WLAN_ENABLE=" + strenable + ";";

#ifdef WLAN2G5GSAMECONFIG
    string ifname5G = get_wlan_ifname(RadioId+1, WlanId);
    sMsg += string("modify /config.ini/") + ifname5G + "/WLAN_ENABLE=" + strenable + ";";
#endif

    return 0;
}
int UpdatewlanEnableWlantlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int UpdatewlanLocalSsidtlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve8(WlanId);
    buf->retrieve8(Enable);
    buf->retrieve16(SsidLength);
    Ssid.assign(buf->GetPtr(), SsidLength);
    buf->retrieveRawBytes(NULL, SsidLength);

    return 0;
}
int UpdatewlanLocalSsidtlv::assemble(buffer *buf)
{
    return 0;
}
int UpdatewlanLocalSsidtlv::saveToCM(string &sMsg)
{
    string ifname = get_wlan_ifname(RadioId, WlanId);
    if (1 == Enable)
    {
        if (UTF8 == be_UTF8(Ssid.c_str()))
        {
            string gb2312ssid = "";
            changeUnicode(Ssid, "utf-8", gb2312ssid, "gb2312");
            sMsg += string("modify /config.ini/") + ifname + "/ESSID_UNICODE=1;";
            sMsg += string("modify /config.ini/") + ifname + "/ESSID=" + gb2312ssid + ";";

#ifdef WLAN2G5GSAMECONFIG
            string ifname5G = get_wlan_ifname(RadioId+1, WlanId);
            sMsg += string("modify /config.ini/") + ifname5G + "/ESSID_UNICODE=1;";
            sMsg += string("modify /config.ini/") + ifname5G + "/ESSID=" + gb2312ssid + ";";
#endif
        }
        else
        {
            sMsg += string("modify /config.ini/") + ifname + "/ESSID_UNICODE=0;";
            sMsg += string("modify /config.ini/") + ifname + "/ESSID=" + Ssid + ";";

#ifdef WLAN2G5GSAMECONFIG
            string ifname5G = get_wlan_ifname(RadioId+1, WlanId);
            sMsg += string("modify /config.ini/") + ifname5G + "/ESSID_UNICODE=0;";
            sMsg += string("modify /config.ini/") + ifname5G + "/ESSID=" + Ssid + ";";
#endif
        }

    }
    else
    {
        sMsg += string("modify /config.ini/") + ifname + "/ESSID_UNICODE=0;";

#ifdef WLAN2G5GSAMECONFIG
        string ifname5G = get_wlan_ifname(RadioId+1, WlanId);
        sMsg += string("modify /config.ini/") + ifname5G + "/ESSID_UNICODE=0;";
#endif
    }
    return 0;
}
int UpdatewlanLocalSsidtlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int UpdatewlanPortalUrltlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve8(WlanId);
    url.assign(buf->GetPtr(), length-2);
    buf->retrieveRawBytes(NULL, length-2);

    return 0;
}
int UpdatewlanPortalUrltlv::assemble(buffer *buf)
{
    return 0;
}
int UpdatewlanPortalUrltlv::saveToCM(string &sMsg)
{
    string ifname = get_wlan_ifname(RadioId, WlanId);
    if (url.length())
    {
        sMsg += "modify /config.ini/" + ifname + "/PORTAL_URL=" + url + ";";
    }
    else
    {
        sMsg += "modify /config.ini/" + ifname + "/PORTAL_URL=" + url + ";";
    }

#ifdef WLAN2G5GSAMECONFIG
    string ifname5G = get_wlan_ifname(RadioId+1, WlanId);
    if (url.length())
    {
        sMsg += "modify /config.ini/" + ifname5G + "/PORTAL_URL=" + url + ";";
    }
    else
    {
        sMsg += "modify /config.ini/" + ifname5G + "/PORTAL_URL=" + url + ";";
    }
#endif

    return 0;
}
int UpdatewlanPortalUrltlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int UpdatewlanIsolatetlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(RadioId);
    buf->retrieve8(WlanId);
    buf->retrieve8(u.isolate);

    return 0;
}
int UpdatewlanIsolatetlv::assemble(buffer *buf)
{
    return 0;
}
int UpdatewlanIsolatetlv::saveToCM(string &sMsg)
{
    return 0;
}
int UpdatewlanIsolatetlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int KeyEventsHotReboot::parse(buffer *buf)
{
    return 0;
}
int KeyEventsHotReboot::assemble(buffer *buf)
{
    skipheader(buf);

    buf->store16(reason);
    length += 2;

    assembleHeader(buf);

    return 0;
}
int KeyEventsHotReboot::saveToCM(string &sMsg)
{
    return 0;
}
int KeyEventsHotReboot::loadFromCM(kvlist &kv)
{
    return 0;
}

int KeyEventsRadioUpdown::parse(buffer *buf)
{
    return 0;
}
int KeyEventsRadioUpdown::assemble(buffer *buf)
{
    skipheader(buf);

    buf->store8(RadioId);
    length += 1;
    buf->store8(RadioState);
    length += 1;
    buf->store16(reason);
    length += 2;

    assembleHeader(buf);

    return 0;
}
int KeyEventsRadioUpdown::saveToCM(string &sMsg)
{
    return 0;
}
int KeyEventsRadioUpdown::loadFromCM(kvlist &kv)
{
    return 0;
}

int KeyEventsWlanDelDown::parse(buffer *buf)
{
    return 0;
}
int KeyEventsWlanDelDown::assemble(buffer *buf)
{
    skipheader(buf);

    buf->store8(RadioId);
    length += 1;
    buf->store8(WlanId);
    length += 1;
    buf->store16(reason);
    length += 2;

    assembleHeader(buf);

    return 0;
}
int KeyEventsWlanDelDown::saveToCM(string &sMsg)
{
    return 0;
}
int KeyEventsWlanDelDown::loadFromCM(kvlist &kv)
{
    return 0;
}

int KeyEventsWlanUp::parse(buffer *buf)
{
    return 0;
}
int KeyEventsWlanUp::assemble(buffer *buf)
{
    skipheader(buf);

    buf->store8(RadioId);
    length += 1;
    buf->store8(WlanId);
    length += 1;
    buf->storeRawBytes(BSSID, 6);
    length += 6;

    assembleHeader(buf);

    return 0;
}
int KeyEventsWlanUp::saveToCM(string &sMsg)
{
    return 0;
}
int KeyEventsWlanUp::loadFromCM(kvlist &kv)
{
    unsigned int iValue = 0;
    int mac[6] = {0};
    string value = "";

    value = kv.getValue("radioid");
    string2int(value, iValue);
    RadioId = iValue;

    value = kv.getValue("wlanid");
    string2int(value, iValue);
    WlanId = iValue;

    value = kv.getValue("bssid");
    sscanf(value.c_str(), "%02x%02x%02x%02x%02x%02x",
           &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
    BSSID[0] = mac[0];
    BSSID[1] = mac[1];
    BSSID[2] = mac[2];
    BSSID[3] = mac[3];
    BSSID[4] = mac[4];
    BSSID[5] = mac[5];

    return 0;
}

int ACLoginResultIPV4::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve16(result);
    buf->retrieve16(errno);
    buf->retrieveRawBytes(ipaddr, 4);

    return 0;
}
int ACLoginResultIPV4::assemble(buffer *buf)
{
    return 0;
}
int ACLoginResultIPV4::saveToCM(string &sMsg)
{
    if (0 == result)
    {
        sMsg += string("modify /config.ini/network/CONNECTION_MODE=1;");
    }
    else
    {
        sMsg += string("modify /config.ini/network/CONNECTION_MODE=0;");
    }
    return 0;
}
int ACLoginResultIPV4::loadFromCM(kvlist &kv)
{
    return 0;
}

int SetSnmpGlobalInteval::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve16(inteval);

    return 0;
}
int SetSnmpGlobalInteval::assemble(buffer *buf)
{
    return 0;
}
int SetSnmpGlobalInteval::saveToCM(string &sMsg)
{
    return 0;
}
int SetSnmpGlobalInteval::loadFromCM(kvlist &kv)
{
    return 0;
}

int SetSnmpTrapKey::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    key.assign(buf->GetPtr(), length);
    buf->retrieveRawBytes(NULL, length);

    return 0;
}
int SetSnmpTrapKey::assemble(buffer *buf)
{
    return 0;
}
int SetSnmpTrapKey::saveToCM(string &sMsg)
{
    return 0;
}
int SetSnmpTrapKey::loadFromCM(kvlist &kv)
{
    return 0;
}

int SetSnmpReadKey::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    key.assign(buf->GetPtr(), length);
    buf->retrieveRawBytes(NULL, length);

    return 0;
}
int SetSnmpReadKey::assemble(buffer *buf)
{
    return 0;
}
int SetSnmpReadKey::saveToCM(string &sMsg)
{
    sMsg += string("modify /config.ini/snmp/ROCOMMUNITY=") + key + ";";
    return 0;
}
int SetSnmpReadKey::loadFromCM(kvlist &kv)
{
    return 0;
}

int SetSnmpWriteKey::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    key.assign(buf->GetPtr(), length);
    buf->retrieveRawBytes(NULL, length);

    return 0;
}
int SetSnmpWriteKey::assemble(buffer *buf)
{
    return 0;
}
int SetSnmpWriteKey::saveToCM(string &sMsg)
{
    sMsg += string("modify /config.ini/snmp/RWCOMMUNITY=") + key + ";";
    return 0;
}
int SetSnmpWriteKey::loadFromCM(kvlist &kv)
{
    return 0;
}

int SetSnmpAlarmsSwitch::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve16(alarms_count);
    for (int i=0; i<alarms_count; ++i)
    {
        __alarms a;
        buf->retrieve16(a.alarmId);
        buf->retrieve16(a.enable);
        buf->retrieve16(a.threshold);

        alarms.push_back(a);
    }

    return 0;
}
int SetSnmpAlarmsSwitch::assemble(buffer *buf)
{
    return 0;
}
int SetSnmpAlarmsSwitch::saveToCM(string &sMsg)
{
    return 0;
}
int SetSnmpAlarmsSwitch::loadFromCM(kvlist &kv)
{
    return 0;
}

int SystemMaintenancetlv::parse(buffer *buf)
{
    if (0 != parseEH(buf))
        return 0;

    buf->retrieve8(operation);

    return 0;
}
int SystemMaintenancetlv::assemble(buffer *buf)
{
    return 0;
}
int SystemMaintenancetlv::saveToCM(string &sMsg)
{
    switch(operation)
    {
    case 1:
        sMsg += "cmd /reboot;";
        break;
    case 2:
        break;
    }
    return 0;
}
int SystemMaintenancetlv::loadFromCM(kvlist &kv)
{
    return 0;
}

int StaAssosicMacaddrtlv::parse(buffer *buf)
{
    return 0;
}
int StaAssosicMacaddrtlv::assemble(buffer *buf)
{
    skipheader(buf);

    buf->storeRawBytes(macaddr, 6);
    length += 6;

    assembleHeader(buf);
    return 0;
}
int StaAssosicMacaddrtlv::saveToCM(string &sMsg)
{
    return 0;
}
int StaAssosicMacaddrtlv::loadFromCM(kvlist &kv)
{
    string value = "";
    int mac[6];

    value = kv.getValue("stamac");
    sscanf(value.c_str(), "%02x%02x%02x%02x%02x%02x",
           &mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
    macaddr[0] = (unsigned char)mac[0];
    macaddr[1] = (unsigned char)mac[1];
    macaddr[2] = (unsigned char)mac[2];
    macaddr[3] = (unsigned char)mac[3];
    macaddr[4] = (unsigned char)mac[4];
    macaddr[5] = (unsigned char)mac[5];

    return 0;
}

int StaAssosicIpv4addrtlv::parse(buffer *buf)
{
    return 0;
}
int StaAssosicIpv4addrtlv::assemble(buffer *buf)
{
    skipheader(buf);

    buf->storeRawBytes(ipaddr, 4);
    length += 4;

    assembleHeader(buf);
    return 0;
}
int StaAssosicIpv4addrtlv::saveToCM(string &sMsg)
{
    return 0;
}
int StaAssosicIpv4addrtlv::loadFromCM(kvlist &kv)
{
    string value = "";
    int ip[4];

    value = kv.getValue("staip");
    sscanf(value.c_str(), "%u.%u.%u.%u",
           &ip[0],&ip[1],&ip[2],&ip[3]);

    ipaddr[0] = (unsigned char)ip[0];
    ipaddr[1] = (unsigned char)ip[1];
    ipaddr[2] = (unsigned char)ip[2];
    ipaddr[3] = (unsigned char)ip[3];

    return 0;
}





