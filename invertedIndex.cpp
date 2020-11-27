#include <iostream>
#include <fstream>
#include <algorithm>

#include "stopwords.hpp"
#include "invertedIndex.hpp"


typedef std::pair<std::string, int> pairT; // {DocumentName, lineNumber}
typedef std::map <std::string, std::vector<pairT>> invertedIdxDS;

std::string InvertedIndex::preprocessToken(std::string token) {
    // Remove the characters such as ., at the end of the token
    if (symbols.find(token.at(token.size() - 1)) != symbols.end()) {
        token = token.substr(0, token.size() - 1);
    }
    // convert to lowercase
    std::transform(token.begin(), token.end(), token.begin(),
                   [](unsigned char c){ return std::tolower(c);});
    return token;
}

void InvertedIndex::addToInvertedIdx(std::string documentName,
                                     std::string line, 
                                     int lineNumber) {
    auto position = 0;
    while ((size_t)(position = line.find(" ")) != std::string::npos) {
        std::string token = line.substr(0, position);
        std::cout << "token: " << token << "\n";
        std::string preprocessedToken = preprocessToken(token);
        if (preprocessedToken.size() > 0) {
#ifdef INCLUDE_STOPWORDS
            if (stopWords.find(preprocessedToken) != stopWords.end()) {
#else
            if (false) {
#endif
                std::cout << token << " is a stop word, so not adding to the data structure\n";
            } else {
                std::cout << token << " is not stop word, so adding to the data structure\n";
                // New Entry
                if (invertedIdx.find(preprocessedToken) == invertedIdx.end()) {
                    invertedIdx.insert({preprocessedToken, std::vector<pairT>{pairT{documentName, lineNumber}}});
                } else {
                    // Token is already there. Add the values {documentName, lineNumber}
                    invertedIdx.at(preprocessedToken).push_back(pairT{documentName, lineNumber});
                }
            }
        }
        line = line.substr(token.size() + 1, line.size() - token.size() - 1);
        position = line.find(" ");
    }
}

void InvertedIndex::scanDocuments(std::string documentName) {
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
            addToInvertedIdx(documentName, line, lineNumber);
        }
    }
}

void InvertedIndex::findToken(std::string token) {
    std::cout << "findToken()\n";
    auto preprocessedToken = preprocessToken(token);
    if (invertedIdx.find(preprocessedToken) != invertedIdx.end()) {
        std::cout << "\"" << token << "\"" << " found in:\n";
        for (auto val:invertedIdx.at(preprocessedToken)) {
            std::cout << ">>>>>Document " << "\"" << val.first << "\"" << " in line " << val.second << "\n";
        }
    } else {
#ifdef INCLUDE_STOPWORDS
        if (stopWords.find(preprocessToken(token)) != stopWords.end()) {
#else
        if (false) {
#endif
            std::cout << ">>>>>>You are searching for a stop word\n";
        } else {
            std::cout << "\"" << token << "\"" << " not found!!!!!\n";
        }
    }
}

void InvertedIndex::print() {
    std::cout << "printInvertedIdxDS\n";
    for (auto val:invertedIdx) {
        std::cout << val.first << " ==> ";
        for (auto element:val.second) {
            std::cout << "{" << element.first << ":" << element.second << "}" << ",";
        }
        std::cout << "\n";
    }
}
