#ifndef TESTBED_H_INCLUDED
#define TESTBED_H_INCLUDED

#include <string.h>
#include <tuple>

#include "dicts.h"
#include "utf8.h"
#include "crfsuite/include/crfsuite_api.hpp"

using namespace std;

typedef vector<vector<pair<feature,int>>> templateType;
typedef pair<size_t,size_t> textToken;  //   <first token index, last token index>

extern int groupId;


class MorphTrainer: public CRFSuite::Trainer
{
public:
    void message(const std::string & msg)
    {
        //  Very poor doc. about how-to-use
    }
};


class Testbed
{
public:
    MorphTrainer tr;
    CRFSuite::Tagger tg;
    Dict* corpora;
    CRFSuite::ItemSequence is;
    CRFSuite::StringList sl;
    string fnameBase;
    vector<textToken> sentTokens,wordTokens;
    unsigned nfield;
public:
    Testbed():fnameBase("model.crfsuite"),nfield(0){}
    ~Testbed(){ delete corpora;}
    void loadOpencorpora(const string &stemsPath);
    void fillCorpora(ifstream& fin);
    void addFile(string filename);
    void load(const string &CRFSFName);
    void trainInit(const string& type="lbfgs",const vector<pair<string,string>>& params = vector<pair<string,string>>());
    void train(bool ifTraining = true); //  StringList parameters
    void setNFold(unsigned nfold){nfield=nfold;}
    pair<tuple<double,double,double>,double> tag(bool ifTagging = true);//   it returns average probabilities
    pair<tuple<double,double,double>,double> probability(const CRFSuite::StringList&,const CRFSuite::StringList&);

    void goodFeatsFinder();
    static vector<pair<string,int>> getLabels(string word, string stem);
    void trainsetGenerator(const string& fname, const templateType & featureTemplate);
    tuple<CRFSuite::ItemSequence,CRFSuite::StringList> readCRFSFile(const string& fname);
};

#endif // TESTBED_H_INCLUDED
