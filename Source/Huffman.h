#pragma once

#include "Utils.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

#define ASCII_SIZE 256

template<class T1, class T2>
struct Pair{
    Pair(const T1&first, const T2&second):
        first(first),
        second(second)
    {}
    Pair() = default;

    bool operator<(const Pair&other) const{
        return second >= other.second;
    }

    T1 first;
    T2 second;

};

struct Node{

    Node(const int frequency, const unsigned char leaf=char()):
        frequency(frequency), 
        right(nullptr),
        left(nullptr),
        leaf(leaf)
    {}
    Node() = default;

    Node *left;
    Node *right;
    unsigned char leaf;
    int frequency;
};

class HuffmanTree{

public:
    /* CONSTRUCTORS AND DESTRUCTOR */
    HuffmanTree(int CharFrequency[ASCII_SIZE]);
    HuffmanTree(const HuffmanTree&);
    HuffmanTree();
    HuffmanTree &operator=(const HuffmanTree&);
    ~HuffmanTree();

    /* PUBLIC METHODS */
    void compress(const std::string&filename);
    void decompress(const std::string&filename);

private:
    /* PRIVATE METHODS */
    void CreateHuffmanTree(std::priority_queue< Pair<Node*, int> >&);
    void GetHuffmanCode(Node *root, std::string encoding);
    bool IsLeaf(Node *node) const;
    void Destroy(Node *root);

    /* PRIVATE ATTRIBUTES */
    Node *mRoot;
    std::vector<std::string> mEncodeTable;
};