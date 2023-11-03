#include "Huffman.h"

int main(int argc, char **argv){

    HuffmanTree tree;
    tree.compress("../Example/githubpic.png");
    tree.decompress("../Compressed/githubpic.png");

    return 0;
}