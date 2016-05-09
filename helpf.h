#ifndef HELPF_H_INCLUDED
#define HELPF_H_INCLUDED
#include <string>
#include <iostream>
#include <sstream>


using namespace std;

template <typename T>
string ntos ( T Number )
{
    ostringstream ss;
    ss << Number;
    return ss.str();
}

template <typename T>
T ston ( const string &Text )
{
    istringstream ss(Text);
    T result;
    return ss >> result ? result : 0;
}

string uint32_tToString(uint32_t cp);
bool m_isupper(string character);
bool m_islower(string character);
string m_tolower(string s);
string m_toupper(string s);

#endif // HELPF_H_INCLUDED
