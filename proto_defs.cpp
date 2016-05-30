#include <iostream>
using namespace std;

#include <sys/epoll.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <syslog.h>
#include "platform.h"
#include "timer.h"
#include "buffer.h"
#include "stringunit.h"
#include "kvlist.h"
#include "proto_elements.h"
#include "proto_defs.h"
#include "iconv.h"
#include "interfaces.h"
#include "private_message.h"
#include "wp_common.h"

// int confSendMsgOnly(char *pbuf);
static int udpsockfd;
static int timeout_count;
static bool rebuild_wlans;

list<ZCtrlMsg *> wait_list;

int udp_listen()
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
    server.sin_port=htons(AP_PORT);
    server.sin_addr.s_addr= htonl (INADDR_ANY);
    if(bind(udpsockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("Bind()error.");
        return -1;
    }

    return udpsockfd;
}

int apzit_send_msg(const char *buf, size_t length)
{
    struct sockaddr_in apcomm_sin;

    bzero(&apcomm_sin,sizeof(apcomm_sin));
    apcomm_sin.sin_family=AF_INET;
    apcomm_sin.sin_port=htons(AP_COMM_PORT);
    apcomm_sin.sin_addr.s_addr = inet_addr(AP_COMM_ADDR);

    int ret = sendto(udpsockfd, buf, length,
                     0, (struct sockaddr*)&apcomm_sin, sizeof(apcomm_sin));
    cout << "send ===== " << ret << endl;
    syslog(LOG_DEBUG, "[%s:%d] send message [%d->%d] type %d length %d\n", __func__, __LINE__,
           AP_PORT, AP_COMM_PORT, MESSAGE_TYPE(buf), ret);

    return ret;
}

void * proto_callback(int fd, int events, void *arg)
{
    if (events & EPOLLIN)
    {
        buffer *buf = new buffer;
        int len = read(fd, buf->GetHead(), buf->GetSize());
        if (0 >= len)
        {
            return NULL;
        }
        buf->SetLength(len);

        ZCtrlMsg *ctlmsg = create_zctrlmsg(buf);
        if (NULL == ctlmsg)
        {
            delete buf;
            return NULL;
        }

        bool reverse_msg = false;  //Ê∂àÊÅØÊòØÂê¶ÈúÄË¶Å‰øùÁïôÊöÇ‰∏çÈáäÊîæ

        cout << "recv message length = "  << buf->GetLength()
            << ",type = " << ctlmsg->GetType() << endl;
        syslog(LOG_DEBUG, "[%s:%d] recv message [%d->%d] length %d type %d\n", __func__, __LINE__,
               AP_COMM_PORT, AP_PORT, buf->GetLength(), ctlmsg->GetType());

        if (0 > ctlmsg->parse(buf))
        {
            cout << "open message error type = " << ctlmsg->GetType() << endl;
            syslog(LOG_ERR, "[%s:%d] parse message %d error\n", __func__, __LINE__,
                   ctlmsg->type);

            delete buf;
            delete ctlmsg;
            return NULL;
        }

        switch((ctlmsg->GetType()))
        {
        case AP_ZIT_ECHO_RESPONSE:
            stop_timer(ECHO_TIMERID);
            add_timer(ECHO_TIMERID, ECHO_TIMEOUT, echo_request_send, NULL);
            break;
        case AP_ZIT_ADDWLAN_REQUEST:
            {
                AddWlanRequest *addwlan = (AddWlanRequest*) ctlmsg;
                string ifname = get_wlan_ifname(addwlan->RadioId, addwlan->WlanId);

                unsigned char BSSID[6] = {0};
                if (0 == GetIFHwAddr(ifname, BSSID, sizeof(BSSID)))
                {
                    AddWlanResponse *rsp = new AddWlanResponse;
                    buffer *rspbuf = new buffer;

                    rsp->result_count = 1;
                    rsp->results[0].radioId = addwlan->RadioId;
                    rsp->results[0].wlanId = addwlan->WlanId;
                    rsp->results[0].result = 0;
                    memcpy(rsp->results[0].BSSID, BSSID, sizeof(BSSID));

                    rsp->assemble(rspbuf);
                    apzit_send_msg(rspbuf->GetHead(), rspbuf->GetLength());
                    delete rspbuf;
                    delete rsp;
                }
                else
                {
                    wait_list.push_back(ctlmsg);
                    reverse_msg = true;
                }
#ifdef WLAN2G5GSAMECONFIG
                string ifname5G = get_wlan_ifname(addwlan->RadioId+1, addwlan->WlanId);
                if (0 == GetIFHwAddr(ifname5G, BSSID, sizeof(BSSID)))
                {
                    AddWlanResponse *rsp = new AddWlanResponse;
                    buffer *rspbuf = new buffer;

                    rsp->result_count = 1;
                    rsp->results[0].radioId = addwlan->RadioId + 1;
                    rsp->results[0].wlanId = addwlan->WlanId;
                    rsp->results[0].result = 0;
                    memcpy(rsp->results[0].BSSID, BSSID, sizeof(BSSID));

                    rsp->assemble(rspbuf);
                    apzit_send_msg(rspbuf->GetHead(), rspbuf->GetLength());
                    delete rspbuf;
                    delete rsp;
                }
#endif
                break;
            }
        case AP_ZIT_DELWLAN_REQUEST:
            {
                DelWlanRequest *delwlanreq = (DelWlanRequest*) ctlmsg;
                DelWlanResponse *delwlanrsp = new DelWlanResponse;
                delwlanrsp->RadioId = delwlanreq->RadioId;
                delwlanrsp->WlanId = delwlanreq->WlanId;
                delwlanrsp->result = 0;

                buffer *delwlanrspbuf = new buffer;
                delwlanrsp->assemble(delwlanrspbuf);
                apzit_send_msg(delwlanrspbuf->GetHead(), delwlanrspbuf->GetLength());
                delete delwlanrspbuf;
                delete delwlanrsp;
                break;
            }
        case AP_ZIT_ADDBLACK_REQUEST:
            {
                AddBlackListRequest *addblreq = (AddBlackListRequest*) ctlmsg;
                AddBlackListResponse *addblrsp = new AddBlackListResponse;
                addblrsp->count = addblreq->count;
                for (int i = 0; i< addblrsp->count; i++)
                {
                    bcopy(addblreq->macs[i], addblrsp->macs[i], 6);
                    addblrsp->result[i] = 0;
                }

                buffer *addblrspbuf = new buffer;
                addblrsp->assemble(addblrspbuf);
                apzit_send_msg(addblrspbuf->GetHead(), addblrspbuf->GetLength());
                delete addblrspbuf;
                delete addblrsp;
                break;
            }
        case AP_ZIT_DELBLACK_REQUEST:
            {
                DelBlackListRequest *delblreq = (DelBlackListRequest*) ctlmsg;
                DelBlackListResponse *delblrsp = new DelBlackListResponse;
                delblrsp->count = delblreq->count;
                for (int i=0; i<delblrsp->count; i++)
                {
                    bcopy(delblreq->macs[i], delblrsp->macs[i], 6);
                    delblrsp->result[i] = 0;
                }

                buffer *delblrspbuf = new buffer;
                delblrsp->assemble(delblrspbuf);
                apzit_send_msg(delblrspbuf->GetHead(), delblrspbuf->GetLength());
                delete delblrspbuf;
                delete delblrsp;
                break;
            }
        case AP_ZIT_UPGRADE_REQUEST:
            {
                UpgradeResponse *upgradersp = new UpgradeResponse;
                upgradersp->result = 0;

                buffer *upgraderspbuf = new buffer;
                upgradersp->assemble(upgraderspbuf);

                apzit_send_msg(buf->GetHead(), buf->GetLength());

                delete upgraderspbuf;
                delete upgradersp;
                break;
            }

        // ExtMessage  need response seq_number
        case AP_ZIT_SETSTASTATE_REQUEST:
            {
                SetStationStateResponse *rsp = new SetStationStateResponse;
                buffer *rspbuf = new buffer;

                ExtMessage *ext = (ExtMessage*)ctlmsg;
                rsp->setSeqNumber(ext->seq_number);

                rsp->assemble(rspbuf);
                apzit_send_msg(rspbuf->GetHead(), rspbuf->GetLength());

                delete rspbuf;
                delete rsp;
                break;
            }
        case AP_ZIT_SETRADIO_REQUEST:
            {
                SetRadioResponse *rsp = new SetRadioResponse;
                buffer *rspbuf = new buffer;

                ExtMessage *ext = (ExtMessage*)ctlmsg;
                rsp->setSeqNumber(ext->seq_number);

                rsp->assemble(rspbuf);
                apzit_send_msg(rspbuf->GetHead(), rspbuf->GetLength());

                delete rspbuf;
                delete rsp;
                break;
            }
        case AP_ZIT_LOADBALANCE_REQUEST:
            {
                SetLoadBalanceResponse *rsp = new SetLoadBalanceResponse;
                buffer *rspbuf = new buffer;

                ExtMessage *ext = (ExtMessage*)ctlmsg;
                rsp->setSeqNumber(ext->seq_number);

                rsp->assemble(rspbuf);
                apzit_send_msg(rspbuf->GetHead(), rspbuf->GetLength());

                delete rspbuf;
                delete rsp;
                break;
            }
        case AP_ZIT_MAINTENANCE_REQUEST:
            {
                SystemMaintainResponse *rsp = new SystemMaintainResponse;
                buffer * rspbuf = new buffer;

                ExtMessage *ext = (ExtMessage*)ctlmsg;
                rsp->setSeqNumber(ext->seq_number);

                rsp->assemble(rspbuf);
                apzit_send_msg(rspbuf->GetHead(), rspbuf->GetLength());

                delete rspbuf;
                delete rsp;
                break;
            }

        case AP_ZIT_CHANGETUNNEL_REQUEST:
            {
                SetDataTunnelResponse *rsp = new SetDataTunnelResponse;
                buffer *rspbuf = new buffer;

                ExtMessage *ext = (ExtMessage*)ctlmsg;
                rsp->setSeqNumber(ext->seq_number);

                rsp->assemble(rspbuf);
                apzit_send_msg(rspbuf->GetHead(), rspbuf->GetLength());

                delete rspbuf;
                delete rsp;
                break;
            }
        case AP_ZIT_GENERICCONFIG_REQUEST:
            {
                GenericConfigResponse *rsp = new GenericConfigResponse;
                buffer *rspbuf = new buffer;

                ExtMessage *ext = (ExtMessage*)ctlmsg;
                rsp->setSeqNumber(ext->seq_number);

                rsp->assemble(rspbuf);
                apzit_send_msg(rspbuf->GetHead(), rspbuf->GetLength());

                delete rspbuf;
                delete rsp;
                break;
            }
        case AP_ZIT_UPDATEWLAN_REQUEST:
            {
                UpdateWlanResponse *rsp = new UpdateWlanResponse;
                buffer *rspbuf = new buffer;

                ExtMessage *ext = (ExtMessage*)ctlmsg;
                rsp->setSeqNumber(ext->seq_number);

                rsp->assemble(rspbuf);
                apzit_send_msg(rspbuf->GetHead(), rspbuf->GetLength());

                delete rspbuf;
                delete rsp;
                break;
            }
        case AP_ZIT_SETSNMP_REQUEST:
            {
                SetSnmpResponse *rsp = new SetSnmpResponse;
                buffer *rspbuf = new buffer;

                ExtMessage *ext = (ExtMessage*)ctlmsg;
                rsp->setSeqNumber(ext->seq_number);

                rsp->assemble(rspbuf);
                apzit_send_msg(rspbuf->GetHead(), rspbuf->GetLength());

                delete rspbuf;
                delete rsp;
                break;
            }
        default:
            cout << "type " << ctlmsg->GetType() <<
                " don't need response." << endl;
            syslog(LOG_DEBUG, "[%s:%d] %d message don't need response\n", __func__, __LINE__,
                   ctlmsg->type);
            break;
        }

        string sMsg = "";
        ctlmsg->saveToCM(sMsg);

        if ("" != sMsg)
        {
            //send to cm
            //confSendMsgOnly((char*)sMsg.c_str());
            cout << sMsg << endl;
            syslog(LOG_DEBUG, "[%s:%d] send to cm :\n%s\n", __func__, __LINE__,
                   sMsg.c_str());
        }

        if (!reverse_msg)
            delete ctlmsg;
        delete buf;
    }

    return 0;
}

