#include "platform.h"
#ifndef __PROTO_ELEMENTS_H__
#define __PROTO_ELEMENTS_H__

#define get_wifi_device(RadioId) \
    (((RadioId)==1)?"wifi0":"wifi1")

string get_wlan_ifname(unsigned char RadioId, unsigned char WlanId);

enum image_elements_type
{
    AP_ZIT_IMAGE_ELEMENTS_PROTOCOL = 32,
    AP_ZIT_IMAGE_ELEMENTS_SERVERIPV4 = 33,
    AP_ZIT_IMAGE_ELEMENTS_SERVERIPV6 = 34,
    AP_ZIT_IMAGE_ELEMENTS_URL = 35,
    AP_ZIT_IMAGE_ELEMENTS_USERNAME = 36,
    AP_ZIT_IMAGE_ELEMENTS_PASSWD = 37,
    AP_ZIT_IMAGE_ELEMENTS_CERTTYPE = 128,
    AP_ZIT_IMAGE_ELEMENTS_CERT = 129,
    AP_ZIT_IMAGE_ELEMENTS_DECODEKEY = 130,
};
enum stainfo_elements_type
{
    AP_ZIT_STAINFO_ELEMENTS_QOS = 1,
    AP_ZIT_STAINFO_ELEMENTS_MSGTYPE = 2,
    AP_ZIT_STAINFO_ELEMENTS_WLANID = 3,
    AP_ZIT_STAINFO_ELEMENTS_PMK = 5,
    AP_ZIT_STAINFO_ELEMENTS_RADIOID = 6,
    AP_ZIT_STAINFO_ELEMENTS_USERFLOW = 7,
    AP_ZIT_STAINFO_ELEMENTS_LEAVEREASON = 8,
    AP_ZIT_STAINFO_ELEMENTS_AUTHFAILREASON = 9,
};
enum stationstate_elements_type
{
    AP_ZIT_STATIONSTATE_ELEMENTS_ADDSTATION = 1,
    AP_ZIT_STATIONSTATE_ELEMENTS_DELSTATION = 2,
    AP_ZIT_STATIONSTATE_ELEMENTS_QOSUPDATE = 3,
    AP_ZIT_STATIONSTATE_ELEMENTS_VLANUPDATE = 4,
    AP_ZIT_STATIONSTATE_ELEMENTS_STATIDUPDATE = 5,
    AP_ZIT_STATIONSTATE_ELEMENTS_PMK = 6,

    //智能转发才会下发本消息
    AP_ZIT_STATIONSTATE_ELEMENTS_USERGRANTRSP = 13,
    AP_ZIT_STATIONSTATE_ELEMENTS_USERGRANT = 14,
    AP_ZIT_STATIONSTATE_ELEMENTS_DEAUTH = 15,
};
enum stationstate_element_usergrant_subtype
{
    AP_ZIT_STATIONSTATE_ELEMENT_USERAGENT_STASTAT = 1,
    AP_ZIT_STATIONSTATE_ELEMENT_USERAGENT_USERACL = 2,
    AP_ZIT_STATIONSTATE_ELEMENT_USERAGENT_USERTIMESTAMP = 3,
};

enum radio_elements_type
{
    AP_ZIT_RADIO_ELEMENTS_RADIOTYPE = 1,
    AP_ZIT_RADIO_ELEMENTS_RADIOPOWER = 2,
    AP_ZIT_RADIO_ELEMENTS_RADIOCHANNEL = 3,
    AP_ZIT_RADIO_ELEMENTS_RATESET11N = 5,
    AP_ZIT_RADIO_ELEMENTS_CHAINMASK = 6,
    AP_ZIT_RADIO_ELEMENTS_AMPDU = 7,
    AP_ZIT_RADIO_ELEMENTS_BANDWIDTH = 8,
    AP_ZIT_RADIO_ELEMENTS_SHORTGI = 9,
    AP_ZIT_RADIO_ELEMENTS_HTMODEL = 10,
    AP_ZIT_RADIO_ELEMENTS_AMSDU = 11,
    AP_ZIT_RADIO_ELEMENTS_BEACONINTE = 12,
    AP_ZIT_RADIO_ELEMENTS_DTIMINTE = 13,
    AP_ZIT_RADIO_ELEMENTS_SLICETHRESHOLD = 14,
    AP_ZIT_RADIO_ELEMENTS_RTSTHRESHOLD =15,
    AP_ZIT_RADIO_ELEMENTS_SHORTPREAMBLE = 16,
    AP_ZIT_RADIO_ELEMENTS_COUNTRY = 17,
    AP_ZIT_RADIO_ELEMENTS_MACARGS = 18,
    AP_ZIT_RADIO_ELEMENTS_COUNTRYMAN = 19,
    AP_ZIT_RADIO_ELEMENTS_ANTENNA = 20,
    AP_ZIT_RADIO_ELEMENTS_RATESET = 21,
    AP_ZIT_RADIO_ELEMENTS_EXTRATESET = 23,
    AP_ZIT_RADIO_ELEMENTS_APSCENE = 25,
    AP_ZIT_RADIO_ELEMENTS_ANTENNAMODE = 26,
    AP_ZIT_RADIO_ELEMENTS_FRAMERETRANS = 27,
    AP_ZIT_RADIO_ELEMENTS_SETMCS11N = 28,
    AP_ZIT_RADIO_ELEMENTS_TPC = 29,
};
enum response_elements_type
{
    AP_ZIT_RESPONSE_ELEMENTS_RESULT = 1,
};

