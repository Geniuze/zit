#ifndef __BUFFER_H__
#define __BUFFER_h__

#define BUF_SIZE_1024 20480

class buffer
{
    char *head;
    char *tail;
    char *ptr;
    size_t length;
    size_t size;
    size_t offset;

public:

    buffer()
    {
        head = new char[BUF_SIZE_1024];
        memset(head, 0, BUF_SIZE_1024);
        size = BUF_SIZE_1024;
        length = 0;
        offset = 0;
        tail = head;
        ptr = head;
    }
    ~buffer()
    {
        delete[] head;
    }
    int retrieve8(unsigned char &c)
    {
        bcopy(ptr, &c, 1);
        ptr ++;
        offset ++;
        return 0;
    }
    int retrieve16(unsigned short &s)
    {
        bcopy(ptr, &s, 2);
        s = ntohs(s);

        ptr += 2;
        offset += 2;
        return 0;
    }
    int retrieve32(unsigned int &i)
    {
        bcopy(ptr, &i, 4);
        i = ntohl(i);
        ptr += 4;
        offset += 4;
        return 0;
    }
    int retrieve64(unsigned long long &l)
    {
        bcopy(ptr, &l, 8);
        ptr += 8;
        offset += 8;
        return 0;
    }
    int retrieveRawBytes(unsigned char *raw, size_t len)
    {
        if (NULL != raw)
        {
            bcopy(ptr, raw, len);
        }
        ptr += len;
        offset += len;
        return 0;
    }
    int store8(unsigned char c)
    {
        bcopy(&c, ptr, 1);

        length ++;
        ptr ++;
        offset ++;
        return 0;
    }
    int store16(unsigned short s)
    {
        s = htons(s);
        bcopy(&s, ptr, 2);

        length += 2;
        ptr += 2;
        offset += 2;
        return 0;
    }
    int store32(unsigned int i)
    {
        i = htonl(i);
        bcopy(&i, ptr, 4);

        length += 4;
        ptr += 4;
        offset += 2;
        return 0;
    }
    int store64(unsigned long long l)
    {
        bcopy(&l, ptr, 8);

        length += 8;
        ptr += 8;
        offset += 2;
        return 0;
    }
    int storeRawBytes(unsigned char *raw, size_t len)
    {
        bcopy(raw, ptr, len);

        length += len;
        ptr += len;
        offset += len;
        return 0;
    }

    char * GetPtr()
    {
        return ptr;
    }
    char * GetHead()
    {
        return head;
    }
    size_t GetSize()
    {
        return size;
    }
    size_t GetLength()
    {
        return length;
    }
    size_t GetOffset()
    {
        return offset;
    }

    size_t SetLength(size_t len)
    {
        length = len;
        tail = head + len;

        return length;
    }

    size_t BackOffset(size_t len)
    {
        offset -= len;
        ptr -= len;

        return offset;
    }
    size_t Skip(size_t len)
    {
        ptr += len;
        offset += len;

        return offset;
    }
    size_t BackToHead()
    {
        ptr = head;
        offset = 0;

        return offset;
    }
    void reSize(size_t nsize)
    {
        delete [] head;
        head = new char[nsize];
        memset(head, 0, nsize);
        size = nsize;
        length = 0;
        offset = 0;
        tail = head;
        ptr = head;
    }

};

#endif
