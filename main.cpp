#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <list>
#include <utility>

#include "stopwords.hpp"

typedef std::pair<std::string, int> pairT; // {DocumentName, lineNumber}
typedef std::map <std::string, std::vector<pairT>> invertedIdxDS;

void addToInvertedIdx(invertedIdxDS& invertedIdx,
                      std::string documentName,
                      std::string line, int lineNumber) {
    auto position = 0;
    while ((size_t)(position = line.find(" ")) != std::string::npos) {
        std::string token = line.substr(0, position);
        std::cout << "token: " << token << "\n";
        if (token.size() > 0) {
            // Remove the characters such as ., at the end of the token
            if (symbols.find(token.at(token.size() - 1)) != symbols.end()) {
                token = token.substr(0, token.size() - 1);
            }
            // New Entry
            if (invertedIdx.find(token) == invertedIdx.end()) {
                invertedIdx.insert({token, std::vector<pairT>{pairT{documentName, lineNumber}}});
            } else {
                // Token is already there. Add the values {documentName, lineNumber}
                invertedIdx.at(token).push_back(pairT{documentName, lineNumber});
            }
        }
        line = line.substr(token.size() + 1, line.size() - token.size() - 1);
        position = line.find(" ");
    }
}

void scanDocuments(std::string documentName,
                   invertedIdxDS& invertedIdx) {
    std::cout << "scanDocuments\n";
    std::ifstream documentStream(documentName);
    if (!documentStream) {
        std::cerr << "Document " << documentName << " not available\n";
        return;
    }
    std::string token;
    unsigned int lineNumber = 0;
    std::string line;

    while(std::getline(documentStream, line)) {
        std::cout << line << "\n";
        if (!line.empty()) {
            lineNumber++;
            addToInvertedIdx(invertedIdx, documentName, line, lineNumber);
        }
    }
}

void findToken(std::string token,
               invertedIdxDS& invertedIdx) {
    std::cout << "findToken()\n";
    if (invertedIdx.find(token) != invertedIdx.end()) {
        std::cout << "\"" << token << "\"" << " found in:\n";
        for (auto val:invertedIdx.at(token)) {
            std::cout << "Document " << "\"" << val.first << "\"" << " in line " << val.second << "\n";
        }
    } else {
        std::cout << "\"" << token << "\"" << " not found\n";
    }
}

void printInvertedIdxDS(invertedIdxDS& invertedIdx) {
    std::cout << "printInvertedIdxDS\n";
    for (auto val:invertedIdx) {
        std::cout << val.first << " ==> ";
        for (auto element:val.second) {
            std::cout << "{" << element.first << ":" << element.second << "}" << ",";
        }
        std::cout << "\n";
    }
}

int main() {
    std::cout << "Inverted Index\n";
    invertedIdxDS invertedIdx;
    scanDocuments("/home/ppa/Documents/invertedIndex/invertedIdx/sample1.txt", invertedIdx);
    printInvertedIdxDS(invertedIdx);
    std::string token;
    findToken("Hello", invertedIdx);
    while (true) {
        std::cout << "Enter token to search:\n";
        std::cin >> token;
        findToken(token, invertedIdx);
    }
}
