#ifndef DICTS_H_INCLUDED
#define DICTS_H_INCLUDED


#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include "main.h"

using namespace std;

extern map<feature,string> featNames;
extern map <string, feature> opcorpFeatMap;

class ParsedWord
{
public:
	vector<string> morphFeats;
	ParsedWord(){morphFeats.resize(F_NR_ITEMS);}
	virtual void print()
	{
		cout << "Parsed Word" << endl;
		for (auto i : morphFeats)
			cout << i << endl;
	}
	string & operator[](feature f) { return morphFeats[f]; }
};

class OpcorpWord : public ParsedWord
{
public:
	OpcorpWord(){}
};

class Dict
{
protected:
	string sentenceBegin;
	string sentenceEnd;
	string tokenBegin;
	string tokenEnd;
	string wordBegin;
	string lemmaIdBegin;
	string featureBegin;

	virtual int parseSentence(istream& ,string&) = 0;
public:
    virtual size_t getCorporaSize() = 0;
    virtual const vector<string> & getFeats(int) = 0;
    virtual string getStem(int) = 0;
    virtual void printWord(int) = 0;
	virtual void fillDictionary(istream& fin) = 0;
	virtual ~Dict(){};
};


class Opcorpora : public Dict
{
	vector<string> stems;
	string prevPunct;
    vector<OpcorpWord> words;
    int parseSentence(istream& ,string&);
public:

	Opcorpora(string dictPath);
    size_t getCorporaSize();
    const vector<string> & getFeats(int index);
    string getStem(int index);
	void loadStems(const string & dictPath);
	void printWord(int index);
	void sentSep();
	OpcorpWord parseWord(string token);
	void analyzeWord(const string& token);
    istream& push_back_line(istream& is, string& str);
    string getDiv(istream &is, string&line,const string& beginSeq, const string& endSeq);
    string wrestDiv(string& text,const string& beginSeq,const string& endSeq);
    istream& fillLine(istream& is, string& str,size_t windowSize = 100);
	void fillDictionary(istream& fin);
};



#endif // DICTS_H_INCLUDED
