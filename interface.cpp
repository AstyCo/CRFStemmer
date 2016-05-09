#include "testbed.h"
#include "helpf.h"


string easyInterface(int argc, const char**argv,Testbed &tb)
{

    setlocale(LC_ALL, "");

    string defaultInput("annot.opcorpora.no_ambig.xml"),
            textFilename,
            trainFilename,
            testFilename,
            outputFilename("test.crfsuite.txt"),
            filelistName,
            stemDB="stems.txt";
    vector<string> inFileNames;

    tb.loadOpencorpora(stemDB); // Initialize the testbed

    for(int i = 1; i < argc ; ++i)
    {
        if(!strcmp(argv[i],"-o"))
        {
            if(argc==i+1)
                throw "<format:> testbed [-o \"output_file\"]";
            outputFilename=argv[++i];
            continue;
        }
        if(!strcmp(argv[i],"-n"))
        {
            if(argc==i+1)
                throw "<format:> testbed [-o \"output_file\"]";
            tb.nfield=ston<unsigned>(argv[++i]);
            continue;
        }
        if(!strcmp(argv[i],"-r"))
        {
            if(argc==i+1)
                throw "<format:> testbed [-r \"filename\"]";
            filelistName=argv[++i];
            {
                string filename;
                ifstream fin(filelistName);
                if (fin.fail())
                    throw "Can't open the file \"" + filelistName + "\" ." ;
                while(getline(fin,filename))
                    inFileNames.push_back(filename);
            }
            continue;
        }

        textFilename=argv[i];
        inFileNames.push_back(textFilename);
    }

    // ---------------------------------------
    inFileNames.push_back("annot.opcorpora.no_ambig.xml");
    //inFileNames.push_back("annot.opcorpora.no_ambig.xml");

    if(inFileNames.empty())
        tb.addFile(defaultInput);
    else
        for(size_t i=0;i<inFileNames.size();++i)
        {
            groupId=i;
            tb.addFile(inFileNames[i]);
        }
    return outputFilename;
}
