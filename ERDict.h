#ifndef ENGLISH_RUSSIAN_DICTIONARY_HPP
#define ENGLISH_RUSSIAN_DICTIONARY_HPP
#include "StringVector.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
// Структура для хранения слова и его переводов


// Узел AVL-дерева
template <typename InputIterator, typename T>
InputIterator myFind(InputIterator first, InputIterator last, const T& value) {
    while (first != last && *first != value) 
    {
        ++first;
    }
    return first;
}

class ERDict {
public:
    ERDict();
    ~ERDict();
    void insert(const std::string& word, const std::string& translation);
    void remove(const std::string& word);
    void removeTranslation(const std::string& word, const std::string& translation);
    StringVector search(const std::string& word);
    void print(std::ostream& out) const;
private:
    struct Word {
        std::string word_;
        StringVector translations_;

    };
    struct Node {
        Word data_;
        Node* left_;
        Node* right_;
        int height_;
    };
    Node* root_;
    Node* createNode(const std::string& word, const std::string& translation);
    void clear(Node* node);
    int getHeight(Node* node);
    int getBalance(Node* node);
    void fixHeight(Node* node);
    Node* rotateRight(Node* node);
    Node* rotateLeft(Node* node);
    Node* balance(Node* node);
    Node* insert(Node* node, const std::string& word, const std::string& translation);
    Node* findMin(Node* node);
    Node* removeMin(Node* node);
    Node* remove(Node* node, const std::string& word);
    void printNode(const Node* node) const;
};

ERDict::ERDict() :
    root_(nullptr)
{}

ERDict::~ERDict() {
    clear(root_);
}

void ERDict::insert(const std::string& word, const std::string& translation) {
    root_ = insert(root_, word, translation);
}

ERDict::Node* ERDict::createNode(const std::string& word, const std::string& translation) {
    Node* node = new Node();
    node->data_.word_ = word;
    node->data_.translations_.push_back(translation);
    node->left_ = nullptr;
    node->right_ = nullptr;
    node->height_ = 1;
    return node;
}

void ERDict::clear(Node* node) {
    if (node == nullptr)
        return;
    clear(node->left_);
    clear(node->right_);
    delete node;
}

int ERDict::getHeight(Node* node) {
    return node ? node->height_ : 0;
}

int ERDict::getBalance(Node* node) {
    return getHeight(node->right_) - getHeight(node->left_);
}

void ERDict::fixHeight(Node* node) {
    node->height_ = std::max(getHeight(node->left_), getHeight(node->right_)) + 1;
}

ERDict::Node* ERDict::rotateRight(Node* node) {
    Node* left = node->left_;
    node->left_ = left->right_;
    left->right_ = node;
    fixHeight(node);
    fixHeight(left);
    return left;
}

ERDict::Node* ERDict::rotateLeft(Node* node) {
    Node* right = node->right_;
    node->right_ = right->left_;
    right->left_ = node;
    fixHeight(node);
    fixHeight(right);
    return right;
}

ERDict::Node* ERDict::balance(Node* node) {
    fixHeight(node);
    if (getBalance(node) == 2) {
        if (getBalance(node->right_) < 0)
            node->right_ = rotateRight(node->right_);
        return rotateLeft(node);
    }
    if (getBalance(node) == -2) {
        if (getBalance(node->left_) > 0)
            node->left_ = rotateLeft(node->left_);
        return rotateRight(node);
    }
    return node;
}

ERDict::Node* ERDict::insert(Node* node, const std::string& word, const std::string & translation) {
    if (node == nullptr) {
        return createNode(word, translation);
    }
    if (word < node->data_.word_){
        node->left_ = insert(node->left_, word, translation);
    }
    else if (word > node->data_.word_){
        node->right_ = insert(node->right_, word, translation);
    }
    else{
        node->data_.translations_.push_back(translation);
    }
    return balance(node);
}

ERDict::Node* ERDict::findMin(Node* node){
    return node->left_ ? findMin(node->left_) : node;
}

ERDict::Node* ERDict::removeMin(Node* node) {
    if (node->left_ == nullptr) {
        return node->right_;
    }
    node->left_ = removeMin(node->left_);
    return balance(node);
}

ERDict::Node* ERDict::remove(Node* node, const std::string& word){
    if (node == nullptr) {
        return nullptr;
    }
    if (word < node->data_.word_) {
        node->left_ = remove(node->left_, word);
    }
    else if (word > node->data_.word_) {
        node->right_ = remove(node->right_, word);
    }
    else {
        if (node->data_.translations_.size() > 1) {
            std::cout << "Enter translation to remove: ";
            std::string translation;
            std::cin >> translation;
            StringVector& translations = node->data_.translations_;
            auto it = myFind(translations.begin(), translations.end(), translation);
            if (it != translations.end())
                erase(it, translations);
            else
                std::cout << "Translation not found." << std::endl;
            return node;
        }
        Node* left = node->left_;
        Node* right = node->right_;
        delete node;
        if (right == nullptr) {
            return left;
        }
        Node* min = findMin(right);
        min->right_ = removeMin(right);
        min->left_ = left;
        return balance(min);
    }
    return balance(node);
}

void ERDict::remove(const std::string& word) {
    root_ = remove(root_, word);
}

StringVector ERDict::search(const std::string& word) {
    StringVector results;
    ERDict::Node* node = root_;
    while (node != nullptr) {
        if (word < node->data_.word_)
        {
            node = node->left_;
        }
        else if (word > node->data_.word_) {
            node = node->right_;
        }
        else {
            results = node->data_.translations_;
            break;
        }
    }
    return results;
}

void ERDict::removeTranslation(const std::string& word, const std::string& translation) {
    Node* node = root_;
    while (node) {
        if (word < node->data_.word_){
            node = node->left_;
        }
        else if (word > node->data_.word_)
        {
           node = node->right_;
        }
        else {
            StringVector& translations = node->data_.translations_;
            StringVector::VectorIterator it = myFind(translations.begin(), translations.end(), translation);
            if (it != translations.end()) {
                erase(it, translations);
                break;
            }
            else {
                std::cout << "Translation is not found." << '\n';
                break;
            }
        }
    }
}


void ERDict::print(std::ostream& out) const {
    if (root_ == nullptr) {
        out << "Empty dictionary" << '\n';
        return;
    }
    out << "_____DICTIONARY:_____\n";
    Node* temp = root_;
    printNode(temp);
    out << "_________________________"<<"\n";
}

void ERDict::printNode(const Node* node) const {
    if (node == nullptr){
        return;
    }
    printNode(node->left_);
    std::cout << node->data_.word_ << ": ";
    std::cout << node->data_.translations_;
    std::cout << '\n';
    printNode(node->right_);
}
#endif