enum loadbalance_elements_type
{
    AP_ZIT_LOADBALANCE_ELEMENTS_ENABLE = 1,
    AP_ZIT_LOADBALANCE_ELEMENTS_APBALANCE = 2,
    AP_ZIT_LOADBALANCE_ELEMENTS_WLANBALANCE =3,
};

enum datatunnel_elements_type
{
    AP_ZIT_CHANGETUNNEL_ELEMENTS_TUNNELINFO = 1,
    AP_ZIT_CHANGETUNNEL_ELEMENTS_SWITCHBACKUP = 2,
};

enum genericconfig_elements_type
{
    AP_ZIT_GENERICCONFIG_5GPRIORITY = 20,
    AP_ZIT_GENERICCONFIG_TUNNELSLICETHRESHOLD = 23,
    AP_ZIT_GENERICCONFIG_STATIONAGE = 25,
    AP_ZIT_GENERICCONFIG_5GPRIORITYNEW = 51,
};

enum updatewlan_elements_type
{
    AP_ZIT_UPDATEWLAN_ELEMENTS_TUNNELID = 2,
    AP_ZIT_UPDATEWLAN_ELEMENTS_UPFLOW = 3,
    AP_ZIT_UPDATEWLAN_ELEMENTS_FLOWCTLTYPE = 4,
    AP_ZIT_UPDATEWLAN_ELEMENTS_UPDATEASSVRIPV4 = 6,
    AP_ZIT_UPDATEWLAN_ELEMENTS_UPDATEASCERT = 7,
    AP_ZIT_UPDATEWLAN_ELEMENTS_UPDATEASSVRIPV6 = 8,
    AP_ZIT_UPDATEWLAN_ELEMENTS_UPDATERADIUSSVRIPV4 = 9,
    AP_ZIT_UPDATEWLAN_ELEMENTS_UPDATERADIUSSVRIPV6 = 10,
    AP_ZIT_UPDATEWLAN_ELEMENTS_WLANENABLE = 11,
    AP_ZIT_UPDATEWLAN_ELEMENTS_WMMTO8021P = 12,
    AP_ZIT_UPDATEWLAN_ELEMENTS_WMMTODSCP = 13,
    AP_ZIT_UPDATEWLAN_ELEMENTS_MIXMODEL = 14,
    AP_ZIT_UPDATEWLAN_ELEMENTS_ETHERISOLAATION = 15,
    AP_ZIT_UPDATEWLAN_ELEMENTS_EAPAUTHPOINT = 19,
    AP_ZIT_UPDATEWLAN_ELEMENTS_VOICESERVICE = 20,
    AP_ZIT_UPDATEWLAN_ELEMENTS_PROBERESPONSECTL = 21,
    AP_ZIT_UPDATEWLAN_ELEMENTS_CTLFRAMERATE = 22,
    AP_ZIT_UPDATEWLAN_ELEMENTS_ADDMACFILTERLIST = 23,
    AP_ZIT_UPDATEWLAN_ELEMENTS_DELMACFILTERLIST = 24,
    AP_ZIT_UPDATEWLAN_ELEMENTS_LOCALESSID = 25,
    AP_ZIT_UPDATEWLAN_ELEMENTS_SSIDPORTLURL = 44,
};

