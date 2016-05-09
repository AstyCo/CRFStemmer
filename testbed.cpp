#include <iomanip>

#include "testbed.h"
#include "helpf.h"


using namespace std;
using namespace CRFSuite;

const string beginOfWord="__BOW__",beginOfSentence="__BOS__",
            endOfWord="__EOW__",endOfSentence="__EOS__";
const string & bow=beginOfWord,bos=beginOfSentence,
            eow=endOfWord,eos=endOfSentence;


tuple<double,double,double> operator+(const tuple<double,double,double> &a, const tuple<double,double,double> &b)
{
    double a1,a2,a3;
    double b1,b2,b3;
    tie(a1,a2,a3)=a;
    tie(b1,b2,b3)=b;
    return make_tuple(a1+b1,a2+b2,a3+b3);
}

tuple<double,double,double> operator/(const tuple<double,double,double> &a, size_t n)
{
    double a1,a2,a3;
    tie(a1,a2,a3)=a;
    if(!n)
        throw "divide by zero";
    return make_tuple(a1/n,a2/n,a3/n);
}

template <typename T,typename U>
std::pair<T,U> operator+(const std::pair<T,U> & l,const std::pair<T,U> & r)
{
    return {l.first+r.first,l.second+r.second};
}
template <typename T,typename U>
std::pair<T,U> operator/(const std::pair<T,U> & l,size_t n)
{
    return {l.first/n,l.second/n};
}

void Testbed::addFile(string filename)
{
    ifstream ftext(filename);
    if (ftext.fail())
        throw "Can't open the file \"" + filename + "\" ." ;
    fillCorpora(ftext);
}

void Testbed::loadOpencorpora(const string &stemsPath)
{
    corpora = new Opcorpora(stemsPath);
}
void Testbed::fillCorpora(ifstream& fin)
{
    corpora->fillDictionary(fin);
}
vector<pair<string,int>> Testbed::getLabels(string word, string stem)
{
    vector<pair<string,int>> res;
    int lab;
    std::string::iterator a0 = word.begin(),b0 = stem.begin();
    std::string::iterator a = word.begin(),b = stem.begin();
    std::string::iterator ae = word.end(),be = stem.end();
    while ( a != ae ) {
        utf8::next(a,ae);
        lab=((stem!="#")&&(b!=be))?1:0;
        if(b!=be) utf8::next(b,be);
        res.push_back(pair<string,int>(string(a0,a),lab));
        a0=a;
    }
    return res;
}
void Testbed::trainsetGenerator(const string& fname, const templateType & featureTemplate)
{
    size_t corporaSize=corpora->getCorporaSize();
    pair<size_t,size_t> sentenceBorders(0,0);
    ofstream fout(fname);
    if(fout.fail())
        throw "Can't open file [-w mode] "+fname;
    for(size_t i=0;i<corporaSize;++i)
    {
        const vector<string> & iFeats=corpora->getFeats(i);
        vector<pair<string,int>> labels=getLabels(iFeats[F_WORD],corpora->getStem(ston<int>(iFeats[F_LEMMAID])));
        bool firstLetter=true;
        if(!i||!iFeats[F_SENTENCE].empty())
        {
            size_t k=i+1;
            while(k<corporaSize)
            {
                const vector<string> & iFeats=corpora->getFeats(k);
                vector<pair<string,int>> labels=getLabels(iFeats[F_WORD],corpora->getStem(ston<int>(iFeats[F_LEMMAID])));
                if(!iFeats[F_SENTENCE].empty())
                    break;
                ++k;
            }
            sentenceBorders=pair<size_t,size_t>( i?i+1:i , k-1);
            if(i) continue;
        }
        for(size_t j=0;j<labels.size();++j)
        {
            string generatedLabel;
            generatedLabel+=ntos<int>(labels[j].second);
            for(auto feat: featureTemplate)
            {
                string f_elems,f_values;
                bool first=true;
                bool notBroken=true;
                for(auto featPiece: feat)
                {
                    feature f=featPiece.first;
                    int offset=featPiece.second;
                    string feat_value;
                    if(f==F_LETTER)
                    {
                        if(j+offset>=0&&j+offset<labels.size())
                        {
                            feat_value=labels[j+offset].first;
                            feat_value=m_tolower(feat_value); // All characters lowercase!!
                        }
                        else
                        {
                            notBroken=false;
                            break;
                        }
                    }
                    else if(i+offset>=sentenceBorders.first&&i+offset<=sentenceBorders.second)
                    {
                        feat_value=corpora->getFeats(i+offset)[f];
                        if(f==F_WORD)
                            feat_value=m_tolower(feat_value);
                    }
                    else
                    {
                        notBroken=false;
                        break;
                    }

                    if(feat_value.empty())
                    {
                        notBroken=false;
                        break;
                    }
                    f_elems+=(first?"":"|")+featNames[f]+"["+ntos<int>(offset)+"]";
                    f_values+=(first?"":"|")+feat_value;
                    first=false;
                }
                if(notBroken)
                {
                    generatedLabel+="\t"+f_elems+"="+f_values;
                }
            }
            if(firstLetter)
            {
                generatedLabel+=string("\t")+beginOfWord;
                if(i==sentenceBorders.first)
                    generatedLabel+=string("\t")+beginOfSentence;
                firstLetter=false;
            }
            if(j==labels.size()-1)
                generatedLabel+=string("\t")+endOfWord;
            if(i==sentenceBorders.second&&j==labels.size()-1)
            {
                generatedLabel+=string("\t")+endOfSentence;
            }

            fout<<generatedLabel<<endl;
        }
    }
    cout<<"created an appropriate CRFSuite file"<<endl;
}

