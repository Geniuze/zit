#ifndef _WP_COMMON_H
#define _WP_COMMON_H

#define WP_TCP 0
#define WP_UDP 1
#define WP_MAC_LEN      6
#define WP_MSG_32       32
#define WP_MSGLEN_64    64
#define WP_MSGLEN_512   512
#define UDP_MAX_LENGTH  1400

enum code_value
{
    JF_CODE,
    YP_CODE,
    SH_CODE,
    AG_CODE,    /* 艾逛 */
    ADT_CODE,   /* 安达通 */
    HL_CODE,    /* 禾连 */
    YS_CODE,    /* 宇视 */
    TD_CODE,   /* 淘点 */
};

enum miType
{
    MI_STA = 1,
    MI_TAG,
    MI_AP,
};

#ifndef IWLIB_H
#define IWLIB_H
/* Encryption type  */
enum iw_encryption_type
{
    OPEN = 0x0,
    WEP,
    WPA_EAP_CCMP,
    WPA_EAP_TKIP,
    WPA_EAP_BOTH,
    WPA_PSK_CCMP,
    WPA_PSK_TKIP,
    WPA_PSK_BOTH,
    WPA2_PSK_CCMP,
    WPA2_PSK_TKIP,
    WPA2_PSK_BOTH,
    WPA2_EAP_CCMP,
    WPA2_EAP_TKIP,
    WPA2_EAP_BOTH,
    WPA_WPA2_CCMP,
    WPA_WPA2_TKIP,
    WPA_WPA2_BOTH,
    KEY_UNKNOWN,
    KEY_MAX,
};
#endif


#define MAX_HISSSID_NUM     5
typedef struct message_drive{
    unsigned char   macaddr[6];
    unsigned short  rssi;
    unsigned char   bssid[6];
    unsigned short  type;
    unsigned short  seqnum;
    unsigned short  noisefloor;
    unsigned short  radiotype;
    unsigned short  channel;
    unsigned short  foundcount;
    unsigned short  assocstate;
    unsigned int    tagStatus;
    unsigned int    ipaddr;
    unsigned int    keytype;
    unsigned long   firsttime;
    unsigned long   lasttime;
    unsigned char   ssid[32];
    unsigned int    ssidnum;
    unsigned char   hisssid[MAX_HISSSID_NUM][32]; // history ssid
}MESSAGE_DRIVE;

struct transfer_conf{
    int port;
    int pro;
    int scantype;
    int vendor;
    int intv;
    unsigned long long int serial_num;
    char ip[64];  
};

struct scan_report{
    int(*init_socket)(); //返回套接字 
    int(*send_packet)(int sock, int port, char *ip, unsigned char *buf, int length);
    void(*assemb_contract)(unsigned char *save, struct transfer_conf *conf, struct scan_report *pointer, int savelen);
    void(*close_socket)(int sock); 
};

typedef struct tagTLV
{
    unsigned short type;
    unsigned short lenth; 
}WP_MSG_HEAD;

#define WP_DATA     1
#define WP_NO_DATA  0

int wp_macCompare(const unsigned char *m1, const unsigned char *m2);
void WP_plumb_func(struct transfer_conf *conf, struct scan_report * pointer);
void SH_give_pointer(struct scan_report *pointer);
void AG_give_pointer(struct scan_report *pointer);
void ADT_give_pointer(struct scan_report *pointer);
void HL_give_pointer(struct scan_report *pointer);
void YS_give_pointer(struct scan_report *pointer, unsigned long long int DevID);
int WP_tcp_sendbuf(char *ip, int port, unsigned char *buf, int buflen);
int WP_udp_sendbuf(char *ip, int port, unsigned char *buf, int buflen);

/* link */
typedef struct msgNode
{
    char *pMsg;
    int   iLen;
    struct msgNode *pnext;
}WP_RCV_NODE;

struct msgNode *get_node(void);
void insert_node(struct msgNode *pNode);

#endif