enum keyevents_elements_type
{
    AP_ZIT_KEYEVENTS_ELEMENTS_HOTREBOOT = 1,
    AP_ZIT_KEYEVENTS_ELEMENTS_RADIOUPDOWN = 2,
    AP_ZIT_KEYEVENTS_ELEMENTS_WLANDELDOWN = 5,
    AP_ZIT_KEYEVENTS_ELEMENTS_WLANUP = 6,
};

enum aclogin_elements_type
{
    AP_ZIT_ACLOGIN_ELEMENTS_IPV4 = 1,
    AP_ZIT_ACLOGIN_ELEMENTS_IPV6 = 2,
};

enum snmpconfig_elements_type
{
    AP_ZIT_SETSNMP_ELEMENTS_GLOBALINTE = 1,
    AP_ZIT_SETSNMP_ELEMENTS_TRAPKEY = 2,
    AP_ZIT_SETSNMP_ELEMENTS_READKEY = 3,
    AP_ZIT_SETSNMP_ELEMENTS_WRITEKEY = 4,
    AP_ZIT_SETSNMP_ELEMENTS_ALARMSWITCH = 9,
};

enum basicstatics_elements_type
{
    AP_ZIT_BASICSTATICS_ELEMENTS_DEVCODE = 20,
    AP_ZIT_BASICSTATICS_ELEMENTS_PHYINTERCOUNT = 21,
    AP_ZIT_BASICSTATICS_ELEMENTS_CPUINFO = 22,
    AP_ZIT_BASICSTATICS_ELEMENTS_MEMCARDTYPE = 23,
    AP_ZIT_BASICSTATICS_ELEMENTS_MEMCARDCAP = 24,
    AP_ZIT_BASICSTATICS_ELEMENTS_FLASHCAP = 25,
    AP_ZIT_BASICSTATICS_ELEMENTS_IPADDRIPV4 = 26,
    AP_ZIT_BASICSTATICS_ELEMENTS_NETWORK = 27,
    AP_ZIT_BASICSTATICS_ELEMENTS_GATEWAY = 28,
    AP_ZIT_BASICSTATICS_ELEMENTS_DEVINFO = 29,
    AP_ZIT_BASICSTATICS_ELEMENTS_SOFTWARENAME = 30,
    AP_ZIT_BASICSTATICS_ELEMENTS_DATACARD = 31,
    AP_ZIT_BASICSTATICS_ELEMENTS_DATACARDVER = 32,
    AP_ZIT_BASICSTATICS_ELEMENTS_IPADDRIPV6 = 33,
    AP_ZIT_BASICSTATICS_ELEMENTS_PREADDRIPV6 = 34,
    AP_ZIT_BASICSTATICS_ELEMENTS_GATEWAYIPV6 = 35,
    AP_ZIT_BASICSTATICS_ELEMENTS_TFCARDCAP = 42,
    AP_ZIT_BASICSTATICS_ELEMENTS_SIMIMSI = 43,

    AP_ZIT_BASICSTATICS_ELEMENTS_CPUUSAGE = 50,
    AP_ZIT_BASICSTATICS_ELEMENTS_MEMUSAGE = 51,
    AP_ZIT_BASICSTATICS_ELEMENTS_APCURFLOW = 52,
    AP_ZIT_BASICSTATICS_ELEMENTS_MONITORMODE = 53,
    AP_ZIT_BASICSTATICS_ELEMENTS_SSIDCOUNT = 54,
    AP_ZIT_BASICSTATICS_ELEMENTS_STAALARMTHRE = 55,
    AP_ZIT_BASICSTATICS_ELEMENTS_COFREALARMTHRE = 56,
    AP_ZIT_BASICSTATICS_ELEMENTS_FREALARMTHRE = 57,
    AP_ZIT_BASICSTATICS_ELEMENTS_CPUUSAGETHRE = 58,
    AP_ZIT_BASICSTATICS_ELEMENTS_MEMUSAGETHRE = 59,
    AP_ZIT_BASICSTATICS_ELEMENTS_FLASHLEFT = 60,
    AP_ZIT_BASICSTATICS_ELEMENTS_TFSTATE = 67,
    AP_ZIT_BASICSTATICS_ELEMENTS_TFLEFT = 68,
    AP_ZIT_BASICSTATICS_ELEMENTS_PORTALVER = 69,