vector<textToken> getTokens(const ItemSequence& is,const string &startSeq,const string &endSeq)
{
    vector<textToken> res;
    size_t startId=0;
    for(size_t i=0;i<is.size();++i)
    {
        for(const Attribute & j:is[i])
        {
            if(!j.attr.compare(startSeq))
                startId=i;
            if(!j.attr.compare(endSeq))
                res.push_back(make_pair(startId,i));
        }
    }
    return res;
}

void probOut(ostream& ofs,const pair<tuple<double,double,double>,double>& probs,const string & s = "")
{
    double prob0,prob1,probAll;
    tie(prob0,prob1,probAll) = probs.first;
    double probWord=probs.second;
    ofs<<setprecision(4)<<s<<(s.empty()?"":" : ")<<
        ntos(prob0)<<" , "<<ntos(prob1)<<" , "<<
        ntos(probAll)<<" , "<<ntos(probWord)<<endl;
}

void Testbed::goodFeatsFinder()
{
    ofstream flog("feats.res");
   // 1,4,6,7,9-20 feats in feature
    vector<feature> feats=  {   F_LETTER,
                                F_WORD,
                                F_PNCTL,
                                F_PNCTR,
                                F_POST,
                                F_ANIM,
                                F_GNDR,
                                F_NMBR,
                                F_CASE,
                                F_ASPC,
                                F_TRNS,
                                F_PERS,
                                F_TENS,
                                F_MOOD,
                                F_INVL,
                                F_VOIC
                            };
    vector<vector<int>> indices={   { -1 },
                                    { 0 },
                                    { 1 },
                                    { -1 , 0 },
                                    { -1 , 1 },
                                    {  0 , 1 },
                                    { -1 , 0, 1 }
                                };
    string fnameBase = "CRFS.";
    string tailFeat,tailInd;
    string CRFSFilename;
    pair<tuple<double,double,double>,double> probs;

    // ONLY BOW/BOS/EOS/EOW
    {
        CRFSFilename=fnameBase+"empty";
        tailFeat="empty";
        tailInd="";
        trainsetGenerator(CRFSFilename,{{}});
        load(CRFSFilename);
        nfield=4;
        train(true);
        probs=tag(true);

        double prob0,prob1,probAll;
        tie(prob0,prob1,probAll) = probs.first;
        double probWord=probs.second;
        flog<<tailFeat<<tailInd<<" : "<<
            ntos(prob0)<<" , "<<ntos(prob1)<<" , "<<
            ntos(probAll)<<" , "<<ntos(probWord)<<endl;
        cout<<tailFeat<<tailInd<<" : "<<
            ntos(prob0)<<" , "<<ntos(prob1)<<" , "<<
            ntos(probAll)<<" , "<<ntos(probWord)<<endl;
    }

    for(auto i: feats)
    {
        for(size_t j=0;j<indices.size();++j)
        {
            templateType tmpl;
            vector<pair<feature,int>> tmplItem;
            tailInd.clear();
            for(auto k: indices[j])
            {
                tailInd+="_"+ntos(k);
                tmplItem.push_back(make_pair(i , k));
            }
            tmpl.push_back(tmplItem);
            tailFeat=featNames[i];
            CRFSFilename=fnameBase+tailFeat+tailInd;

            trainsetGenerator(CRFSFilename,tmpl);
            load(CRFSFilename);
            bool training=true,tagging=true;
            train(training);
            probs=tag(tagging);

            double prob0,prob1,probAll;
            tie(prob0,prob1,probAll) = probs.first;
            double probWord=probs.second;
            flog<<tailFeat<<tailInd<<" : "<<
                ntos(prob0)<<" , "<<ntos(prob1)<<" , "<<
                ntos(probAll)<<" , "<<ntos(probWord)<<endl;
            cout<<tailFeat<<tailInd<<" : "<<
                ntos(prob0)<<" , "<<ntos(prob1)<<" , "<<
                ntos(probAll)<<" , "<<ntos(probWord)<<endl;
        }
    }

}


