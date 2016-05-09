# CRFStemmer
coursework

Stems.txt important

To run the project need first to install CRFSuite libraries annd Liblbfgs libraries.
For this download files

  https://github.com/downloads/chokkan/crfsuite/crfsuite-0.12.tar.gz
  https://github.com/downloads/chokkan/liblbfgs/liblbfgs-1.10.tar.gz
  
and follow the instructions at the section "Building binaries from the source distribution"
  http://www.chokkan.org/software/crfsuite/manual.html
  
To compile project using g++ command may look like this
  g++ --std=c++11 *.cpp -o CRFStemmer -L<path_to_lib> -llbfgs -lcqdb -lcrfsuite -Wl,-rpath=<path_to_lib>
, where <path_to_lib> - path to the directory stores libraries above.
(\home\astyco\local\lib in my case)