    AP_ZIT_BASICSTATICS_ELEMENTS_CPUUSAGEAVE = 100,
    AP_ZIT_BASICSTATICS_ELEMENTS_MEMUSAGEAVE = 101,
    AP_ZIT_BASICSTATICS_ELEMENTS_ASSOCIATECOUNT = 105,
    AP_ZIT_BASICSTATICS_ELEMENTS_ASSOFAILCOUNT = 106,
    AP_ZIT_BASICSTATICS_ELEMENTS_REASSOCOUNT = 107,
    AP_ZIT_BASICSTATICS_ELEMENTS_ASSOUNKNOWNFAILCOUNT = 108,
    AP_ZIT_BASICSTATICS_ELEMENTS_ABNORMALBREAKCOUNT = 109,
    AP_ZIT_BASICSTATICS_ELEMENTS_ASSORESFINCOUNT = 110,
    AP_ZIT_BASICSTATICS_ELEMENTS_ASSORATEFAILCOUNT = 111,
    AP_ZIT_BASICSTATICS_ELEMENTS_LOWRSSIFAILCOUNT = 112,
    AP_ZIT_BASICSTATICS_ELEMENTS_CPUUSAGEMAX = 113,
    AP_ZIT_BASICSTATICS_ELEMENTS_MEMUSAGEMAX = 114,
    AP_ZIT_BASICSTATICS_ELEMENTS_DEVUPTIME = 115,
    AP_ZIT_BASICSTATICS_ELEMENTS_REASSOFAILCOUNT = 116,
    AP_ZIT_BASICSTATICS_ELEMENTS_ASSOFAILUNKNOWNREASON = 117,
    AP_ZIT_BASICSTATICS_ELEMENTS_ASSOFAILUNKNOWNPROTO = 118,
    AP_ZIT_BASICSTATICS_ELEMENTS_ALLSTAASSOTIME = 119,
    AP_ZIT_BASICSTATICS_ELEMENTS_LTEUPFLOW = 122,
    AP_ZIT_BASICSTATICS_ELEMENTS_LTEDOWNFLOW = 123,
};

enum maintenance_elements_type
{
    AP_ZIT_MAINTENANCE_ELEMENTS_SYSTEMOPERATOR = 1,
};

enum staassosicinfo_elements_type
{
    AP_ZIT_STAASSOSICINFO_STAMACADDR = 1,
    AP_ZIT_STAASSOSICINFO_STARATESTATISTICS = 3,
    AP_ZIT_STAASSOSICINFO_STADBMSTATISTICS = 4,
    AP_ZIT_STAASSOSICINFO_STASNRSTATISTICS = 5,
    AP_ZIT_STAASSOSICINFO_STAPKGSSTATISTICS = 6,
    AP_ZIT_STAASSOSICINFO_STAERRPKGSSTATISTICS = 7,
    AP_ZIT_STAASSOSICINFO_STAIPV4ADDR = 9,
    AP_ZIT_STAASSOSICINFO_STAIPV6ADDR = 10,
};

class Element
{
public:
    unsigned short type;
    unsigned short length;

    bool valid;

public:
    Element():length(0), valid(false){}
    virtual ~Element(){}

    virtual int parse(buffer *buf);
    virtual int assemble(buffer *buf);
    virtual int saveToCM(string &sMsg);
    virtual int loadFromCM(kvlist &kv);
    int parseEH(buffer *buf);
    int skipheader(buffer *buf);
    int assembleHeader(buffer* buf);
    int skiptlv(buffer *buf)
    {
        unsigned short _type,_length;
        buf->retrieve16(_type);
        buf->retrieve16(_length);
        buf->retrieveRawBytes(NULL, _length);
        cout << "skip an unsupport tlv type = " << _type << " length = " << _length << endl;
        syslog(LOG_DEBUG, "[%s:%d] type %d length %d is not support,skip it\n", __func__, __LINE__,
               _type, _length);

        return 0;
    }
    bool isValid()
    {
        return valid;
    }
    void setValid(bool Valid)
    {
        valid = Valid;
    }
};

