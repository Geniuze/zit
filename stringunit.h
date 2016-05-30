#ifndef __STRINGUNIT_H__
#define __STRINGUNIT_H__

enum ENCODE
{
    GB2312 = 0,
    UTF8 = 1,
};

unsigned int string2int(const string & str, unsigned int &i);

void int2string(unsigned int i, string &str);

vector<string> split(string s, string lim);

int changeUnicode(string src, string src_unicode, string &dst, string dst_unicode);

int be_UTF8(const char * str);

string MacNoColo2Colo(string nocolo);

#endif
