#include <iostream>

using namespace std;
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "iconv.h"
#include "stringunit.h"

unsigned int string2int(const string & str, unsigned int &i)
{
#if 0
    stringstream s;
    s << str;
    try{
        s >> i;
    }
    catch(...){
        cout << "error hahaha" << endl;
        i = 0;
    }
    return i;
#endif
    sscanf(str.c_str(), "%d", &i);
    return i;
}

void int2string(unsigned int i, string &str)
{
    char buf[128] = {0};
    snprintf(buf, sizeof(buf), "%d", i);
    str = buf;
}
void int2stringhex(unsigned int i, string &str)
{
    char buf[128] = {0};
    snprintf(buf, sizeof(buf), "%#x", i);
    str = buf;
}

vector<string> split(string s, string lim)
{
    vector<string> v;
    size_t pos = s.find(lim);
    while(pos != string::npos)
    {
        v.push_back(s.substr(0, pos));
        s = s.substr(pos+1);

        pos = s.find(lim);
    }
    v.push_back(s);

    cout << "parse string by \"" << lim << "\"" << endl;

    for (vector<string>::iterator it = v.begin();
         it != v.end();
         it ++)
    {
        cout << *it << endl;
    }
    return v;
}

int changeUnicode(string src, string src_unicode, string &dst, string dst_unicode)
{
    iconv_t cd;

    char *szFrom, *szTo;
    unsigned int iLenFrom, iLenTo;

    unsigned int outLength = src.length() * 6;
    char *szOut = (char *)malloc(outLength);
    memset(szOut, 0, outLength);

    const char *p = src.c_str();
    szFrom = const_cast<char *>(p);

    szTo = szOut;

    iLenFrom = src.length();

    iLenTo = outLength;
    cd = iconv_open(dst_unicode.c_str(), src_unicode.c_str());

    if(iconv(cd, &szFrom, &iLenFrom, &szTo, &iLenTo) == (unsigned int) -1)
    {
        cout << "convert failed" << endl;
        syslog(LOG_ERR, "[%s:%d] convert string [%s->%s:%s] failed\n", __func__, __LINE__,
               src_unicode.c_str(), dst_unicode.c_str(), src.c_str());
    }

    iconv_close(cd);

    szTo = szOut;
    iLenTo = strlen(szOut);
    szTo[iLenTo] = '\0';

    dst.assign(szOut);
    if (szOut)
    {
        free(szOut);
        szOut = NULL;
    }
    return iLenTo;
}


static u_int32_t count_GB2312(const char * str)
{
    u_int32_t len = (u_int32_t)strlen (str);
    u_int32_t  counter = 0, i;
    u_int8_t  head = 0x80;
    u_int8_t  firstChar, secondChar;

    for (i = 0; i < len - 1; ++i)
    {
        firstChar = (u_int8_t)str[i];
        if (!(firstChar & head))
        {
            continue;
        }
        secondChar = (u_int8_t)str[i+1];
        if (firstChar >= 161 && firstChar <= 247 && secondChar>=161 && secondChar <= 254)
        {
            counter+= 2;
            ++i;
        }
    }
    return counter;
}

static u_int32_t count_UTF8(const char * str)
{
    u_int32_t  len = (u_int32_t)strlen (str);
    u_int32_t  counter = 0, i;
    u_int8_t   head = 0x80;
    u_int8_t   firstChar;
    for (i = 0; i < len; ++i)
    {
        firstChar = (u_int8_t)str[i];
        if (!(firstChar & head))
        {
            continue;
        }

        u_int8_t  tmpHead = head;
        u_int32_t wordLen = 0 , tPos = 0;
        while (firstChar & tmpHead)
        {
            ++ wordLen;
            tmpHead >>= 1;
        }
        if (wordLen <= 1)
        {
            continue; //utf8最小长度为2
        }
        wordLen --;
        if (wordLen + i >= len)
        {
            break;
        }
        for (tPos = 1; tPos <= wordLen; ++tPos)
        {
            u_int8_t  secondChar = (u_int8_t)str[i + tPos];
            if (!(secondChar & head))
            {
                break;
            }
        }
        if (tPos > wordLen)
        {
            counter += wordLen + 1;
            i += wordLen;
        }
    }
    return counter;
}


int be_UTF8(const char * str)
{
    u_int32_t iGBK = 0;
    u_int32_t iUTF8 = 0;

    if (0 == strlen(str))
    {
        return GB2312;
    }

    iGBK = count_GB2312(str);
    iUTF8= count_UTF8(str);
    if (iUTF8 > iGBK)
    {
        return UTF8;
    }
    return GB2312;
}

string MacNoColo2Colo(string nocolo)
{
    int mac[6] = {0};
    char mac_colo[64] = {0};

    sscanf(nocolo.c_str(), "%02x%02x%02x%02x%02x%02x",
           &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);

    snprintf(mac_colo, sizeof(mac_colo), "%02x:%02x:%02x:%02x:%02x:%02x",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    string colo = mac_colo;

    return colo;
}

