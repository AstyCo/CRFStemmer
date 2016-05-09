#include "dicts.h"
#include "helpf.h"

using namespace std;

extern int strCounter;

int groupId;

Opcorpora::Opcorpora(string dictPath)
{
    loadStems(dictPath);
    sentenceBegin = "<sentence ";
    sentenceEnd = "</sentence>";
    tokenBegin = "<token ";
    tokenEnd = "</token>";
}

size_t Opcorpora::getCorporaSize()
{
    return words.size();
}

const vector<string> & Opcorpora::getFeats(int index)
{
    return words[index].morphFeats;
}

string Opcorpora::getStem(int index)
{
    return stems[index];
}

void Opcorpora::loadStems(const string & dictPath)
{
    string line;
    ifstream ifstDic(dictPath.c_str());
    if(ifstDic.fail())
        throw "Can't open stems dictionary at path:"+dictPath;
    while(getline(ifstDic,line))
        stems.push_back(line);
    ifstDic.close();
}
void Opcorpora::printWord(int index)
{
    words[index].print();
}

void Opcorpora::sentSep()
{
    prevPunct.clear();
    OpcorpWord sentDelim;
    sentDelim[F_SENTENCE] = "SENTENCE_DELIM";
    words.push_back(sentDelim);
}


OpcorpWord Opcorpora::parseWord(string token)
{
    OpcorpWord newWord;
    size_t curPosInToken = 0;
    size_t posHelper;
    if ((curPosInToken = token.find(" text=", curPosInToken)) == string::npos)
        throw "can't find ^ text=^ at:" + token;
    curPosInToken += 7;
    posHelper = token.find("\"", curPosInToken);
    newWord[F_WORD] = token.substr(curPosInToken, posHelper - curPosInToken);
    if ((curPosInToken = token.find("<l id=", curPosInToken)) == string::npos)
        throw "can't find ^<l id=^ at:" + token;
    curPosInToken += 7;
    posHelper = token.find("\"", curPosInToken);
    newWord[F_LEMMAID] = token.substr(curPosInToken, posHelper - curPosInToken);
    while ((curPosInToken = token.find("<g v=", curPosInToken)) != string::npos)
    {
        curPosInToken += 6;
        posHelper = token.find("\"", curPosInToken);
        string featName= token.substr(curPosInToken, posHelper - curPosInToken);
        newWord[opcorpFeatMap[featName]] = featName;
    }
    newWord[F_GROUP_ID]=ntos(groupId);
    return newWord;
}

void Opcorpora::analyzeWord(const string& token)
{
    OpcorpWord newWord=parseWord(token);
    if (newWord[F_PNCT].empty())
    {
        newWord[F_PNCTL] = prevPunct;
        words.push_back(newWord);
        prevPunct.clear();
    }
    else
    {
        prevPunct = newWord[F_WORD];
        if (!words.empty())
            words[words.size() - 1][F_PNCTR] = newWord[F_WORD];
    }
}


istream& Opcorpora::push_back_line(istream& is, string& str)
{
    string line;
    if(getline(is,line))
        str+=line;
    return is;
}

string Opcorpora::getDiv(istream &is, string&line,const string& beginSeq, const string& endSeq)
{
    size_t f1,f2;
    string div;
    while(((f1=line.find(beginSeq))==string::npos||(f2=line.find(endSeq))==string::npos)\
          &&push_back_line(is,line))
        continue;
    if(!is)
        throw "wrong format of corpora, expected:\""+endSeq+'\"';
    f2-=f1;
    f2+=endSeq.length();
    div = line.substr(f1,f2);
    //cout<<"getDiv substr "<<f1<<' '<<f2<<endl<<"Token is: "<<div<<endl;
    line.erase(f1,f2);
    return div;
}

string Opcorpora::wrestDiv(string& text,const string& beginSeq,const string& endSeq)
{
    size_t f1,f2;
    string div;
    if((f1=text.find(beginSeq))!=string::npos&&(f2=text.find(endSeq))!=string::npos)
    {
        f2-=f1;
        f2+=endSeq.length();
        div = text.substr(f1,f2);
        //cout<<"wrestDiv substr "<<f1<<' '<<f2<<endl<<"Token is: "<<div<<endl;
        text.erase(f1,f2);
    }
    return div;
}


istream& Opcorpora::fillLine(istream& is, string& str,size_t windowSize)
{
    size_t readSize = windowSize;
    size_t minSize = readSize+windowSize; // for read part size + all time in memory part size
    string line;
    while(str.size()>=minSize)
        str.erase(0,readSize);
    while(str.size()<minSize&&is)
    {
        getline(is,line,'>');
        str+=line;
    }
    return is;
}

int Opcorpora::parseSentence(istream& fin,string& line)
{
    string div;
    size_t foundPos;
    while ((foundPos=line.find(sentenceBegin))==string::npos&&fin)
        fillLine (fin, line);
    if (fin)
    {
        string sentenceDiv = getDiv(fin,line,sentenceBegin,sentenceEnd);
        string token;
        while(!(token=wrestDiv(sentenceDiv,tokenBegin,tokenEnd)).empty())
            analyzeWord(token);
        sentSep();
        ++strCounter;
        if (!(strCounter % 100))
            cout << strCounter << " sentence completed\n";
        return _OK;
    }
    else
        return _EOF;
}

void Opcorpora::fillDictionary(istream& fin)
{
    string s;
    while (parseSentence(fin,s))
        continue;
    cerr << "Corpora parsed.\n";
}




