#ifndef __PROTO_DEFS_H__
#define __PROTO_DEFS_H__

enum AP_ZIT_MESSAGE_TYPE
{
    AP_ZIT_SCANARGS_REQUEST = 7,
    AP_ZIT_SCANRESULT_REQUEST = 9,
    AP_ZIT_ADDWLAN_REQUEST = 18,
    AP_ZIT_ADDWLAN_RESPONSE = 23,
    AP_ZIT_WORKMODE_REQUEST = 28,
    AP_ZIT_WORKMODE_RESPONSE = 29,
    AP_ZIT_DELWLAN_REQUEST = 32,
    AP_ZIT_DELWLAN_RESPONSE = 33,
    AP_ZIT_ADDBLACK_REQUEST = 36,
    AP_ZIT_ADDBLACK_RESPONSE = 37,
    AP_ZIT_DELBLACK_REQUEST = 38,
    AP_ZIT_DELBLACK_RESPONSE = 39,
    AP_ZIT_UPGRADE_REQUEST = 44,
    AP_ZIT_UPGRADE_RESPONSE = 45,
    AP_ZIT_SETAS_REQUEST = 46,
    AP_ZIT_SETAS_RESPONSE = 47,
    AP_ZIT_STAINFO_REQUEST = 32001,
};
enum AP_ZIT_EXT_MESSAGE_TYPE
{
    AP_ZIT_LINKINTEGRITY_REQUEST = 65537,
    AP_ZIT_LINKINTEGRITY_RESPONSE = 65538,
    AP_ZIT_SETNTP_REQUEST = 65539,
    AP_ZIT_SETNTP_RESPONSE = 65540,
    AP_ZIT_AUTOCHANNEL_REQUEST = 65541,
    AP_ZIT_AUTOCHANNEL_RESPONSE = 65542,
    AP_ZIT_IGMPSNOOPING_REQUEST = 65543,
    AP_ZIT_IGMPSNOOPING_RESPONSE = 65544,
    AP_ZIT_LOADBALANCE_REQUEST = 65545,
    AP_ZIT_LOADBALANCE_RESPONSE = 65546,
    AP_ZIT_ISOLATION_REQUEST = 65547,
    AP_ZIT_ISOLATION_RESPONSE = 65548,
    AP_ZIT_MAINTENANCE_REQUEST = 65551,
    AP_ZIT_MAINTENANCE_RESPONSE = 65552,
    AP_ZIT_SETRADIO_REQUEST = 65553,
    AP_ZIT_SETRADIO_RESPONSE = 65554,
    AP_ZIT_SETSTASTATE_REQUEST = 65555,
    AP_ZIT_SETSTASTATE_RESPONSE = 65556,
    AP_ZIT_CHANGETUNNEL_REQUEST = 65557,
    AP_ZIT_CHANGETUNNEL_RESPONSE = 65558,
    AP_ZIT_SEARCHAPCOMM_REQUEST = 65559,
    AP_ZIT_SEARCHAPCOMM_RESPONSE = 65560,
    AP_ZIT_GENERICCONFIG_REQUEST = 65561,
    AP_ZIT_GENERICCONFIG_RESPONSE = 65562,
    AP_ZIT_UPDATEWLAN_REQUEST = 65563,
    AP_ZIT_UPDATEWLAN_RESPONSE = 65564,
    AP_ZIT_ROMINGCONFIG_REQUEST = 65566,
    AP_ZIT_ROMINGCONFIG_RESQPONSE = 65567,
    AP_ZIT_APSTATE_REQUEST = 65568,
    AP_ZIT_ECHO_REQUEST = 65569,
    AP_ZIT_ECHO_RESPONSE = 65570,
    AP_ZIT_WIRELESSSTATISTICS_REQUEST = 65571,
    AP_ZIT_USERSTATISTICS_REQUEST = 65572,
    AP_ZIT_CRITICALEVENT_REQUEST = 65573,
    AP_ZIT_ACSEARCHRESULT_REQUST = 65574,
    AP_ZIT_ACLOGIN_REQUEST = 65575,
    AP_ZIT_SETRADIUS_REQUEST = 65576,
    AP_ZIT_SETRADIUS_RESPONSE = 65577,
    AP_ZIT_DELDYNAMICBLACK_REQUEST = 65578,
    AP_ZIT_DELDYNAMICBLACK_RESPONSE = 65579,
    AP_ZIT_SEARCHAP_REQUEST = 65580,
    AP_ZIT_SEARCHAP_RESPONSE = 65581,
    AP_ZIT_SETSECUREARGS_REQUEST = 65582,
    AP_ZIT_SETSECUREARGS_RESPONSE = 65583,
    AP_ZIT_SETSNMP_REQUEST = 65584,
    AP_ZIT_SETSNMP_RESPONSE = 65585,
    AP_ZIT_LANSTATISTICS_REQUEST = 65586,
    AP_ZIT_VIRTUALAPSTATISTICS_REQUEST = 65587,
    AP_ZIT_STASYNC_REQUEST = 65588,
    AP_ZIT_MESHCHILDJOIN_REQUEST = 65589,
    AP_ZIT_MESHCHILDJOIN_RESPONSE = 65590,
    AP_ZIT_MESHCHILDAUTHRESULT_REQUEST = 65591,
    AP_ZIT_MESHCHILDAUTHRESULT_RESPONSE = 65592,
    AP_ZIT_MESHGLOBLECONFIG_REQUEST = 65593,
    AP_ZIT_MBSSGLOBLECONFIG_REQUEST = 65594,
    AP_ZIT_MESHLOCALARGS_REQUEST = 65595,
    AP_ZIT_MESHLOCALWIRELESSARGS_REQUEST = 65596,
    AP_ZIT_MESHTOPOLOGYCHANGE_REQUEST = 65597,
    AP_ZIT_STARATEPACKAGES_REQUEST = 65606,
    AP_ZIT_STAASSOSICINFO_REQUEST = 65607,
    AP_ZIT_SETLOGREPORT_REQUEST = 65608,
    AP_ZIT_SETLOGREPORT_RESPONSE = 65609,
    AP_ZIT_INTELOCALFLOW_REQUEST = 65650,
    AP_ZIT_STASASSOSICINFO_REQUEST = 65651,

};