int epoll_add_udp_server(int epoll_fd)
{
    int ret = -1;
    int fd = udp_listen();
    if (0 > fd)
    {
        return -1;
    }
    struct epoll_event ev;
    static struct epoll_callback ec;
    ec.func = proto_callback;
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

int echo_timeout_func(void *arg)
{
    timeout_count++;

    // ‰∏ÄÊó¶Âá∫Áé∞echoË∂ÖÊó∂ÔºåÂÜçÊ¨°‰∏ãÂèëÊ∑ªÂä†wlan0‰πãÂâçÔºåÂÖ®ÈÉ®Âà†Èô§ÊâÄÊúâwlan
    rebuild_wlans = true;

    cout << "echo timeout : " << timeout_count << endl;
    syslog(LOG_ERR, "[%s:%d] echo timeout %d times", __func__, __LINE__,
           timeout_count);
    if (timeout_count >= 3)
    {
        cout << "restart apcomm" << endl;
        syslog(LOG_ERR, "[%s:%d] echo timeout %d times, need restart apcomm", __func__, __LINE__,
               timeout_count);

        system("killall -9 apcomm");
        timeout_count = 0;
    }
    echo_request_send(arg);
    return 0;
}
int echo_request_send(void *arg)
{
    buffer *buf = new buffer;
    EchoRequest *pechoreq = new EchoRequest;

    pechoreq->assemble(buf);
    apzit_send_msg(buf->GetHead(), buf->GetLength());

    add_timer(ECHO_TIMERID, ECHO_TIMEOUT, echo_timeout_func, NULL);

    delete pechoreq;
    delete buf;
    return 0;
}
int monitor_info(void *arg)
{
    cout << "wait_list elements count " << wait_list.size() << endl;
    for (list<ZCtrlMsg*>::iterator it = wait_list.begin();
         it != wait_list.end();
         it++)
    {
        cout << "wait_list element " << (*it)->GetType() << endl;
    }
    cout << endl;

    add_timer(MONITOR_TIMERID, ECHO_TIMEOUT, monitor_info, NULL);
    return 0;
}

ZCtrlMsg* create_zctrlmsg(unsigned int type)
{
    switch(type)
    {
    case AP_ZIT_SCANARGS_REQUEST:
        return new SetScanRequest;

    case AP_ZIT_SCANRESULT_REQUEST:
        return new SetScanResultRequest;

    case AP_ZIT_ADDWLAN_REQUEST:
        return new AddWlanRequest;

    case AP_ZIT_ADDWLAN_RESPONSE:
        return new AddWlanResponse;

    case AP_ZIT_DELWLAN_REQUEST:
        return new DelWlanRequest;

    case AP_ZIT_DELWLAN_RESPONSE:
        return new DelWlanResponse;

    case AP_ZIT_ADDBLACK_REQUEST:
        return new AddBlackListRequest;

    case AP_ZIT_ADDBLACK_RESPONSE:
        return new AddBlackListResponse;

    case AP_ZIT_DELBLACK_REQUEST:
        return new DelBlackListRequest;

    case AP_ZIT_DELBLACK_RESPONSE:
        return new DelBlackListResponse;

    case AP_ZIT_UPGRADE_REQUEST:
        return new UpgradeRequest;

    case AP_ZIT_UPGRADE_RESPONSE:
        return new UpgradeResponse;

    case AP_ZIT_STAINFO_REQUEST:
        return new StaEventInfoRequest;

    case AP_ZIT_SETSTASTATE_REQUEST:
        return new SetStationStateRequest;

    case AP_ZIT_SETSTASTATE_RESPONSE:
        return new SetStationStateResponse;

    case AP_ZIT_SETRADIO_REQUEST:
        return new SetRadioRequest;

    case AP_ZIT_SETRADIO_RESPONSE:
        return new SetRadioResponse;

    case AP_ZIT_LOADBALANCE_REQUEST:
        return new SetLoadBalanceRequest;

    case AP_ZIT_LOADBALANCE_RESPONSE:
        return new SetLoadBalanceResponse;

    case AP_ZIT_MAINTENANCE_REQUEST:
        return new SystemMaintainRequest;

    case AP_ZIT_MAINTENANCE_RESPONSE:
        return new SystemMaintainResponse;

    case AP_ZIT_CHANGETUNNEL_REQUEST:
        return new SetDataTunnelRequest;

    case AP_ZIT_CHANGETUNNEL_RESPONSE:
        return new SetDataTunnelResponse;

    case AP_ZIT_GENERICCONFIG_REQUEST:
        return new GenericConfigRequest;

    case AP_ZIT_GENERICCONFIG_RESPONSE:
        return new GenericConfigResponse;

    case AP_ZIT_UPDATEWLAN_REQUEST:
        return new UpdateWlanRequest;

    case AP_ZIT_UPDATEWLAN_RESPONSE:
        return new UpdateWlanResponse;

    case AP_ZIT_CRITICALEVENT_REQUEST:
        return new KeyEventsRequest;

    case AP_ZIT_ACLOGIN_REQUEST:
        return new ACLoginRequest;

    case AP_ZIT_SETSNMP_REQUEST:
        return new SetSnmpRequest;

    case AP_ZIT_SETSNMP_RESPONSE:
        return new SetSnmpResponse;

    case AP_ZIT_ECHO_REQUEST:
        return new EchoRequest;

    case AP_ZIT_ECHO_RESPONSE:
        return new EchoResponse;

    default:
        cout << "unknown type " << type << endl;
        syslog(LOG_DEBUG, "[%s:%d] %d message con't support\n", __func__, __LINE__, type);
        return NULL;
    }
    return NULL;
}

ZCtrlMsg* create_zctrlmsg(buffer *buf)
{
    unsigned int type = MESSAGE_TYPE(buf->GetHead());
    return create_zctrlmsg(type);
}

int ZCtrlHdr::skipheader(buffer *buf)
{
    buf->Skip(8);
    return 0;
}

int ZCtrlHdr::assembleHeader(buffer *buf)
{
    buf->BackToHead();
    ZCtrlHdr::assemble(buf);
    return 0;
}


int ZCtrlHdr::parse(buffer *buf)
{
    buf->retrieve32(type);
    buf->retrieve32(length);
    return 0;
}
int ZCtrlHdr::assemble(buffer *buf)
{
    buf->store32(type);
    buf->store32(length);
    return 0;
}
int ZCtrlHdr::saveToCM(string& sMsg)
{
    return 0;
}
int ZCtrlHdr::loadFromCM(kvlist& kv)
{
    return 0;
}

int SecureInfo_Wapipsk::parse(buffer *buf)
{
    buf->retrieve8(WAPISecureType);
    buf->retrieve8(WAPIShareKeyType);
    buf->retrieve8(WAPIShareKeyLength);
    key.assign(buf->GetPtr(), WAPIShareKeyLength);
    buf->retrieveRawBytes(NULL, WAPIShareKeyLength);
    return 0;
}
int SecureInfo_Wapipsk::assemble(buffer *buf)
{
    return 0;
}
int SecureInfo_Wapipsk::saveToCM(string& sMsg)
{
    return 0;
}
int SecureInfo_Wapipsk::loadFromCM(kvlist& kv)
{
    return 0;
}

int SecureInfo_WapiCert::parse(buffer *buf)
{
    buf->retrieve8(WAPICertSecureType);
    return 0;
}
int SecureInfo_WapiCert::assemble(buffer *buf)
{
    return 0;
}
int SecureInfo_WapiCert::saveToCM(string& sMsg)
{
    return 0;
}
int SecureInfo_WapiCert::loadFromCM(kvlist& kv)
{
    return 0;
}

int SecureInfo_Wepsk::parse(buffer *buf)
{
    buf->retrieve8(WEPSKSecureLen);
    buf->retrieve8(WEPSKType);
    buf->retrieve8(WEPSKKeyNo);

    buf->retrieve8(WEPSKKey1Len);
    WEPSKKey1.assign(buf->GetPtr(), WEPSKKey1Len);;
    buf->retrieveRawBytes(NULL, WEPSKKey1Len);

    buf->retrieve8(WEPSKKey2Len);
    WEPSKKey2.assign(buf->GetPtr(), WEPSKKey2Len);
    buf->retrieveRawBytes(NULL, WEPSKKey2Len);

    buf->retrieve8(WEPSKKey3Len);
    WEPSKKey3.assign(buf->GetPtr(), WEPSKKey3Len);
    buf->retrieveRawBytes(NULL, WEPSKKey3Len);

    buf->retrieve8(WEPSKKey4Len);
    WEPSKKey4.assign(buf->GetPtr(), WEPSKKey4Len);
    buf->retrieveRawBytes(NULL, WEPSKKey4Len);
    return 0;
}
int SecureInfo_Wepsk::assemble(buffer *buf)
{
    return 0;
}
int SecureInfo_Wepsk::saveToCM(string& sMsg)
{
    return 0;
}
int SecureInfo_Wepsk::loadFromCM(kvlist& kv)
{
    return 0;
}

int SecureInfo_eap::parse(buffer *buf)
{
    buf->retrieve8(EAPEncryptionType);
    return 0;
}
int SecureInfo_eap::assemble(buffer *buf)
{
    return 0;
}
int SecureInfo_eap::saveToCM(string& sMsg)
{
    return 0;
}
int SecureInfo_eap::loadFromCM(kvlist& kv)
{
    return 0;
}

int SecureInfo_wpa_wpa2::parse(buffer *buf)
{
    buf->retrieve8(WPAPSKEncryptionType);
    buf->retrieve8(WPAPSKKeyType);
    buf->retrieve8(WPAPSKShareKeyLen);
    WPAPSKKey.assign(buf->GetPtr(), WPAPSKShareKeyLen);;
    buf->retrieveRawBytes(NULL, WPAPSKShareKeyLen);
    return 0;
}
int SecureInfo_wpa_wpa2::assemble(buffer *buf)
{
    return 0;
}
int SecureInfo_wpa_wpa2::saveToCM(string& sMsg)
{
    return 0;
}
int SecureInfo_wpa_wpa2::loadFromCM(kvlist& kv)
{
    return 0;
}

int SecureInfo_wpa_o_wpa2::parse(buffer *buf)
{
    buf->retrieve8(WPA2PSKEncryptionType);
    buf->retrieve8(WPA2PSKShareKeyLen);
    WPA2PSKKey.assign(buf->GetPtr(), WPA2PSKShareKeyLen);;
    buf->retrieveRawBytes(NULL, WPA2PSKShareKeyLen);
    return 0;
}
int SecureInfo_wpa_o_wpa2::assemble(buffer *buf)
{
    return 0;
}
int SecureInfo_wpa_o_wpa2::saveToCM(string& sMsg)
{
    return 0;
}
int SecureInfo_wpa_o_wpa2::loadFromCM(kvlist& kv)
{
    return 0;
}

int SetScanRequest::parse(buffer *buf)
{
    ZCtrlHdr::parse(buf);
    buf->retrieve8(RadioId);
    buf->retrieve8(ScanChannel);
    buf->retrieve8(ScanMode);
    buf->retrieve8(Rev);
    buf->retrieve16(ScanIntv);
    return 0;
}
int SetScanRequest::saveToCM(string& sMsg)
{
    string value;

    scanRadioId = RadioId;
    sMsg += "modify /config.ini/we/CHANNEL_SET_2G=0x0;";
    sMsg += "modify /config.ini/we/CHANNEL_SET_5G=0x0;";

    if (0 != ScanMode)
    {
        value = "1";
    }
    else
    {
        value = "0";
    }
    sMsg += "modify /config.ini/we/EF_ENABLE=" + value + ";";
    int2string(ScanIntv, value);
    sMsg += "modify /config.ini/we/EF_INTV=" + value+ ";";

    sMsg += "modify /config.ini/we/EF_CODE=12;";
    sMsg += "modify /config.ini/we/EF_PRO=1;";
    sMsg += "modify /config.ini/we/EF_SERVERIP=127.0.0.1;";
    sMsg += "modify /config.ini/we/EF_SERVERPORT=18003;";
    sMsg += "modify /config.ini/we/EF_SCANTYPE=0;";

    return 0;
}
int SetScanRequest::assemble(buffer *buf)
{
    return 0;
}
int SetScanRequest::loadFromCM(kvlist& kv)
{
    return 0;
}

int SetScanResultRequest::parse(buffer *buf)
{
    return 0;
}
int SetScanResultRequest::saveToCM(string& sMsg)
{
    return 0;
}
int SetScanResultRequest::assemble(buffer *buf)
{
    skipheader(buf);

    buf->store16(count);
    length += 2;
    buf->store16(rev);
    length += 2;

    for (int i=0; i<count; ++i)
    {
        buf->store8(infos[i].RadioId);
        length += 1;
        buf->store8(infos[i].Channel);
        length += 1;
        buf->store8(infos[i].Rssi);
        length += 1;
        buf->store8(infos[i].Snr);
        length += 1;
        buf->store8(infos[i].u.RType);
        length += 1;
        buf->store8(infos[i].Rev);
        length += 1;
        buf->store32(infos[i].DataRate);
        length += 2;
        buf->storeRawBytes(infos[i].BSSID, 6);
        length += 6;
        buf->storeRawBytes(infos[i].MAC, 6);
        length += 6;
        buf->store16(infos[i].BeaconIntv);
        length += 2;
        buf->store8(infos[i].KeyType);
        length += 1;
        buf->store8(infos[i].SSIDlength);
        length += 1;
        buf->storeRawBytes((unsigned char *)infos[i].Ssid.c_str(), infos[i].SSIDlength);
        length += infos[i].SSIDlength;
    }

    assembleHeader(buf);
    return 0;
}
int SetScanResultRequest::loadFromCM(kvlist& kv)
{
    return 0;
}
int SetScanResultRequest::loadFromCMPtr(void *ptr)
{
    struct message_drive *results = (struct message_drive*)ptr;
    int i = count;

    infos[i].RadioId = scanRadioId;
    infos[i].Channel = results->channel;
    infos[i].Rssi = results->rssi - 95; // not sure
    infos[i].Snr = results->rssi;  //  not sure

    if (MI_STA == results->type)
        infos[i].u.s.Type = 1;
    else
        infos[i].u.s.Type = 2;

    infos[i].u.s.bAdHoc = 0;
    infos[i].Rev = 0;
    infos[i].DataRate = 0; // not sure
    memcpy(infos[i].BSSID, results->bssid, 6);
    memcpy(infos[i].MAC, results->macaddr, 6);

    printf("mac : %02x:%02x:%02x:%02x:%02x:%02x\n",
           results->macaddr[0],
           results->macaddr[1],
           results->macaddr[2],
           results->macaddr[3],
           results->macaddr[4],
           results->macaddr[5]);

    infos[i].BeaconIntv = 0; // not sure

    switch(results->keytype)
    {
    case OPEN:
        infos[i].KeyType = 0;
        break;
    case WEP:
        infos[i].KeyType = 1;
        break;
    case WPA_EAP_CCMP:
    case WPA_PSK_CCMP:
    case WPA2_PSK_CCMP:
    case WPA2_EAP_CCMP:
    case WPA_WPA2_CCMP:
    case WPA_EAP_BOTH:
    case WPA_PSK_BOTH:
    case WPA2_PSK_BOTH:
    case WPA2_EAP_BOTH:
    case WPA_WPA2_BOTH:
        infos[i].KeyType = 6;
        break;
    case WPA_EAP_TKIP:
    case WPA_PSK_TKIP:
    case WPA2_PSK_TKIP:
    case WPA2_EAP_TKIP:
    case WPA_WPA2_TKIP:
        infos[i].KeyType = 5;
        break;
    default:
        infos[i].KeyType = 0;
        break;
    }
    infos[i].SSIDlength = strlen((const char*)results->ssid);
    infos[i].Ssid.assign((const char *)results->ssid, infos[i].SSIDlength);

    count++;
    return 0;
}

int AddWlanRequest::parse(buffer *buf)
{
    ZCtrlHdr::parse(buf);
    buf->retrieve8(RadioId);
    buf->retrieve8(WlanId);
    buf->retrieve8(HideSSID); // 0--‰∏çÈöêËóè 1--ÈöêËóè
    buf->retrieve8(SecureModel); //1--wapi 2--80211i
    buf->retrieve8(SecureStrategy);
    buf->retrieve8(MaxUser);
    buf->retrieve8(Qos);
    buf->retrieve8(MacFilterRule);
    buf->retrieve16(Ability);
    buf->retrieve16(VlanId);
    buf->retrieve8(MacModel);
    buf->retrieve8(FrameEncap);
    buf->retrieve32(SsidDownTran);
    buf->retrieve32(UserDownTran);
    buf->retrieve8(SsidLength);
    ssid.assign(buf->GetPtr(), SsidLength);
    buf->retrieveRawBytes(NULL, SsidLength);

    if (SecureModel == 1)
    {
        switch(SecureStrategy)
        {
        case 0: break;
        case 1: wapipsk.parse(buf);     //wapi-psk
                break;
        case 2: wapicert.parse(buf);    //wapi ËØÅ‰π¶
                break;
        default:break;
        }
    }
    else if (SecureModel == 2)
    {
        switch(SecureStrategy)
        {
        case 0: break;

        case 9:                         // wep-sk
        case 1: wepsk.parse(buf);       // wep
                break;
        case 2:                         // wpa/wpa2-eap
        case 7:                         // wpa-eap
        case 8: eap.parse(buf);         // wpa2-eap
                break;
        case 3: wpa_wpa2.parse(buf);    // wpa/wpa2-psk
                break;
        case 4: break;                  // 11i ËØÅ‰π¶

        case 5:                         // wpa-psk
        case 6: wpa_o_wpa2.parse(buf);  // wpa2-psk
                break;
        default:break;
        }
    }

    return 0;
}
int AddWlanRequest::saveToCM(string& sMsg)
{
#if 0
    if (1 == WlanId && 1 == RadioId && rebuild_wlans)
    {
        char delall[] = "cmd /del_all_apwlans;";
        confSendMsgOnly(delall);
        rebuild_wlans = false;
    }
#endif


    string ifname = get_wlan_ifname(RadioId, WlanId);

    string value = "";
    int2string(WlanId, value);
    sMsg += "add /config.ini/" + ifname + "/WLAN_ID=" + value + ";";

#ifdef WLAN2G5GSAMECONFIG
    string ifname5G = get_wlan_ifname(RadioId+1, WlanId);
    sMsg += "add /config.ini/" + ifname5G + "/WLAN_ID=" + value + ";";
#endif

    int2string(Qos, value);
	sMsg += "modify /config.ini/" + ifname + "/QOS_ENABLE=" + value + ";";

#ifdef WLAN2G5GSAMECONFIG
	sMsg += "modify /config.ini/" + ifname5G + "/QOS_ENABLE=" + value + ";";
#endif

	sMsg += "modify /config.ini/" + ifname + "/ESSID=" + ssid + ";";

#ifdef WLAN2G5GSAMECONFIG
	sMsg += "modify /config.ini/" + ifname5G + "/ESSID=" + ssid + ";";
#endif
    int2string(HideSSID, value);
	sMsg += "modify /config.ini/" + ifname + "/HIDE_SSID=" + value + ";";

#ifdef WLAN2G5GSAMECONFIG
	sMsg += "modify /config.ini/" + ifname5G + "/HIDE_SSID=" + value + ";";
#endif
    int2string(MaxUser, value);
    sMsg += "modify /config.ini/" + ifname + "/MAX_STA=" + value + ";";

#ifdef WLAN2G5GSAMECONFIG
    sMsg += "modify /config.ini/" + ifname5G + "/MAX_STA=" + value + ";";
#endif
    int2string(MacFilterRule, value);
    sMsg += "modify /config.ini/" + ifname + "/MAC_CMD=" + value + ";";

#ifdef WLAN2G5GSAMECONFIG
    sMsg += "modify /config.ini/" + ifname5G + "/MAC_CMD=" + value + ";";
#endif
    int tmpvlanid = (VlanId==1)?0:VlanId;
    int2string(tmpvlanid, value);
    sMsg += "modify /config.ini/" + ifname + "/VLAN_ID=" + value + ";";;

#ifdef WLAN2G5GSAMECONFIG
    sMsg += "modify /config.ini/" + ifname5G + "/VLAN_ID=" + value + ";";;
#endif
    int2string(SsidDownTran, value);
    sMsg += "modify /config.ini/" + ifname + "/MAX_DOWN_TRAFFIC=" + value + ";";

#ifdef WLAN2G5GSAMECONFIG
    sMsg += "modify /config.ini/" + ifname5G + "/MAX_DOWN_TRAFFIC=" + value + ";";
#endif
    int2string(UserDownTran, value);
    sMsg += "modify /config.ini/" + ifname + "/USER_DOWN_BANDWIDTH=" + value + ";";

#ifdef WLAN2G5GSAMECONFIG
    sMsg += "modify /config.ini/" + ifname5G + "/USER_DOWN_BANDWIDTH=" + value + ";";
#endif
    sMsg += "modify /config.ini/" + ifname + "/WLAN_DEFAULT=0;";

#ifdef WLAN2G5GSAMECONFIG
    sMsg += "modify /config.ini/" + ifname5G + "/WLAN_DEFAULT=0;";
#endif
    if (SecureModel == 1)
    {
        switch(SecureStrategy)
        {
        case 0: //open
            	sMsg += "modify /config.ini/" + ifname + "/AUTH_MODE=0;";
#ifdef WLAN2G5GSAMECONFIG
            	sMsg += "modify /config.ini/" + ifname5G + "/AUTH_MODE=0;";
#endif
                break;
        case 1: //wapi-psk
	            sMsg += "modify /config.ini/" + ifname + "/AUTH_MODE=7;";
                sMsg += "modify /config.ini/" + ifname + "/SECURITY_CIPHERS=7;";
        	    sMsg += "modify /config.ini/" + ifname + "/KEY_WORD1=" + wapipsk.key + ";";
        	    sMsg += "modify /config.ini/" + ifname + "/KEY_WORD2=" + wapipsk.key + ";";

#ifdef WLAN2G5GSAMECONFIG
	            sMsg += "modify /config.ini/" + ifname5G + "/AUTH_MODE=7;";
                sMsg += "modify /config.ini/" + ifname5G + "/SECURITY_CIPHERS=7;";
        	    sMsg += "modify /config.ini/" + ifname5G + "/KEY_WORD1=" + wapipsk.key + ";";
        	    sMsg += "modify /config.ini/" + ifname5G + "/KEY_WORD2=" + wapipsk.key + ";";
#endif
                break;
        case 2: wapicert.saveToCM(sMsg);    //wapi ËØÅ‰π¶
                break;
        default:break;
        }
    }
    else if (SecureModel == 2)
    {
        switch(SecureStrategy)
        {
        case 0: //open
                sMsg += "modify /config.ini/" + ifname + "/AUTH_MODE=0;";
#ifdef WLAN2G5GSAMECONFIG
                sMsg += "modify /config.ini/" + ifname5G + "/AUTH_MODE=0;";
#endif
                break;

        case 9: // wep-sk
        case 1: // wep
                sMsg += "modify /config.ini/" + ifname + "/AUTH_MODE=1;";
                sMsg += "modify /config.ini/" + ifname + "/SECURITY_CIPHERS=0;";

#ifdef WLAN2G5GSAMECONFIG
                sMsg += "modify /config.ini/" + ifname5G + "/AUTH_MODE=1;";
                sMsg += "modify /config.ini/" + ifname5G + "/SECURITY_CIPHERS=0;";
#endif
                int2string(wepsk.WEPSKKeyNo, value);
                sMsg += "modify /config.ini/" + ifname + "/KEY_NO=" + value + ";";
                sMsg += "modify /config.ini/" + ifname + "/KEY_WORD1=" + wepsk.WEPSKKey1 + ";";
                sMsg += "modify /config.ini/" + ifname + "/KEY_WORD2=" + wepsk.WEPSKKey2 + ";";
                sMsg += "modify /config.ini/" + ifname + "/KEY_WORD3=" + wepsk.WEPSKKey3 + ";";
                sMsg += "modify /config.ini/" + ifname + "/KEY_WORD4=" + wepsk.WEPSKKey4 + ";";
#ifdef WLAN2G5GSAMECONFIG
                sMsg += "modify /config.ini/" + ifname5G + "/KEY_NO=" + value + ";";
                sMsg += "modify /config.ini/" + ifname5G + "/KEY_WORD1=" + wepsk.WEPSKKey1 + ";";
                sMsg += "modify /config.ini/" + ifname5G + "/KEY_WORD2=" + wepsk.WEPSKKey2 + ";";
                sMsg += "modify /config.ini/" + ifname5G + "/KEY_WORD3=" + wepsk.WEPSKKey3 + ";";
                sMsg += "modify /config.ini/" + ifname5G + "/KEY_WORD4=" + wepsk.WEPSKKey4 + ";";
#endif
                break;
        case 2: // wpa/wpa2-eap
        case 7:
                // wpa-eap
                sMsg += "modify /config.ini/" + ifname + "/AUTH_MODE=4;";
                sMsg += "modify /config.ini/" + ifname + "/AUTH_MODE=4;";

#ifdef WLAN2G5GSAMECONFIG
                sMsg += "modify /config.ini/" + ifname5G + "/AUTH_MODE=4;";
                sMsg += "modify /config.ini/" + ifname5G + "/AUTH_MODE=4;";
#endif
                break;
        case 8:
                // wpa2-eap
                sMsg += "modify /config.ini/" + ifname + "/AUTH_MODE=8;";
                sMsg += "modify /config.ini/" + ifname + "/SECURITY_CIPHERS=4;";

#ifdef WLAN2G5GSAMECONFIG
                sMsg += "modify /config.ini/" + ifname5G + "/AUTH_MODE=8;";
                sMsg += "modify /config.ini/" + ifname5G + "/SECURITY_CIPHERS=4;";
#endif
                break;
        case 3:
                // wpa/wpa2-psk
                sMsg += "modify /config.ini/" + ifname + "/AUTH_MODE=32;";
                sMsg += "modify /config.ini/" + ifname + "/SECURITY_CIPHERS=6;";
                sMsg += "modify /config.ini/" + ifname + "/KEY_WORD1=" + wpa_wpa2.WPAPSKKey + ";";
                sMsg += "modify /config.ini/" + ifname + "/KEY_WORD2=" + wpa_wpa2.WPAPSKKey + ";";

#ifdef WLAN2G5GSAMECONFIG
                sMsg += "modify /config.ini/" + ifname5G + "/AUTH_MODE=32;";
                sMsg += "modify /config.ini/" + ifname5G + "/SECURITY_CIPHERS=6;";
                sMsg += "modify /config.ini/" + ifname5G + "/KEY_WORD1=" + wpa_wpa2.WPAPSKKey + ";";
                sMsg += "modify /config.ini/" + ifname5G + "/KEY_WORD2=" + wpa_wpa2.WPAPSKKey + ";";
#endif
                break;
        case 4: break;                  // 11i ËØÅ‰π¶

        case 5:
                // wpa-psk
                sMsg += "modify /config.ini/" + ifname + "/AUTH_MODE=16;";
                sMsg += "modify /config.ini/" + ifname + "/SECURITY_CIPHERS=6;";
                sMsg += "modify /config.ini/" + ifname + "/KEY_WORD1=" + wpa_o_wpa2.WPA2PSKKey + ";";
                sMsg += "modify /config.ini/" + ifname + "/KEY_WORD2=" + wpa_o_wpa2.WPA2PSKKey + ";";
#ifdef WLAN2G5GSAMECONFIG
                sMsg += "modify /config.ini/" + ifname5G + "/AUTH_MODE=16;";
                sMsg += "modify /config.ini/" + ifname5G + "/SECURITY_CIPHERS=6;";
                sMsg += "modify /config.ini/" + ifname5G + "/KEY_WORD1=" + wpa_o_wpa2.WPA2PSKKey + ";";
                sMsg += "modify /config.ini/" + ifname5G + "/KEY_WORD2=" + wpa_o_wpa2.WPA2PSKKey + ";";
#endif
                break;

        case 6:
                // wpa2-psk
                sMsg += "modify /config.ini/" + ifname + "/AUTH_MODE=32;";
                sMsg += "modify /config.ini/" + ifname + "/SECURITY_CIPHERS=6;";
                sMsg += "modify /config.ini/" + ifname + "/KEY_WORD1=" + wpa_o_wpa2.WPA2PSKKey + ";";
                sMsg += "modify /config.ini/" + ifname + "/KEY_WORD2=" + wpa_o_wpa2.WPA2PSKKey + ";";
#ifdef WLAN2G5GSAMECONFIG
                sMsg += "modify /config.ini/" + ifname5G + "/AUTH_MODE=32;";
                sMsg += "modify /config.ini/" + ifname5G + "/SECURITY_CIPHERS=6;";
                sMsg += "modify /config.ini/" + ifname5G + "/KEY_WORD1=" + wpa_o_wpa2.WPA2PSKKey + ";";
                sMsg += "modify /config.ini/" + ifname5G + "/KEY_WORD2=" + wpa_o_wpa2.WPA2PSKKey + ";";
#endif
                break;
        default:
                break;
        }
    }
    return 0;
}
int AddWlanRequest::assemble(buffer *buf)
{
    return 0;
}
int AddWlanRequest::loadFromCM(kvlist& kv)
{
    return 0;
}

int AddWlanResponse::parse(buffer *buf)
{
    return 0;
}
int AddWlanResponse::saveToCM(string& sMsg)
{
    return 0;
}
int AddWlanResponse::assemble(buffer *buf)
{
    skipheader(buf);

    buf->store8(result_count);
    length += 1;
    for (vector<__result>::iterator it = results.begin();
         it != results.end();
         it++)
    {
        buf->store8(it->radioId);
        length += 1;
        buf->store8(it->wlanId);
        length += 1;
        buf->store8(it->result);
        length += 1;
        buf->storeRawBytes(it->BSSID, sizeof(it->BSSID));
        length += 6;
    }

    assembleHeader(buf);
    return 0;
}
int AddWlanResponse::loadFromCM(kvlist& kv)
{
    int mac[6] = {0};
    string value = "";
    unsigned int iValue = 0;

    result_count = 1;

    value = kv.getValue("radioid");
    string2int(value, iValue);
    results[0].radioId = iValue;

    value = kv.getValue("wlanid");
    string2int(value, iValue);
    results[0].wlanId = iValue;

    results[0].result = 0;

    value = kv.getValue("bssid");
    sscanf(value.c_str(), "%02x%02x%02x%02x%02x%02x",
           &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
    results[0].BSSID[0] = (unsigned char)mac[0];
    results[0].BSSID[1] = (unsigned char)mac[1];
    results[0].BSSID[2] = (unsigned char)mac[2];
    results[0].BSSID[3] = (unsigned char)mac[3];
    results[0].BSSID[4] = (unsigned char)mac[4];
    results[0].BSSID[5] = (unsigned char)mac[5];

    return 0;
}

int DelWlanRequest::parse(buffer *buf)
{
    ZCtrlHdr::parse(buf);

    buf->retrieve8(RadioId);
    buf->retrieve8(WlanId);
    return 0;
}
int DelWlanRequest::saveToCM(string& sMsg)
{
    string ifname = get_wlan_ifname(RadioId, WlanId);
    string strwlanid = "";
    int2string(WlanId-1, strwlanid);

    sMsg += string("del /config.ini/") + ifname + "/WLAN_ID=" + strwlanid + ";";
#if WLAN2G5GSAMECONFIG
    string ifname5G = get_wlan_ifname(RadioId+1, WlanId);
    sMsg += string("del /config.ini/") + ifname5G + "/WLAN_ID=" + strwlanid + ";";
#endif
    return 0;
}
int DelWlanRequest::assemble(buffer *buf)
{
    return 0;
}
int DelWlanRequest::loadFromCM(kvlist& kv)
{
    return 0;
}

int DelWlanResponse::parse(buffer *buf)
{
    return 0;
}
int DelWlanResponse::saveToCM(string& sMsg)
{
    return 0;
}
int DelWlanResponse::assemble(buffer *buf)
{
    skipheader(buf);

    buf->store8(RadioId);
    length += 1;
    buf->store8(WlanId);
    length += 1;
    buf->store8(result);
    length += 1;

    assembleHeader(buf);
    return 0;
}
int DelWlanResponse::loadFromCM(kvlist& kv)
{
    return 0;
}

int AddBlackListRequest::parse(buffer *buf)
{
    ZCtrlHdr::parse(buf);

    buf->retrieve8(count);
    for (int i = 0; i<count; ++i)
    {
        buf->retrieveRawBytes(macs[i], 6);
    }
    return 0;
}
int AddBlackListRequest::saveToCM(string& sMsg)
{
    char szValue[64] = {0};
    for(int i = 0; i<count; ++i)
    {
        sprintf(szValue,"%02x:%02x:%02x:%02x:%02x:%02x",
                macs[i][0],macs[i][1],macs[i][2],macs[i][3],macs[i][4],macs[i][5]);
        sMsg += string("add /config.ini/macfilter/MAC=") + szValue + ";";
    }

    return 0;
}
int AddBlackListRequest::assemble(buffer *buf)
{
    return 0;
}
int AddBlackListRequest::loadFromCM(kvlist& kv)
{
    return 0;
}

int AddBlackListResponse::parse(buffer *buf)
{
    return 0;
}
int AddBlackListResponse::saveToCM(string& sMsg)
{
    return 0;
}
int AddBlackListResponse::assemble(buffer *buf)
{
    skipheader(buf);

    buf->store8(count);
    length += 1;
    for (int i = 0; i<count; ++i)
    {
        buf->storeRawBytes(macs[i], 6);
        length += 6;
        buf->store8(result[i]);
        length += 1;
    }

    assembleHeader(buf);
    return 0;
}
int AddBlackListResponse::loadFromCM(kvlist& kv)
{
    return 0;
}

int DelBlackListRequest::parse(buffer *buf)
{
    ZCtrlHdr::parse(buf);

    buf->retrieve8(count);
    for (int i = 0; i<count; ++i)
    {
        buf->retrieveRawBytes(macs[i], 6);
    }
    return 0;
}
int DelBlackListRequest::saveToCM(string& sMsg)
{
    char szValue[64] = {0};
    for(int i = 0; i<count; ++i)
    {
        sprintf(szValue,"%02x:%02x:%02x:%02x:%02x:%02x",
                macs[i][0],macs[i][1],macs[i][2],macs[i][3],macs[i][4],macs[i][5]);
        sMsg += string("del /config.ini/macfilter/MAC=") + szValue + ";";
    }

    return 0;
}
int DelBlackListRequest::assemble(buffer *buf)
{
    return 0;
}
int DelBlackListRequest::loadFromCM(kvlist& kv)
{
    return 0;
}

int DelBlackListResponse::parse(buffer *buf)
{
    return 0;
}
int DelBlackListResponse::saveToCM(string& sMsg)
{
    return 0;
}
int DelBlackListResponse::assemble(buffer *buf)
{
    skipheader(buf);

    buf->store8(count);
    length += 1;
    for (int i = 0; i<count; ++i)
    {
        buf->storeRawBytes(macs[i], 6);
        length += 6;
        buf->store8(result[i]);
        length += 1;
    }

    assembleHeader(buf);
    return 0;
}
int DelBlackListResponse::loadFromCM(kvlist& kv)
{
    return 0;
}

int UpgradeRequest::parse(buffer *buf)
{
    ZCtrlHdr::parse(buf);
    buf->retrieve16(ImageType);
    buf->retrieve16(IdentifierLength);
    buf->retrieveRawBytes(ImageIdentifier, IdentifierLength);

    do {
        int begin_len = buf->GetLength() - buf->GetOffset();
        Protocol.parse(buf);
        serveripv4.parse(buf);
        url.parse(buf);
        username.parse(buf);
        passwd.parse(buf);
        int end_len = buf->GetLength() - buf->GetOffset();
        if (begin_len == end_len)
        {
            skiptlv(buf);
        }
    }while (buf->GetOffset() < buf->GetLength());

    return 0;
}
int UpgradeRequest::assemble(buffer *buf)
{
    return 0;
}
int UpgradeRequest::saveToCM(string &sMsg)
{
    if(1 == ImageType)
    {
        sMsg += "modify /config.ini/loadconf/LOAD_FLAG=0;";
	    sMsg += "modify /config.ini/loadconf/FILE_TYPE=0;";

        if(true == Protocol.valid)
        {
            Protocol.saveToCM(sMsg);
        }
        //∑˛ŒÒ∆˜ip
        if(true == serveripv4.valid)
        {
            serveripv4.saveToCM(sMsg);
        }

        //¬∑æ∂œ¬∑¢
        if(true == url.valid)
        {
            url.saveToCM(sMsg);
        }
        //”√ªß√˚œ¬∑¢
        if(true == username.valid)
        {
            username.saveToCM(sMsg);
        }
        //√‹¬Îœ¬∑¢
        if(true == passwd.valid)
        {
            passwd.saveToCM(sMsg);
        }

        //…˝º∂ø™πÿ
        sMsg += "modify_force /config.ini/loadconf/LOAD_SWITCH=1;";
    }
    return 0;
}
int UpgradeRequest::loadFromCM(kvlist &kv)
{
    return 0;
}

int UpgradeResponse::parse(buffer *buf)
{
    return 0;
}
int UpgradeResponse::saveToCM(string& sMsg)
{
    return 0;
}
int UpgradeResponse::assemble(buffer *buf)
{
    skipheader(buf);

    buf->store8(result);
    length += 1;

    assembleHeader(buf);
    return 0;
}
int UpgradeResponse::loadFromCM(kvlist& kv)
{
    return 0;
}

int StaEventInfoRequest::parse(buffer *buf)
{
    return 0;
}
int StaEventInfoRequest::assemble(buffer *buf)
{
    skipheader(buf);

    buf->storeRawBytes(UserMac, 6);
    length += 6;
    buf->storeRawBytes(BSSID, 6);
    length += 6;
    buf->store8(SecureType);
    length += 1;
    buf->store32(TimeStamp);
    length += 4;
    buf->store8(SsidLength);
    length += 1;
    buf->storeRawBytes((unsigned char *)Ssid.c_str(), SsidLength);
    length += SsidLength;

    if (qostlv.isValid())
    {
        qostlv.assemble(buf);
        length += qostlv.length + 4;
    }
    if (msgtypetlv.isValid())
    {
        msgtypetlv.assemble(buf);
        length += msgtypetlv.length + 4;
    }
    if (wlanidtlv.isValid())
    {
        wlanidtlv.assemble(buf);
        length += msgtypetlv.length + 4;
    }
    if (radioidtlv.isValid())
    {
        radioidtlv.assemble(buf);
        length += radioidtlv.length + 4;
    }
    if (leavereasontlv.isValid())
    {
        leavereasontlv.assemble(buf);
        length += wlanidtlv.length + 4;
    }

    assembleHeader(buf);
    return 0;
}
int StaEventInfoRequest::saveToCM(string &sMsg)
{
    return 0;
}
int StaEventInfoRequest::loadFromCM(kvlist& kv)
{
    int mac[6] = {0};
    unsigned int iValue = 0;
    string value = "";

    value = kv.getValue("stamac");
    sscanf(value.c_str(), "%02x%02x%02x%02x%02x%02x",
           &mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
    UserMac[0] = (unsigned char)mac[0];
    UserMac[1] = (unsigned char)mac[1];
    UserMac[2] = (unsigned char)mac[2];
    UserMac[3] = (unsigned char)mac[3];
    UserMac[4] = (unsigned char)mac[4];
    UserMac[5] = (unsigned char)mac[5];

    value = kv.getValue("bssid");
    sscanf(value.c_str(), "%02x%02x%02x%02x%02x%02x",
           &mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
    BSSID[0] = (unsigned char)mac[0];
    BSSID[1] = (unsigned char)mac[1];
    BSSID[2] = (unsigned char)mac[2];
    BSSID[3] = (unsigned char)mac[3];
    BSSID[4] = (unsigned char)mac[4];
    BSSID[5] = (unsigned char)mac[5];

    value = kv.getValue("securityType");
    string2int(value, iValue);
    SecureType = (unsigned char) iValue;

    value = kv.getValue("time");
    string2int(value, TimeStamp);

    Ssid = kv.getValue("ssid");

    SsidLength = Ssid.length();

    if (qostlv.isValid())
    {
        qostlv.loadFromCM(kv);
    }
    if (msgtypetlv.isValid())
    {
        msgtypetlv.loadFromCM(kv);
    }
    if (wlanidtlv.isValid())
    {
        wlanidtlv.loadFromCM(kv);
    }
    if (radioidtlv.isValid())
    {
        radioidtlv.loadFromCM(kv);
    }
    if (leavereasontlv.isValid())
    {
        leavereasontlv.loadFromCM(kv);
    }
    return 0;
}

int ExtMessage::parse(buffer *buf)
{
    ZCtrlHdr::parse(buf);
    buf->retrieve16(version);
    buf->retrieve8(seq_number);
    buf->retrieve8(reverse);
    return 0;
}
int ExtMessage::assemble(buffer *buf)
{
    ZCtrlHdr::skipheader(buf);

    buf->store16(version);
    length += 2;
    buf->store8(seq_number);
    length += 1;
    buf->store8(reverse);
    length += 1;

    ZCtrlHdr::assembleHeader(buf);
    return 0;
}
int ExtMessage::saveToCM(string& sMsg)
{
    return 0;
}
int ExtMessage::loadFromCM(kvlist& kv)
{
    return 0;
}

int SetStationStateRequest::parse(buffer *buf)
{
    ExtMessage::parse(buf);

    do {
        int begin_len = buf->GetLength() - buf->GetOffset();

        addstation.parse(buf);
        delstation.parse(buf);
        vlanupdate.parse(buf);
        statidupdate.parse(buf);
        useragent.parse(buf);

        int end_len = buf->GetLength() - buf->GetOffset();
        if (begin_len == end_len)
        {
            skiptlv(buf);
        }
    }while (0 < (int)(buf->GetLength()-buf->GetOffset()));

    return 0;
}
int SetStationStateRequest::assemble(buffer *buf)
{
    return 0;
}
int SetStationStateRequest::saveToCM(string& sMsg)
{
    if (addstation.isValid())
    {
        addstation.saveToCM(sMsg);
    }
    if (delstation.isValid())
    {
        delstation.saveToCM(sMsg);
    }
    if (vlanupdate.isValid())
    {
        vlanupdate.saveToCM(sMsg);
    }
    if (statidupdate.isValid())
    {
        statidupdate.saveToCM(sMsg);
    }
    if (useragent.isValid())
    {
        useragent.saveToCM(sMsg);
    }

    return 0;
}
int SetStationStateRequest::loadFromCM(kvlist& kv)
{
    return 0;
}

int SetStationStateResponse::parse(buffer *buf)
{
    return 0;
}
int SetStationStateResponse::assemble(buffer *buf)
{
    skipheader(buf);

    if (resulttlv.isValid())
    {
        resulttlv.assemble(buf);
        length += resulttlv.length + 4;
    }

    assembleHeader(buf);
    return 0;
}
int SetStationStateResponse::saveToCM(string& sMsg)
{
    return 0;
}
int SetStationStateResponse::loadFromCM(kvlist& kv)
{
    return 0;
}

int SetRadioRequest::parse(buffer *buf)
{
    ExtMessage::parse(buf);

    do {
        int begin_len = buf->GetLength() - buf->GetOffset();

        ModelSet.parse(buf);
        PowerSet.parse(buf);
        ChannelSet.parse(buf);
        ChainMaskSet.parse(buf);
        AmpduSet.parse(buf);
        BandWidthSet.parse(buf);
        ShortGISet.parse(buf);
        AmsduSet.parse(buf);
        BeaconIntvSet.parse(buf);
        DtimIntvSet.parse(buf);
        FragSet.parse(buf);
        RtsSet.parse(buf);
        ShortPreambleSet.parse(buf);
        CountrySet.parse(buf);

        int end_len = buf->GetLength() - buf->GetOffset();
        if (begin_len == end_len)
        {
            skiptlv(buf);
        }
    }while (buf->GetOffset() < buf->GetLength());

    return 0;
}
int SetRadioRequest::assemble(buffer *buf)
{
    return 0;
}
int SetRadioRequest::saveToCM(string& sMsg)
{
    if (ModelSet.isValid())
    {
        ModelSet.saveToCM(sMsg);
    }
    if (PowerSet.isValid())
    {
        PowerSet.saveToCM(sMsg);
    }
    if (ChannelSet.isValid())
    {
        ChannelSet.saveToCM(sMsg);
    }
    if (ChainMaskSet.isValid())
    {
        ChainMaskSet.saveToCM(sMsg);
    }
    if (AmpduSet.isValid())
    {
        AmpduSet.saveToCM(sMsg);
    }
    if (BandWidthSet.isValid())
    {
        BandWidthSet.saveToCM(sMsg);
    }
    if (ShortGISet.isValid())
    {
        ShortGISet.saveToCM(sMsg);
    }
    if (AmsduSet.isValid())
    {
        AmsduSet.saveToCM(sMsg);
    }
    if (BeaconIntvSet.isValid())
    {
        BeaconIntvSet.saveToCM(sMsg);
    }
    if (DtimIntvSet.isValid())
    {
        DtimIntvSet.saveToCM(sMsg);
    }
    if (FragSet.isValid())
    {
        FragSet.saveToCM(sMsg);
    }
    if (RtsSet.isValid())
    {
        RtsSet.saveToCM(sMsg);
    }
    if (ShortPreambleSet.isValid())
    {
        ShortPreambleSet.saveToCM(sMsg);
    }
    if (CountrySet.isValid())
    {
        CountrySet.saveToCM(sMsg);
    }
    return 0;
}
int SetRadioRequest::loadFromCM(kvlist& kv)
{
    return 0;
}

int SetRadioResponse::parse(buffer *buf)
{
    return 0;
}
int SetRadioResponse::assemble(buffer *buf)
{
    skipheader(buf);

    if (resulttlv.isValid())
    {
        resulttlv.assemble(buf);
        length += resulttlv.length + 4;
    }

    assembleHeader(buf);
    return 0;
}
int SetRadioResponse::saveToCM(string& sMsg)
{
    return 0;
}
int SetRadioResponse::loadFromCM(kvlist& kv)
{
    return 0;
}

int SetLoadBalanceRequest::parse(buffer *buf)
{
    ExtMessage::parse(buf);

    do {
        int begin_len = buf->GetLength() - buf->GetOffset();

        int end_len = buf->GetLength() - buf->GetOffset();
        if (begin_len == end_len)
        {
            skiptlv(buf);
        }
    }while (buf->GetOffset() < buf->GetLength());

    return 0;
}
int SetLoadBalanceRequest::assemble(buffer *buf)
{
    return 0;
}
int SetLoadBalanceRequest::saveToCM(string& sMsg)
{
    return 0;
}
int SetLoadBalanceRequest::loadFromCM(kvlist& kv)
{
    return 0;
}

int SetLoadBalanceResponse::parse(buffer *buf)
{
    return 0;
}
int SetLoadBalanceResponse::assemble(buffer *buf)
{
    skipheader(buf);

    if (resulttlv.isValid())
    {
        resulttlv.assemble(buf);
        length += resulttlv.length + 4;
    }

    assembleHeader(buf);
    return 0;
}
int SetLoadBalanceResponse::saveToCM(string& sMsg)
{
    return 0;
}
int SetLoadBalanceResponse::loadFromCM(kvlist& kv)
{
    return 0;
}

int SetDataTunnelRequest::parse(buffer *buf)
{
    ExtMessage::parse(buf);

    do {
        int begin_len = buf->GetLength() - buf->GetOffset();

        tuninfotlv.parse(buf);
        switchbackuptlv.parse(buf);

        int end_len = buf->GetLength() - buf->GetOffset();
        if (begin_len == end_len)
        {
            skiptlv(buf);
        }
    }while (buf->GetOffset() < buf->GetLength());

    return 0;
}
int SetDataTunnelRequest::assemble(buffer *buf)
{
    return 0;
}
int SetDataTunnelRequest::saveToCM(string& sMsg)
{
    if (tuninfotlv.isValid())
    {
        tuninfotlv.saveToCM(sMsg);
    }
    if (switchbackuptlv.isValid())
    {
        switchbackuptlv.saveToCM(sMsg);
    }
    return 0;
}
int SetDataTunnelRequest::loadFromCM(kvlist& kv)
{
    return 0;
}

int SetDataTunnelResponse::parse(buffer *buf)
{
    return 0;
}
int SetDataTunnelResponse::assemble(buffer *buf)
{
    skipheader(buf);

    if (resulttlv.isValid())
    {
        resulttlv.assemble(buf);
        length += resulttlv.length + 4;
    }

    assembleHeader(buf);
    return 0;
}
int SetDataTunnelResponse::saveToCM(string& sMsg)
{
    return 0;
}
int SetDataTunnelResponse::loadFromCM(kvlist& kv)
{
    return 0;
}

int SystemMaintainRequest::parse(buffer *buf)
{
    ExtMessage::parse(buf);

    do {
        int begin_len = buf->GetLength() - buf->GetOffset();

        systemmaintenance.parse(buf);

        int end_len = buf->GetLength() - buf->GetOffset();
        if (begin_len == end_len)
        {
            skiptlv(buf);
        }
    }while (buf->GetOffset() < buf->GetLength());

    return 0;
}
int SystemMaintainRequest::assemble(buffer *buf)
{
    return 0;
}
int SystemMaintainRequest::saveToCM(string& sMsg)
{
    if (systemmaintenance.isValid())
    {
        systemmaintenance.saveToCM(sMsg);
    }
    return 0;
}
int SystemMaintainRequest::loadFromCM(kvlist& kv)
{
    return 0;
}


int SystemMaintainResponse::parse(buffer *buf)
{
    return 0;
}
int SystemMaintainResponse::assemble(buffer *buf)
{
    skipheader(buf);

    if (resulttlv.isValid())
    {
        resulttlv.assemble(buf);
        length += resulttlv.length + 4;
    }

    assembleHeader(buf);
    return 0;
}
int SystemMaintainResponse::saveToCM(string& sMsg)
{
    return 0;
}
int SystemMaintainResponse::loadFromCM(kvlist& kv)
{
    return 0;
}

int GenericConfigRequest::parse(buffer *buf)
{
    ExtMessage::parse(buf);

    do {
        int begin_len = buf->GetLength() - buf->GetOffset();

        Generic11ntlv.parse(buf);
        tunnelSlicetlv.parse(buf);
        stationAgeingtlv.parse(buf);
        Generic11nNewtlv.parse(buf);

        int end_len = buf->GetLength() - buf->GetOffset();
        if (begin_len == end_len)
        {
            skiptlv(buf);
        }
    }while (buf->GetOffset() < buf->GetLength());

    return 0;
}
int GenericConfigRequest::assemble(buffer *buf)
{
    return 0;
}
int GenericConfigRequest::saveToCM(string& sMsg)
{
    if (Generic11ntlv.isValid())
    {
        Generic11ntlv.saveToCM(sMsg);
    }
    if (tunnelSlicetlv.isValid())
    {
        tunnelSlicetlv.saveToCM(sMsg);
    }
    if (stationAgeingtlv.isValid())
    {
        stationAgeingtlv.saveToCM(sMsg);
    }
    if (Generic11nNewtlv.isValid())
    {
        Generic11nNewtlv.saveToCM(sMsg);
    }
    return 0;
}
int GenericConfigRequest::loadFromCM(kvlist& kv)
{
    return 0;
}

int GenericConfigResponse::parse(buffer *buf)
{
    return 0;
}
int GenericConfigResponse::assemble(buffer *buf)
{
    skipheader(buf);

    if (resulttlv.isValid())
    {
        resulttlv.assemble(buf);
        length += resulttlv.length + 4;
    }

    assembleHeader(buf);
    return 0;
}
int GenericConfigResponse::saveToCM(string& sMsg)
{
    return 0;
}
int GenericConfigResponse::loadFromCM(kvlist& kv)
{
    return 0;
}

int UpdateWlanRequest::parse(buffer *buf)
{
    ExtMessage::parse(buf);

    do {
        int begin_len = buf->GetLength() - buf->GetOffset();

        tunnelID.parse(buf);
        upflowctl.parse(buf);
        enableWlan.parse(buf);
        localSsid.parse(buf);
        portalurl.parse(buf);

        int end_len = buf->GetLength() - buf->GetOffset();
        if (begin_len == end_len)
        {
            skiptlv(buf);
        }
    }while (buf->GetOffset() < buf->GetLength());

    return 0;
}
int UpdateWlanRequest::assemble(buffer *buf)
{
    return 0;
}
int UpdateWlanRequest::saveToCM(string& sMsg)
{
    if (tunnelID.isValid())
    {
        tunnelID.saveToCM(sMsg);
    }
    if (upflowctl.isValid())
    {
        upflowctl.saveToCM(sMsg);
    }
    if (enableWlan.isValid())
    {
        enableWlan.saveToCM(sMsg);
    }
    if (localSsid.isValid())
    {
        localSsid.saveToCM(sMsg);
    }
    if (portalurl.isValid())
    {
        portalurl.saveToCM(sMsg);
    }
    return 0;
}
int UpdateWlanRequest::loadFromCM(kvlist& kv)
{
    return 0;
}

int UpdateWlanResponse::parse(buffer *buf)
{
    return 0;
}
int UpdateWlanResponse::assemble(buffer *buf)
{
    skipheader(buf);

    if (resulttlv.isValid())
    {
        resulttlv.assemble(buf);
        length += resulttlv.length + 4;
    }

    assembleHeader(buf);
    return 0;
}
int UpdateWlanResponse::saveToCM(string& sMsg)
{
    return 0;
}
int UpdateWlanResponse::loadFromCM(kvlist& kv)
{
    return 0;
}

int KeyEventsRequest::parse(buffer *buf)
{
    return 0;
}
int KeyEventsRequest::assemble(buffer *buf)
{
    skipheader(buf);

    if (hotreboot.isValid())
    {
        hotreboot.assemble(buf);
        length += hotreboot.length + 4;
    }
    if (radioupdown.isValid())
    {
        radioupdown.assemble(buf);
        length += radioupdown.length + 4;
    }
    if (wlandeldown.isValid())
    {
        wlandeldown.assemble(buf);
        length += wlandeldown.length + 4;
    }
    if (wlanup.isValid())
    {
        wlanup.assemble(buf);
        length += wlanup.length + 4;
    }

    assembleHeader(buf);
    return 0;
}
int KeyEventsRequest::saveToCM(string& sMsg)
{
    return 0;
}
int KeyEventsRequest::loadFromCM(kvlist& kv)
{
    if (hotreboot.isValid())
    {
        hotreboot.loadFromCM(kv);
    }
    if (radioupdown.isValid())
    {
        radioupdown.loadFromCM(kv);
    }
    if (wlandeldown.isValid())
    {
        wlandeldown.loadFromCM(kv);
    }
    if (wlanup.isValid())
    {
        wlanup.loadFromCM(kv);
    }

    return 0;
}

int ACLoginRequest::parse(buffer *buf)
{
    ExtMessage::parse(buf);

    do {
        int begin_len = buf->GetLength() - buf->GetOffset();

        acipv4.parse(buf);

        int end_len = buf->GetLength() - buf->GetOffset();
        if (begin_len == end_len)
        {
            skiptlv(buf);
        }
    }while (buf->GetOffset() < buf->GetLength());

    return 0;
}
int ACLoginRequest::assemble(buffer *buf)
{
    return 0;
}
int ACLoginRequest::saveToCM(string& sMsg)
{
    if (acipv4.isValid())
    {
        acipv4.saveToCM(sMsg);
    }
    return 0;
}
int ACLoginRequest::loadFromCM(kvlist& kv)
{
    return 0;
}

int SetSnmpRequest::parse(buffer *buf)
{
    ExtMessage::parse(buf);

    do {
        int begin_len = buf->GetLength() - buf->GetOffset();

        ginterval.parse(buf);
        trapkey.parse(buf);
        readkey.parse(buf);
        writekey.parse(buf);
        alarms_switch.parse(buf);

        int end_len = buf->GetLength() - buf->GetOffset();
        if (begin_len == end_len)
        {
            skiptlv(buf);
        }
    }while (buf->GetOffset() < buf->GetLength());

    return 0;
}
int SetSnmpRequest::assemble(buffer *buf)
{
    return 0;
}
int SetSnmpRequest::saveToCM(string& sMsg)
{
    if (ginterval.isValid())
    {
        ginterval.saveToCM(sMsg);
    }
    if (trapkey.isValid())
    {
        trapkey.saveToCM(sMsg);
    }
    if (readkey.isValid())
    {
        readkey.saveToCM(sMsg);
    }
    if (writekey.isValid())
    {
        writekey.saveToCM(sMsg);
    }
    if (alarms_switch.isValid())
    {
        alarms_switch.saveToCM(sMsg);
    }
    return 0;
}
int SetSnmpRequest::loadFromCM(kvlist& kv)
{
    return 0;
}


int SetSnmpResponse::parse(buffer *buf)
{
    return 0;
}
int SetSnmpResponse::assemble(buffer *buf)
{
    skipheader(buf);

    if (resulttlv.isValid())
    {
        resulttlv.assemble(buf);
        length += resulttlv.length + 4;
    }

    assembleHeader(buf);
    return 0;
}
int SetSnmpResponse::saveToCM(string& sMsg)
{
    return 0;
}
int SetSnmpResponse::loadFromCM(kvlist& kv)
{
    return 0;
}

int StaAssosicInfoRequest::parse(buffer *buf)
{
    return 0;
}
int StaAssosicInfoRequest::assemble(buffer *buf)
{
    skipheader(buf);

    if (macaddrtlv.isValid())
    {
        macaddrtlv.assemble(buf);
        length += macaddrtlv.length + 4;
    }
    if (ipaddrtlv.isValid())
    {
        ipaddrtlv.assemble(buf);
        length += ipaddrtlv.length + 4;
    }

    assembleHeader(buf);
    return 0;
}
int StaAssosicInfoRequest::saveToCM(string& sMsg)
{
    return 0;
}
int StaAssosicInfoRequest::loadFromCM(kvlist& kv)
{
    if (macaddrtlv.isValid())
    {
        macaddrtlv.loadFromCM(kv);
    }
    if (ipaddrtlv.isValid())
    {
        ipaddrtlv.loadFromCM(kv);
    }
    return 0;
}