tuple<ItemSequence,StringList> Testbed::readCRFSFile(const string& fname)
{
    CRFSuite::ItemSequence xseq;
    CRFSuite::StringList yseq;
    ifstream fin(fname);
    if(!fin)
        throw "can't open CRFSFile"+fname;
    string line;
    while(getline(fin,line))
    {
        CRFSuite::Item xseqIt;
        stringstream s(line);
        string word;
        for (size_t i = 0; s >> word; i++)
        {
            if (!i)
            {
                yseq.push_back(word);
            }
            else
            {
                xseqIt.push_back(Attribute(word));
            }
        }
        xseq.push_back(xseqIt);
    }
    return make_tuple(xseq,yseq);
}

void Testbed::trainInit(const string& type,const vector<pair<string,string>>& params)
{
    tr.clear();
    if(!tr.select(type,"crf1d"))
        throw "can't init training algorithm <"+type+",crf1d>";

    for(size_t i=0;i<params.size();i++)
        tr.set(params[i].first,params[i].second);

    sentTokens=getTokens(is,bos,eos);
}
void Testbed::load(const string &CRFSFName)
{
    tuple<ItemSequence,StringList> data = readCRFSFile(CRFSFName);
    tie(is,sl) = data;
    cout<<"training files loaded\n";
}
void Testbed::train(bool ifTrain)
{

    if(!ifTrain)
        return;
    string tail("");
    //tmp without cross-validation

    trainInit();

    if(is.size()!=sl.size())
            throw "different ItemSequence and StringList sizes to append()\n";

    if(!nfield)
    {
        tr.append(is,sl,0);
        tr.train(fnameBase+tail,-1);
    }
    else
    {
        double step=sentTokens.size()/nfield;
        auto itx=is.begin();
        auto ity=sl.begin();

        for(unsigned j=0;j<nfield;++j)
        {
            size_t a0=j*step,  a1=(j==nfield-1)?size_t(sentTokens.size()-1):size_t((j+1)*step-1);
            size_t b0=sentTokens[a0].first,b1=sentTokens[a1].second;
            ItemSequence partIS(itx+b0,itx+b1);
            StringList partSL(ity+b0,ity+b1);

            tr.append(partIS,partSL,j);
        }
        // Result of cycle - <nfield> of models
        for(unsigned j=0;j<nfield;++j)
        {
            tail=ntos<int>(j);
            tr.train(fnameBase+tail,j);
        }
    }
    cout<<"Training Completed\n";
}