#define AP_COMM_PORT 9008
#define AP_COMM_ADDR "127.0.0.1"
#define AP_PORT 9007

#define AP_WIFI0  1
#define AP_WIFI1  2

#define FIRST_ECHO_RELAY 5
#define ECHO_TIMEOUT 60
#define ECHO_TIMERID 1000
#define MONITOR_TIMERID 1001

#define MESSAGE_TYPE(message) \
    (ntohl(*(unsigned int*)(message)))

int apzit_send_msg(const char *buf, size_t length);

class buffer;

class ZCtrlHdr
{
public:
    unsigned int type;
    unsigned int length;

public:
    ZCtrlHdr():length(0)
    {
    }
    virtual ~ZCtrlHdr(){}

    virtual int parse(buffer *buf);
    virtual int saveToCM(string& sMsg);
    virtual int assemble(buffer *buf);
    virtual int loadFromCM(kvlist& kv);

    int skipheader(buffer *buf);

    int assembleHeader(buffer *buf);

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

    int GetType()
    {
        return type;
    }
};

class ZCtrlMsg : public ZCtrlHdr
{
    bool valid;
public:
    bool isValid()
    {
        return valid;
    }
    void setValid(bool val)
    {
        valid = val;
    }
};

class SecureInfo_Wapipsk
{
public:
    // wapi-psk
    unsigned char WAPISecureType;
    unsigned char WAPIShareKeyType;
    unsigned char WAPIShareKeyLength;
    string key;

public:
    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);

};
class SecureInfo_WapiCert
{
public:
    //wapi Certificate
    unsigned char WAPICertSecureType;

public:
    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};
class SecureInfo_Wepsk
{
public:
    // ieee80211i wep-sk
    unsigned char WEPSKSecureLen;
    unsigned char WEPSKType;
    unsigned char WEPSKKeyNo;
    unsigned char WEPSKKey1Len;
    string WEPSKKey1;
    unsigned char WEPSKKey2Len;
    string WEPSKKey2;
    unsigned char WEPSKKey3Len;
    string WEPSKKey3;
    unsigned char WEPSKKey4Len;
    string WEPSKKey4;

public:
    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};
class SecureInfo_eap
{
public:
    // ieee80211i wpa/wpa2(eap),
    // wpa(eap),wpa2(eap)
    unsigned char EAPEncryptionType;

public:
    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};
class SecureInfo_wpa_wpa2
{
public:
    // ieee80211i wpa/wpa2-psk
    unsigned char WPAPSKEncryptionType;
    unsigned char WPAPSKKeyType;
    unsigned char WPAPSKShareKeyLen;
    string WPAPSKKey;

public:
    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};
