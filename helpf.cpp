
#include <iterator>

#include "utf8.h"

#include "helpf.h"



string uint32_tToString(uint32_t cp)
{
    char u[5] = {0,0,0,0,0};
    utf8::append(cp, u);
    return u;
}

bool m_isupper(string character) //works only for 1 character test function1
{
    uint32_t cp = 0;
    std::string::iterator b = character.begin();
    std::string::iterator e = character.end();
    while ( b != e )
    {
        cp = utf8::next(b,e);
    }
    if(cp<=0x042F&&cp>=0x0410)
        return true;
    if(cp==0x0401)
        return true;
    return false;
}

bool m_islower(string character)  //works only for 1 character test function2
{
    uint32_t cp = 0;
    std::string::iterator b = character.begin();
    std::string::iterator e = character.end();
    while ( b != e )
    {
        cp = utf8::next(b,e);
    }
    if(cp<=0x044F&&cp>=0x0430)
        return true;
    if(cp==0x0451)
        return true;
    return false;
}

string m_tolower(string s)
{
    string res;
    uint32_t cp = 0;
    std::string::iterator b0 = s.begin();
    std::string::iterator b = s.begin();
    std::string::iterator e = s.end();
    while ( b != e )
    {
        b0=b;
        bool f=true;
        cp = utf8::next(b,e);
        if(cp<=0x042F&&cp>=0x0410)
        {
            res+=uint32_tToString(cp+0x0020);
            f=false;
        }
        if(cp==0x0401)
        {
            res+=uint32_tToString(0x0451);
            f=false;
        }
        if(f) res+=string(b0,b);
    }
    return res;
}

string m_toupper(string s)
{
    string res;
    uint32_t cp = 0;
    std::string::iterator b0 = s.begin();
    std::string::iterator b = s.begin();
    std::string::iterator e = s.end();
    while ( b != e )
    {
        b0=b;
        bool f=true;
        cp = utf8::next(b,e);
        if(cp<=0x044F&&cp>=0x0430)
        {
            res+=uint32_tToString(cp-0x0020);
            f=false;
        }
        if(cp==0x0451)
        {
            res+=uint32_tToString(0x0401);
            f=false;
        }
        if(f) res+=string(b0,b);
    }
    return res;
}