class image_protocoltlv : public Element
{
public:
    unsigned char proto;
public:
    image_protocoltlv()
    {
        type = AP_ZIT_IMAGE_ELEMENTS_PROTOCOL;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class image_serveripv4tlv : public Element
{
public:
    unsigned char ipaddr[4];
public:
    image_serveripv4tlv()
    {
        type = AP_ZIT_IMAGE_ELEMENTS_SERVERIPV4;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class image_serveripv6tlv : public Element
{
public:
    unsigned char ipaddr[16];
public:
    image_serveripv6tlv()
    {
        type = AP_ZIT_IMAGE_ELEMENTS_SERVERIPV6;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class image_urltlv : public Element
{
public:
    string url;
public:
    image_urltlv()
    {
        type = AP_ZIT_IMAGE_ELEMENTS_URL;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class image_usernametlv : public Element
{
public:
    string username;
public:
    image_usernametlv()
    {
        type = AP_ZIT_IMAGE_ELEMENTS_USERNAME;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class image_passwdtlv : public Element
{
public:
    string passwd;
public:
    image_passwdtlv()
    {
        type = AP_ZIT_IMAGE_ELEMENTS_PASSWD;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class image_certtypetlv : public Element
{
public:
    unsigned char certtype;
public:
    image_certtypetlv()
    {
        type = AP_ZIT_IMAGE_ELEMENTS_CERTTYPE;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class image_certtlv : public Element
{
public:
    string cert;  //用string 有可能不生效
public:
    image_certtlv()
    {
        type = AP_ZIT_IMAGE_ELEMENTS_CERT;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class image_decodekeytlv : public Element
{
public:
    string key;
public:
    image_decodekeytlv()
    {
        type = AP_ZIT_IMAGE_ELEMENTS_DECODEKEY;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class stainfo_qostlv : public Element
{
public:
    unsigned char qos;
public:
    stainfo_qostlv()
    {
        type = AP_ZIT_STAINFO_ELEMENTS_QOS;
    }
    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class stainfo_msgtypetlv : public Element
{
public:
    unsigned char msgtype;
public:

    stainfo_msgtypetlv()
    {
        type = AP_ZIT_STAINFO_ELEMENTS_MSGTYPE;
    }
    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class stainfo_wlanidtlv : public Element
{
public:
    unsigned char WlanId;
public:
    stainfo_wlanidtlv()
    {
        type = AP_ZIT_STAINFO_ELEMENTS_WLANID;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class stainfo_radioidtlv : public Element
{
public:
    unsigned char RadioId;
public:
    stainfo_radioidtlv()
    {
        type = AP_ZIT_STAINFO_ELEMENTS_RADIOID;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class stainfo_leavereasontlv : public Element
{
public:
    unsigned short reason;
public:
    stainfo_leavereasontlv()
    {
        type = AP_ZIT_STAINFO_ELEMENTS_LEAVEREASON;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class stastatus_addstation : public Element
{
public:
    unsigned char RadioId;
    unsigned char WlanId;
    unsigned short AddType;
    unsigned char UserMac[6];
public:
    stastatus_addstation()
    {
        type = AP_ZIT_STATIONSTATE_ELEMENTS_ADDSTATION;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class stastatus_delstation : public Element
{
public:
    unsigned char RadioId;
    unsigned char WlanId;
    unsigned short DelCode;
    unsigned char UserMac[6];
public:
    stastatus_delstation()
    {
        type = AP_ZIT_STATIONSTATE_ELEMENTS_DELSTATION;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class stastatus_vlanupdate : public Element
{
public:
    unsigned char UserMac[6];
    unsigned short vlanTag;
public:
    stastatus_vlanupdate()
    {
        type = AP_ZIT_STATIONSTATE_ELEMENTS_VLANUPDATE;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class stastatus_statidupdate : public Element
{
public:
    //大小端注意
#ifdef i386
    union {
        struct {
            unsigned char reverse:4;
            unsigned char act:4;
        }s;
        unsigned char action;
    }u;
#else
    union {
        struct {
            unsigned char reverse:4;
            unsigned char act:4;
        }s;
        unsigned char action;
    }u;
#endif

    unsigned char slotid;
    unsigned char UserMac[6];
    unsigned int stasid;
public:
    stastatus_statidupdate()
    {
        type = AP_ZIT_STATIONSTATE_ELEMENTS_STATIDUPDATE;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class stastatus_useragent_stastat : public Element
{
public:
    unsigned char value;

public:
    stastatus_useragent_stastat()
    {
        type = AP_ZIT_STATIONSTATE_ELEMENT_USERAGENT_STASTAT;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class stastatus_useragent_useracl : public Element
{
public:
    unsigned short aclGroupId;

public:
    stastatus_useragent_useracl()
    {
        type = AP_ZIT_STATIONSTATE_ELEMENT_USERAGENT_USERACL;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class stastatus_useragent_usertimestamp : public Element
{
public:
    unsigned int TimeStamp;

public:
    stastatus_useragent_usertimestamp()
    {
        type = AP_ZIT_STATIONSTATE_ELEMENT_USERAGENT_USERTIMESTAMP;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class stastatus_useragent : public Element
{
public:
    unsigned char UserMac[6];
    unsigned char RadioId;
    unsigned char WlanId;
    stastatus_useragent_stastat stastate;
    stastatus_useragent_useracl useracl;
    stastatus_useragent_usertimestamp timerstamp;

public:
    stastatus_useragent()
    {
        type = AP_ZIT_STATIONSTATE_ELEMENTS_USERGRANT;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class RadioModelSettlv : public Element
{
public:
    unsigned char RadioId;
    unsigned int RadioModel;
public:
    RadioModelSettlv()
    {
        type = AP_ZIT_RADIO_ELEMENTS_RADIOTYPE;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class RadioPowertlv : public Element
{
public:
    unsigned char RadioId;
    unsigned char PowerType;
    unsigned short Power;
public:
    RadioPowertlv()
    {
        type = AP_ZIT_RADIO_ELEMENTS_RADIOPOWER;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class RadioChanneltlv : public Element
{
public:
    unsigned char RadioId;
    unsigned short Channel;
public:
    RadioChanneltlv()
    {
        type = AP_ZIT_RADIO_ELEMENTS_RADIOCHANNEL;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class RadioChainMasktlv : public Element
{
public:
    unsigned char RadioId;
    unsigned char ChainMask;
public:
    RadioChainMasktlv()
    {
        type = AP_ZIT_RADIO_ELEMENTS_CHAINMASK;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class RadioAmpdutlv : public Element
{
public:
    unsigned char RadioId;
    unsigned char AMPDUEnable;
public:
    RadioAmpdutlv()
    {
        type = AP_ZIT_RADIO_ELEMENTS_AMPDU;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class RadioBandWidthtlv : public Element
{
public:
    unsigned char RadioId;
    unsigned char BandWidth;
public:
    RadioBandWidthtlv()
    {
        type = AP_ZIT_RADIO_ELEMENTS_BANDWIDTH;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class RadioShortGItlv : public Element
{
public:
    unsigned char RadioId;
    unsigned char ShortGI;
public:
    RadioShortGItlv()
    {
        type = AP_ZIT_RADIO_ELEMENTS_SHORTGI;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class RadioAmsdutlv : public Element
{
public:
    unsigned char RadioId;
    unsigned char AMSDUEnable;
public:
    RadioAmsdutlv()
    {
        type = AP_ZIT_RADIO_ELEMENTS_AMSDU;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class RadioBeaconIntetlv : public Element
{
public:
    unsigned char RadioId;
    unsigned short BeaconInte;
public:
    RadioBeaconIntetlv()
    {
        type = AP_ZIT_RADIO_ELEMENTS_BEACONINTE;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class RadioDTIMIntetlv : public Element
{
public:
    unsigned char RadioId;
    unsigned char DTIMInte;
public:
    RadioDTIMIntetlv()
    {
        type = AP_ZIT_RADIO_ELEMENTS_DTIMINTE;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class RadioSliceThresholdtlv : public Element
{
public:
    unsigned char RadioId;
    unsigned short SliceThreshold;
public:
    RadioSliceThresholdtlv()
    {
        type = AP_ZIT_RADIO_ELEMENTS_SLICETHRESHOLD;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class RadioRtsThresholdtlv : public Element
{
public:
    unsigned char RadioId;
    unsigned short RtsThreshold;
public:
    RadioRtsThresholdtlv()
    {
        type = AP_ZIT_RADIO_ELEMENTS_RTSTHRESHOLD;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class RadioShortPreambletlv : public Element
{
public:
    unsigned char RadioId;
    unsigned char ShortPreamble;
public:
    RadioShortPreambletlv()
    {
        type = AP_ZIT_RADIO_ELEMENTS_SHORTPREAMBLE;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class RadioCountrytlv : public Element
{
public:
    unsigned char RadioId;
    unsigned char Country[2];
public:
    RadioCountrytlv()
    {
        type = AP_ZIT_RADIO_ELEMENTS_COUNTRY;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};















class SetResulttlv : public Element
{
public:
    unsigned char result;
public:
    SetResulttlv()
    {
        type = AP_ZIT_RESPONSE_ELEMENTS_RESULT;
        result = 0;
        valid = true;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class SetTunnelInfotlv : public Element
{
public:
#ifdef i386
    union {
        struct {
            unsigned char tunneltype:4;
            unsigned char reverse:3;
            unsigned char iptype:1;
        }s;
        unsigned char tunnelconf;
    }u;

#else
    union {
        struct {
            unsigned char iptype:1;
            unsigned char reverse:3;
            unsigned char tunneltype:4;
        }s;
        unsigned char tunnelconf;
    }u;
#endif
    unsigned char tunnelMode;
    unsigned char ipaddr[4];
    unsigned short portnumber;
public:
    SetTunnelInfotlv()
    {
        type = AP_ZIT_CHANGETUNNEL_ELEMENTS_TUNNELINFO;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class SwitchBackupTunneltlv : public Element
{
public:
#ifdef i386
    union {
        struct {
            unsigned char tunneltype:4;
            unsigned char reverse:3;
            unsigned char iptype:1;
        }s;
        unsigned char tunnelconf;
    }u;

#else
    union {
        struct {
            unsigned char iptype:1;
            unsigned char reverse:3;
            unsigned char tunneltype:4;
        }s;
        unsigned char tunnelconf;
    }u;
#endif
    unsigned char tunnelMode;
    unsigned char ipaddr[4];
    unsigned short portnumber;
public:
    SwitchBackupTunneltlv()
    {
        type = AP_ZIT_CHANGETUNNEL_ELEMENTS_SWITCHBACKUP;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class Generic5G11NPrioritytlv : public Element
{
public:
    unsigned char enable;
public:
    Generic5G11NPrioritytlv()
    {
        type = AP_ZIT_GENERICCONFIG_5GPRIORITY;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class GenericTunnelSliceThresholdtlv : public Element
{
public:
    unsigned short threshold;
public:
    GenericTunnelSliceThresholdtlv()
    {
        type = AP_ZIT_GENERICCONFIG_TUNNELSLICETHRESHOLD;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class GenericStationAgetlv : public Element
{
public:
    unsigned short stationAgeing;
public:
    GenericStationAgetlv()
    {
        type = AP_ZIT_GENERICCONFIG_STATIONAGE;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class Generic5G11NPriorityNewtlv : public Element
{
public:
    unsigned char enable5GPririty;
    unsigned char limitedNum;
    unsigned short reserve;

public:
    Generic5G11NPriorityNewtlv()
    {
        type = AP_ZIT_GENERICCONFIG_5GPRIORITYNEW;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class UpdatewlanTunnelIDtlv : public Element
{
public:
    unsigned char RadioId;
    unsigned char WlanId;
    unsigned char DataTunnelMode;
public:
    UpdatewlanTunnelIDtlv()
    {
        type = AP_ZIT_UPDATEWLAN_ELEMENTS_TUNNELID;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class UpdatewlanUpflowctltlv: public Element
{
public:
    unsigned char RadioId;
    unsigned char WlanId;
    unsigned int SsidUpFlow;
    unsigned int UserUpFlow;
public:
    UpdatewlanUpflowctltlv()
    {
        type = AP_ZIT_UPDATEWLAN_ELEMENTS_UPFLOW;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class UpdatewlanEnableWlantlv: public Element
{
public:
    unsigned char RadioId;
    unsigned char WlanId;
    unsigned char Enable;
public:
    UpdatewlanEnableWlantlv()
    {
        type = AP_ZIT_UPDATEWLAN_ELEMENTS_WLANENABLE;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class UpdatewlanIsolatetlv : public Element
{
public:
    unsigned char RadioId;
    unsigned char WlanId;
#ifdef i386
    union {
        struct {
            unsigned char reverse:4;
            unsigned char userisolate:1;
            unsigned char mcastisolate:1;
            unsigned char bcastisolate:1;
            unsigned char allisolate:1;
        }s;
        unsigned char isolate;
    }u;

#else
    union {
        struct {
            unsigned char allisolate:1;
            unsigned char bcastisolate:1;
            unsigned char mcastisolate:1;
            unsigned char userisolate:1;
            unsigned char reverse:4;
        }s;
        unsigned char isolate;
    }u;
#endif
public:
    UpdatewlanIsolatetlv()
    {
        type = AP_ZIT_UPDATEWLAN_ELEMENTS_ETHERISOLAATION;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class UpdatewlanLocalSsidtlv : public Element
{
public:
    unsigned char RadioId;
    unsigned char WlanId;
    unsigned char Enable;
    unsigned short SsidLength;
    string Ssid;
public:
    UpdatewlanLocalSsidtlv()
    {
        type = AP_ZIT_UPDATEWLAN_ELEMENTS_LOCALESSID;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class UpdatewlanPortalUrltlv : public Element
{
public:
    unsigned char RadioId;
    unsigned char WlanId;
    string url;
public:
    UpdatewlanPortalUrltlv()
    {
        type = AP_ZIT_UPDATEWLAN_ELEMENTS_SSIDPORTLURL;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class KeyEventsHotReboot : public Element
{
public:
    unsigned short reason;
public:
    KeyEventsHotReboot()
    {
        type = AP_ZIT_KEYEVENTS_ELEMENTS_HOTREBOOT;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class KeyEventsRadioUpdown : public Element
{
public:
    unsigned char RadioId;
    unsigned char RadioState;
    unsigned short reason;
public:
    KeyEventsRadioUpdown()
    {
        type = AP_ZIT_KEYEVENTS_ELEMENTS_RADIOUPDOWN;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class KeyEventsWlanDelDown : public Element
{
public:
    unsigned char RadioId;
    unsigned char WlanId;
    unsigned short reason;
public:
    KeyEventsWlanDelDown()
    {
        type = AP_ZIT_KEYEVENTS_ELEMENTS_WLANDELDOWN;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class KeyEventsWlanUp : public Element
{
public:
    unsigned char RadioId;
    unsigned char WlanId;
    unsigned char BSSID[6];
public:
    KeyEventsWlanUp()
    {
        type = AP_ZIT_KEYEVENTS_ELEMENTS_WLANUP;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class ACLoginResultIPV4 : public Element
{
public:
    unsigned short result;
    unsigned short errno;
    unsigned char ipaddr[4];

public:
    ACLoginResultIPV4()
    {
        type = AP_ZIT_ACLOGIN_ELEMENTS_IPV4;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class SetSnmpGlobalInteval : public Element
{
public:
    unsigned short inteval;
public:
    SetSnmpGlobalInteval()
    {
        type = AP_ZIT_SETSNMP_ELEMENTS_GLOBALINTE;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class SetSnmpTrapKey : public Element
{
public:
    string key;
public:
    SetSnmpTrapKey()
    {
        type = AP_ZIT_SETSNMP_ELEMENTS_TRAPKEY;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class SetSnmpReadKey : public Element
{
public:
    string key;
public:
    SetSnmpReadKey()
    {
        type = AP_ZIT_SETSNMP_ELEMENTS_READKEY;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class SetSnmpWriteKey : public Element
{
public:
    string key;
public:
    SetSnmpWriteKey()
    {
        type = AP_ZIT_SETSNMP_ELEMENTS_WRITEKEY;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class SetSnmpAlarmsSwitch : public Element
{
public:
    struct __alarms
    {
        unsigned short alarmId;
        unsigned short enable;
        unsigned short threshold;
    };

    unsigned short alarms_count;
    vector<__alarms> alarms;
public:
    SetSnmpAlarmsSwitch()
    {
        type = AP_ZIT_SETSNMP_ELEMENTS_ALARMSWITCH;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class SystemMaintenancetlv : public Element
{
public:
    unsigned char operation;
public:
    SystemMaintenancetlv()
    {
        type = AP_ZIT_MAINTENANCE_ELEMENTS_SYSTEMOPERATOR;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class StaAssosicMacaddrtlv : public Element
{
public:
    unsigned char macaddr[6];
public:
    StaAssosicMacaddrtlv()
    {
        type = AP_ZIT_STAASSOSICINFO_STAMACADDR;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};

class StaAssosicIpv4addrtlv : public Element
{
public:
    unsigned char ipaddr[4];
public:
    StaAssosicIpv4addrtlv()
    {
        type = AP_ZIT_STAASSOSICINFO_STAIPV4ADDR;
    }

    int parse(buffer *buf);
    int assemble(buffer *buf);
    int saveToCM(string &sMsg);
    int loadFromCM(kvlist &kv);
};





#endif
