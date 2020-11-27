#include <iostream>
#include "invertedIndex.hpp"

int main() {
    std::cout << "Inverted Index\n";
    InvertedIndex invertedIndex;
    invertedIndex.scanDocuments("/home/ppa/Documents/invertedIndex/invertedIdx/sample1.txt");
    invertedIndex.print();
    std::string token;
    invertedIndex.findToken("Hello");
    while (true) {
        std::cout << "Enter token to search:\n";
        std::cin >> token;
        invertedIndex.findToken(token);
    }
}
