#include "ERDict.h"
#include "locale.h"

int main() {
    ERDict dict;
    //setlocale(LC_ALL, "ru_RU.UTF-8");
    dict.insert("apple", "apple");
    //dict.print(std::cout);
    dict.insert("apple", "aplle");
    //dict.print(std::cout);
    dict.insert("banana", "banan");
    //dict.print(std::cout);
    dict.insert("cherry", "c");
    //dict.print(std::cout);
    dict.insert("grape", "vino");
    //dict.print(std::cout);
    dict.insert("pear", "pear");
    //dict.print(std::cout);
    //std::cout << "Translations of 'apple': " << '\n';
    StringVector translations = dict.search("apple");
    //std::cout << translations;
    //std::cout << '\n';
    dict.remove("cherry");
    //dict.print(std::cout);
    dict.removeTranslation("apple", "tangerine");
    //dict.print(std::cout);
    dict.removeTranslation("apple", "apple");
    dict.print(std::cout);
    return 0;
}