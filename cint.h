/*
 * cint.h
 *
 *  Created on: 2012-1-5
 *      Author: liangyong
 */

#ifndef _DUNCHONG_CINT_H_
#define _DUNCHONG_CINT_H_

#ifdef __cplusplus
extern "C" {
#endif
/* message define */
#define MSG_TAG_TYPE		"type"
#define MSG_TAG_SUBTYPE	"subtype"
#define MSG_TAG_SRCTSK		"srctsk"
#define MSG_TAG_SRCSVR	"srcsvr"
#define MSG_TAG_DSTTSK		"dsttsk"
#define MSG_TAG_DSTSVR	"dstsvr"
#define MSG_TAG_TOTLEN	"totlen"
#define MSG_TAG_MSGID		"msgid"
#define MSG_TAG_DATA		"data"


typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;

typedef unsigned char u_int8_t;
typedef unsigned short u_int16_t;
typedef unsigned int u_int32_t;
typedef unsigned long long u_int64_t;

typedef unsigned int size_t;
typedef unsigned int u_int;
typedef unsigned long u_long;

typedef struct
{
	u_int8_t  type;	//管理消息(注册、订阅等)，数据消息
 	u_int8_t  subType;	//请求消息，响应消息
 	u_int8_t  srcTsk;	
 	u_int8_t  srcSvr;
 	u_int8_t  dstTsk;	
 	u_int8_t  dstSvr;
	u_int16_t uiMsgid;
	u_int16_t uiTotLen;
}MSG_HEADER_ST;


/*
 * IMessage C语言版本接口
 */
typedef void* msg_t;
typedef void* IPMsg_t;
typedef void* FILE_HANDLE;

#define MSG_BODY_MAX_LEN	4096

int open_conn(char szPath[32]);
int close_conn();
msg_t create_msg();
void release_msg(msg_t msg);
msg_t set_msg_header(msg_t pMsg, MSG_HEADER_ST stMsgHeader);
msg_t set_msg_body(msg_t pMsg, char szBody[MSG_BODY_MAX_LEN], int iLen);
int send_msg(msg_t pMsg);
msg_t recv_msg(int iTimeOut);
int get_int(msg_t msg, const char* tag);

#if 0
msg_t get_msg();

void put_msg(msg_t msg);

int set_int(msg_t msg, const char* tag, int value);

int set_str(msg_t msg, const char* tag, const char* value);

int get_int(msg_t msg, const char* tag);

int get_str(msg_t msg, const char* tag, const char* value);

int open_conn(const char* src);

int send_msg(const char* task, const char* service, msg_t msg/*, IPMsg_t ipMsg*/);

msg_t recv_msg(/*int packid, IPMsg_t ipMsg*/);

int close_conn();
#endif

FILE_HANDLE loadConf(const char *pcFilePath);

int saveConf(FILE_HANDLE pstConfigFile, char *pcFilePath);

int delFileHandle(FILE_HANDLE pstConfigFile);

int getItemNumByPath(FILE_HANDLE pstConfigFile, char *pcConfigPath);

int getValueByItemIdx(FILE_HANDLE pstConfigFile, char *pcConfigPath, int iItemIndex, char *pcConfigValue);

void getValueByPath(FILE_HANDLE pstConfigFile, const char *pcConfigPath, char *pcConfigValue);

void* getUserConfigService();

#define TASK_NAME_CGI	"cgi"
#define TASK_IMP_PATH	"/tmp/imp/"
#define CGI_CONFIG_PATH "/tmp/"
#define CM_CONFIG_FILE	"config.ini"
#define CGI_CONFIG_FILE	CGI_CONFIG_PATH"config.ini"

#define CGI_SUCCESS			0
#define CGI_FAIL				1
#define CGI_COMMON_ERR	2

#define MAX_WIFI_NUM		2
#define MAX_PAYLOAD_LENGTH	4096
#define STRING_LENGTH_8	8
#define STRING_LENGTH_16	16
#define STRING_LENGTH_32	32
#define STRING_LENGTH_64	64
#define STRING_LENGTH_1024		1024
#define MAX_BRANCH_PATH     64
#define MAX_BRANCH_NAME     16

//int sock;	//可以由httpd定义，调用cgi传入，所有cgi使用
//char szPayLoad[MAX_PAYLOAD_LENGTH];

enum eOptType
{
	CH_TYPE_MODIFY,
	CH_TYPE_READ,
	CH_TYPE_MODIFY_FILE_ONLY,
	CH_TYPE_ADD,
	CH_TYPE_DEL,
	CH_TYPE_DEL_SET,
	CH_TYPE_CMD,
	CH_TYPE_MAX,
};

enum WlanStatus
{
	EM_WLAN_DELETED,
	EM_WLAN_CREATED,
};

struct item_pair
{
	char *item_path;
	char *item_value;
};

enum eTaskId
{
	CAE_TASK_CM = 1,
	CAE_TASK_CGI,
	CAE_TASK_CAPWAP,
	CAE_TASK_EM,
	CAE_TASK_VAPI,
	CAE_TASK_SNMP,
	CAE_TASK_EVENT,
	CAE_TASK_CONF,
};

/* 射频标识 */
enum RadioFlag 
{
	Radio_2G,
	Radio_5G,
	Radio_MAX,
};

#ifdef __cplusplus
  }
#endif

#endif /* _DUNCHONG_CINT_H_ */
