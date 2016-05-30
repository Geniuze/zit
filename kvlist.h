
#ifndef __KVLIST_H__
#define __KVLIST_H__

class kvlist
{
    map<string,string> maps;
public:
    void setValue(string& key, string& value)
    {
        maps[key] = value;
    }
    string getValue(string key)
    {
        if (maps.find(key) == maps.end())
            return "";
        return maps[key];
    }
    int GetKvFromStr(string s);
};

#endif
