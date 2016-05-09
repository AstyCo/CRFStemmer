#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>

#include <locale>
#include <exception>
#include <typeinfo>
#include <stdexcept>
#include <string.h>

#include "utf8.h"

#include "main.h"
#include "helpf.h"
#include "testbed.h"


using namespace std;
int strCounter;

const int featureSize = F_NR_ITEMS;

extern map<feature,string> featNames;
extern map <string, feature> opcorpFeatMap;

typedef pair<feature,int> f_tmpl;
string easyInterface(int argc, const char**argv, Testbed&);

void execution(int argc, const char **argv)
{
    Testbed tb;
    string outputFilename = easyInterface(argc,argv,tb);


    templateType tmpl={
            { f_tmpl (F_LETTER , 0 ) } ,
            { f_tmpl (F_LETTER , -1 ),f_tmpl (F_LETTER , 0 ) } ,
            { f_tmpl (F_LETTER , 0 ), f_tmpl (F_LETTER , 1 ) } ,
            { f_tmpl (F_WORD , 0 ) } ,
            { f_tmpl (F_WORD , -1 ) } ,
            { f_tmpl (F_WORD , 1 ) }
        //  { f_tmpl (F_POST , 0 ) } ,
        //  { f_tmpl (F_POST , -1 ) } ,
        //  { f_tmpl (F_POST , 1 ) }
    };

    //L-BFGS parameters
    /*
    tb.tr.set("feature.minfreq","0");
    tb.tr.set("feature.possible_states","0");
    tb.tr.set("feature.possible_transitions","0");
    tb.tr.set("c1","0");
    tb.tr.set("c2","1");
    tb.tr.set("max_iterations","INT_MAX");
    tb.tr.set("num_memories","6");
    tb.tr.set("epsilon","1e-5");
    tb.tr.set("delta","1e-5");
    tb.tr.set("linesearch","MoreThuente");
    tb.tr.set("max_linesearch","20");
    */

    tb.trainsetGenerator(outputFilename,tmpl);
    tb.load(outputFilename);
    bool training=true,tagging=true;
    tb.train(training);
    tb.tag(tagging);
}


int main(int argc, const char**argv)
{
	try
	{
	    cout << "----BEGIN----" << endl;

        execution(argc,argv);

		cout << "----END----" << endl;
	}
	catch (const char* s)
	{
		cout << "caught:\n";
		cout << s << endl;
	}
	catch (const std::runtime_error& re)
	{
		// specific handling for runtime_error
		std::cerr << "Runtime error: " << re.what() << std::endl;
	}
	catch (const std::exception& ex)
	{
		// specific handling for all exceptions extending std::exception, except
		// std::runtime_error which is handled explicitly
		std::cerr << "Error occurred: " << ex.what() << std::endl;
	}
	catch (...)
	{
		// catch any other errors (that we have no information about)
		std::cerr << "Unknown failure occured. Possible memory corruption" << std::endl;
	}
	return 0;
}
