#ifndef INVERTEDINDEX_HPP
#define INVERTEDINDEX_HPP

#include <map>
#include <utility>
#include <vector>
#include <string>

#define DEBUG 
//#define INCLUDE_STOPWORDS 

typedef std::pair<std::string, int> pairT; // {DocumentName, lineNumber}
typedef std::map <std::string, std::vector<pairT>> invertedIdxDS;

class InvertedIndex {
    private:
        invertedIdxDS invertedIdx;
    public:
        std::string preprocessToken(std::string token);
        void addToInvertedIdx(std::string documentName, 
                              std::string line,
                              int lineNumber);
        void scanDocuments(std::string documentName);
        void findToken(std::string token);
        void print();
};

#endif