class SecureInfo_wpa_o_wpa2
{
public:
    // ieee80211 wpa-psk or wpa2-psk
    unsigned char WPA2PSKEncryptionType;
    unsigned char WPA2PSKShareKeyLen;
    string WPA2PSKKey;

public:
    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class SetScanRequest : public ZCtrlMsg
{
public:
    unsigned char RadioId;
    unsigned char ScanChannel;
    unsigned char ScanMode;
    unsigned char Rev;
    unsigned short ScanIntv;
public:
    SetScanRequest()
    {
        type = AP_ZIT_SCANARGS_REQUEST;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class SetScanResultRequest : public ZCtrlMsg
{
    struct __info
    {
        unsigned char RadioId;
        unsigned char Channel;
        char Rssi;
        char Snr;
#ifdef i386
        union{
            unsigned char RType;
            struct {
                unsigned char bAdHoc:3;
                unsigned char Rev:1;
                unsigned char Type:4;
            }s;
        }u;

#else
        union{
            unsigned char RType;
            struct {
                unsigned char Type:4;
                unsigned char Rev:1;
                unsigned char bAdHoc:3;
            }s;
        }u;
#endif
        unsigned char Rev;
        unsigned int DataRate;
        unsigned char BSSID[6];
        unsigned char MAC[6];
        unsigned short BeaconIntv;
        unsigned char KeyType;
        unsigned char SSIDlength;
        string Ssid;
    };
public:
    unsigned short count;
    unsigned short rev;

    __info infos[32];

public:
    SetScanResultRequest():count(0),rev(0)
    {
        type = AP_ZIT_SCANRESULT_REQUEST;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);

    int loadFromCMPtr(void *ptr);
};

class AddWlanRequest : public ZCtrlMsg
{
public:
    unsigned char RadioId;
    unsigned char WlanId;
    unsigned char HideSSID;
    unsigned char SecureModel;
    unsigned char SecureStrategy;
    unsigned char MaxUser;
    unsigned char Qos;
    unsigned char MacFilterRule;
    unsigned short Ability;
    unsigned short VlanId;
    unsigned char MacModel;
    unsigned char FrameEncap;
    unsigned int SsidDownTran;
    unsigned int UserDownTran;
    unsigned char SsidLength;
    string ssid;

    // 加密相关
    SecureInfo_Wapipsk wapipsk;
    SecureInfo_WapiCert wapicert;
    SecureInfo_Wepsk wepsk;
    SecureInfo_eap eap;
    SecureInfo_wpa_wpa2 wpa_wpa2;
    SecureInfo_wpa_o_wpa2 wpa_o_wpa2;
public:
    AddWlanRequest()
    {
        type = AP_ZIT_ADDWLAN_REQUEST;
    }
    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class AddWlanResponse : public ZCtrlMsg
{
    struct __result{
        unsigned char radioId;
        unsigned char wlanId;
        unsigned char result;
        unsigned char BSSID[6];
    };
public:
    unsigned char result_count;
    vector<__result> results;

public:
    AddWlanResponse():results(1)
    {
        type = AP_ZIT_ADDWLAN_RESPONSE;
    }
    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class DelWlanRequest : public ZCtrlMsg
{
public:
    unsigned char RadioId;
    unsigned char WlanId;
public:
    DelWlanRequest()
    {
        type = AP_ZIT_DELWLAN_REQUEST;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class DelWlanResponse : public ZCtrlMsg
{
public:
    unsigned char RadioId;
    unsigned char WlanId;
    unsigned char result;
public:
    DelWlanResponse()
    {
        type = AP_ZIT_DELWLAN_RESPONSE;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class AddBlackListRequest : public ZCtrlMsg
{
public:
    unsigned char count;
    unsigned char macs[255][6];
public:
    AddBlackListRequest()
    {
        type = AP_ZIT_ADDBLACK_REQUEST;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class AddBlackListResponse : public ZCtrlMsg
{
public:
    unsigned char count;
    unsigned char macs[255][6];
    unsigned char result[255];
public:
    AddBlackListResponse()
    {
        type = AP_ZIT_ADDBLACK_RESPONSE;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class DelBlackListRequest : public ZCtrlMsg
{
public:
    // 条目数为0时表示全部删除
    unsigned char count;
    unsigned char macs[255][6];
public:
    DelBlackListRequest()
    {
        type = AP_ZIT_DELBLACK_REQUEST;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class DelBlackListResponse : public ZCtrlMsg
{
public:
    unsigned char count;
    unsigned char macs[255][6];
    unsigned char result[255];
public:
    DelBlackListResponse()
    {
        type = AP_ZIT_DELBLACK_RESPONSE;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class UpgradeRequest : public ZCtrlMsg
{
public:
    unsigned short ImageType;
    unsigned short IdentifierLength;
    unsigned char ImageIdentifier[32];

    image_protocoltlv Protocol;
    image_serveripv4tlv serveripv4;
    image_urltlv url;
    image_usernametlv username;
    image_passwdtlv passwd;

public:
    UpgradeRequest()
    {
        type = AP_ZIT_UPGRADE_REQUEST;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class UpgradeResponse : public ZCtrlMsg
{
public:
    unsigned char result;
public:
    UpgradeResponse()
    {
        type = AP_ZIT_UPGRADE_RESPONSE;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class StaEventInfoRequest : public ZCtrlMsg
{
public:
    unsigned char UserMac[6];
    unsigned char BSSID[6];
    unsigned char SecureType;
    unsigned int TimeStamp;
    unsigned char SsidLength;
    string Ssid;

    stainfo_qostlv qostlv;
    stainfo_msgtypetlv msgtypetlv;
    stainfo_wlanidtlv wlanidtlv;
    stainfo_radioidtlv radioidtlv;
    stainfo_leavereasontlv leavereasontlv;

public:
    StaEventInfoRequest()
    {
        type = AP_ZIT_STAINFO_REQUEST;
    }
    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

//以下为扩展消息元
class ExtMessage :public ZCtrlMsg
{
public:
    unsigned short version;
    unsigned char seq_number;
    unsigned char reverse;
public:
    ExtMessage():version(0),seq_number(0),reverse(0)
    {
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);

    int skipheader(buffer *buf)
    {
        buf->Skip(12);
        return 0;
    }
    int assembleHeader(buffer *buf)
    {
        buf->BackToHead();
        ExtMessage::assemble(buf);
        return 0;
    }
    void setSeqNumber(unsigned char SeqNumber)
    {
        seq_number = SeqNumber;
    }
};

class SetStationStateRequest : public ExtMessage
{
public:
    stastatus_addstation addstation;
    stastatus_delstation delstation;
    stastatus_vlanupdate vlanupdate;
    stastatus_statidupdate statidupdate;
    stastatus_useragent useragent;

public:
    SetStationStateRequest()
    {
        type = AP_ZIT_SETSTASTATE_REQUEST;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class SetStationStateResponse : public ExtMessage
{
public:
    SetResulttlv resulttlv;

public:
    SetStationStateResponse()
    {
        type = AP_ZIT_SETSTASTATE_RESPONSE;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class SetRadioRequest : public ExtMessage
{
public:
    RadioModelSettlv ModelSet;
    RadioPowertlv PowerSet;
    RadioChanneltlv ChannelSet;
    RadioChainMasktlv ChainMaskSet;
    RadioAmpdutlv AmpduSet;
    RadioBandWidthtlv BandWidthSet;
    RadioShortGItlv ShortGISet;
    RadioAmsdutlv AmsduSet;
    RadioBeaconIntetlv BeaconIntvSet;
    RadioDTIMIntetlv DtimIntvSet;
    RadioSliceThresholdtlv FragSet;
    RadioRtsThresholdtlv RtsSet;
    RadioShortPreambletlv ShortPreambleSet;
    RadioCountrytlv CountrySet;

public:
    SetRadioRequest()
    {
        type = AP_ZIT_SETRADIO_REQUEST;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class SetRadioResponse : public ExtMessage
{
public:
    SetResulttlv resulttlv;
public:
    SetRadioResponse()
    {
        type = AP_ZIT_SETRADIO_RESPONSE;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class SetLoadBalanceRequest : public ExtMessage
{
public:
public:
    SetLoadBalanceRequest()
    {
        type = AP_ZIT_LOADBALANCE_REQUEST;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class SetLoadBalanceResponse : public ExtMessage
{
public:
    SetResulttlv resulttlv;
public:
    SetLoadBalanceResponse()
    {
        type = AP_ZIT_LOADBALANCE_RESPONSE;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class SetDataTunnelRequest : public ExtMessage
{
public:
    SetTunnelInfotlv tuninfotlv;
    SwitchBackupTunneltlv switchbackuptlv;

public:
    SetDataTunnelRequest()
    {
        type = AP_ZIT_CHANGETUNNEL_REQUEST;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class SetDataTunnelResponse : public ExtMessage
{
public:
    SetResulttlv resulttlv;
public:
    SetDataTunnelResponse()
    {
        type = AP_ZIT_CHANGETUNNEL_RESPONSE;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class SystemMaintainRequest : public ExtMessage
{
public:
    SystemMaintenancetlv systemmaintenance;
public:
    SystemMaintainRequest()
    {
        type = AP_ZIT_MAINTENANCE_REQUEST;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class SystemMaintainResponse : public ExtMessage
{
public:
    SetResulttlv resulttlv;
public:
    SystemMaintainResponse()
    {
        type = AP_ZIT_MAINTENANCE_RESPONSE;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class GenericConfigRequest : public ExtMessage
{
public:
    Generic5G11NPrioritytlv Generic11ntlv;
    GenericTunnelSliceThresholdtlv tunnelSlicetlv;
    GenericStationAgetlv stationAgeingtlv;
    Generic5G11NPriorityNewtlv Generic11nNewtlv;
public:
    GenericConfigRequest()
    {
        type = AP_ZIT_GENERICCONFIG_REQUEST;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class GenericConfigResponse : public ExtMessage
{
public:
    SetResulttlv resulttlv;
public:
    GenericConfigResponse()
    {
        type = AP_ZIT_GENERICCONFIG_RESPONSE;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class UpdateWlanRequest : public ExtMessage
{
public:
    UpdatewlanTunnelIDtlv tunnelID;
    UpdatewlanUpflowctltlv upflowctl;
    UpdatewlanEnableWlantlv enableWlan;
    UpdatewlanIsolatetlv staisolate;
    UpdatewlanLocalSsidtlv localSsid;
    UpdatewlanPortalUrltlv portalurl;

public:
    UpdateWlanRequest()
    {
        type = AP_ZIT_UPDATEWLAN_REQUEST;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class UpdateWlanResponse : public ExtMessage
{
public:
    SetResulttlv resulttlv;
public:
    UpdateWlanResponse()
    {
        type = AP_ZIT_UPDATEWLAN_RESPONSE;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};



class EchoRequest : public ExtMessage
{
public:
    EchoRequest()
    {
        type = AP_ZIT_ECHO_REQUEST;
    }
};
class EchoResponse : public ExtMessage
{
public:
    EchoResponse()
    {
        type = AP_ZIT_ECHO_RESPONSE;
    }
};

class KeyEventsRequest : public ExtMessage
{
public:
    KeyEventsHotReboot hotreboot;
    KeyEventsRadioUpdown radioupdown;
    KeyEventsWlanDelDown wlandeldown;
    KeyEventsWlanUp wlanup;
public:
    KeyEventsRequest()
    {
        type = AP_ZIT_CRITICALEVENT_REQUEST;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class ACLoginRequest : public ExtMessage
{
public:
    ACLoginResultIPV4 acipv4;
public:
    ACLoginRequest()
    {
        type = AP_ZIT_ACLOGIN_REQUEST;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class SetSnmpRequest : public ExtMessage
{
public:
    SetSnmpGlobalInteval ginterval;
    SetSnmpTrapKey trapkey;
    SetSnmpReadKey readkey;
    SetSnmpWriteKey writekey;
    SetSnmpAlarmsSwitch alarms_switch;

public:
    SetSnmpRequest()
    {
        type = AP_ZIT_SETSNMP_REQUEST;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class SetSnmpResponse : public ExtMessage
{
public:
    SetResulttlv resulttlv;

public:
    SetSnmpResponse()
    {
        type = AP_ZIT_SETSNMP_RESPONSE;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};

class StaAssosicInfoRequest : public ExtMessage
{
public:
    StaAssosicMacaddrtlv macaddrtlv;
    StaAssosicIpv4addrtlv ipaddrtlv;
public:
    StaAssosicInfoRequest()
    {
        type = AP_ZIT_STAASSOSICINFO_REQUEST;
    }

    int parse(buffer *buf);
    int saveToCM(string& sMsg);
    int assemble(buffer *buf);
    int loadFromCM(kvlist& kv);
};










int epoll_add_udp_server(int epoll_fd);
int echo_timeout_func(void *arg);
int echo_request_send(void *arg);
int monitor_info(void *arg);

ZCtrlMsg* create_zctrlmsg(int type);
ZCtrlMsg* create_zctrlmsg(buffer *buf);

#endif
