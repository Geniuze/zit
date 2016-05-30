#include <iostream>
using namespace std;

#include <string>
#include <map>
#include <vector>

#include "stringunit.h"
#include "kvlist.h"

int kvlist::GetKvFromStr(string s)
{
   vector<string> v = split(s, ";");
   kvlist kv;
   for (vector<string>::iterator it = v.begin();
        it != v.end();
        it ++)
   {
       size_t pos = it->find("=");
       if (pos != string::npos)
       {
           string key = it->substr(0,pos);
           string value = it->substr(pos+1);
           cout << "key = " << key << endl;
           cout << "value = " << value << endl;
           setValue(key, value);
       }
   }
   return 0;
}