pair<tuple<double,double,double>,double> Testbed::tag(bool doTagging)
{
    ofstream flog("CRF_stemmer.log");
    tg.close();
    if(!doTagging)
        return make_pair(make_tuple(0.0,0.0,0.0),0.0);
    string tail;
    size_t j=0;
    double step;
    auto itx=is.begin();
    auto ity=sl.begin();
    pair<tuple<double,double,double>,double> averageProbs=
        make_pair(make_tuple(0.0,0.0,0.0),0.0);
    if(!nfield)
    {
        tail="";
        step=1;
    }
    else
    {
        tail="0";
        step=sentTokens.size()/nfield;
        cout<<nfield<<"-fold cross-validation:"<<endl;
        cout<<"(labels's \"0\" #prob , labels's \"1\" #prob ,"<<
            " total labels's #prob , word's #prob)"<<endl;
    }
    do
    {
        StringList labelsRes;
        size_t a0=j*step,  a1=(nfield==0||j==nfield-1)?size_t(sentTokens.size()-1):size_t((j+1)*step-1);
        size_t b0=sentTokens[a0].first,b1=sentTokens[a1].second;
        ItemSequence partIS(itx+b0,itx+b1);
        StringList partSL(ity+b0,ity+b1);

        if(!tg.open(fnameBase+tail))
            throw "can't open model file"+fnameBase+tail;

        //tg.set(partIS);
        labelsRes=tg.tag(partIS);

        wordTokens=getTokens(partIS,bow,eow);
        auto probs = probability(labelsRes,partSL);
        averageProbs=probs+averageProbs;
        double prob0,prob1,probAll;
        tie(prob0,prob1,probAll)=probs.first;
        cout<<endl;
        probOut(cout,probs);
        tail=ntos(++j);
    }   while(j<nfield);
    averageProbs=averageProbs/(nfield?nfield:1);
    if(nfield)
    {
        cout<<endl;
        probOut(cout,averageProbs,"avg");
    }
    cout<<"Tagging completed\n";
    return averageProbs;
}

pair<tuple<double,double,double>,double> Testbed::probability(const CRFSuite::StringList& tagTokens,const CRFSuite::StringList& correctTokens)
{
    const size_t tokenCnt=tagTokens.size(),wordCnt=wordTokens.size();
    size_t corrTokenCnt=0,corrWordCnt=0;
    size_t tok0Cnt=0,tok1Cnt=0,corrTok0Cnt=0,corrTok1Cnt=0;
    for(auto &&i: wordTokens)
    {
        bool wordIsCorrect = true;
        for(size_t j=i.first;j<=i.second;++j)
        {
            if(tagTokens[j].compare(correctTokens[j]))
            {
                wordIsCorrect=false;
            }
            else
            {
                corrTokenCnt++;
                if(!correctTokens[j].compare("0"))
                    corrTok0Cnt++;
                else
                    corrTok1Cnt++;
            }
            if(!correctTokens[j].compare("0"))
                tok0Cnt++;
            else
                tok1Cnt++;
        }
        if(wordIsCorrect)
        {
            corrWordCnt++;
        }
    }
    if(!tok0Cnt||!tok1Cnt)
        throw "wrong learning set: only 1 type of labels in selection";
    auto tokResProbs=make_tuple((1.0*corrTok0Cnt)/tok0Cnt,(1.0*corrTok1Cnt)/tok1Cnt,(1.0*corrTokenCnt)/tokenCnt);
    auto wordResProbs=(1.0*corrWordCnt)/wordCnt;
    return make_pair(tokResProbs,wordResProbs);
}
