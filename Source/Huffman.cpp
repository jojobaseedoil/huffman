#include "Huffman.h"

/* CONSTRUCTORS AND DESTRUCTOR */
HuffmanTree::HuffmanTree(int CharFrequency[ASCII_SIZE]):
    mEncodeTable(ASCII_SIZE)
{
    std::priority_queue<Pair<Node*, int>> pq;
    std::string encoding = "";

    /* Creates a tree for every symbol */
    for(int i=0; i<ASCII_SIZE; i++){
        if(CharFrequency[i] == 0)
            continue;
        Node *tree = new Node(CharFrequency[i], char()+i);
        pq.push( Pair<Node*, int>(tree, tree->frequency) );
    }

    /* Create huffman tree and enconding table */
    this->CreateHuffmanTree(pq);
    this->GetHuffmanCode(mRoot, encoding);
}

HuffmanTree::HuffmanTree(const HuffmanTree&other){
    mRoot = nullptr;
    *this = other;
}

HuffmanTree::HuffmanTree():
    mRoot(nullptr)
{

}

HuffmanTree &HuffmanTree::operator=(const HuffmanTree&other){
    /* check for self assign */
    if(this == &other)
        return *this;
    Destroy(mRoot);
    /* Create new huffman tree and new encoding table */
    mRoot = other.mRoot;
    mEncodeTable = other.mEncodeTable;
    std::string encoding="";

    return *this;
}

HuffmanTree::~HuffmanTree(){
    Destroy(mRoot);
}

/* PUBLIC METHODS */
void HuffmanTree::compress(const std::string&filename){

    std::string encoded = "";
    std::vector<bool> compressed;
    std::vector<unsigned char> istream;
    bool bit;
    int freqs[256] = {0};

    /* READ INPUT FILE */
    std::ifstream fin(filename);
    if(!fin.is_open()){
        std::cerr << "Could not open input file " << filename << ".\n";
        fin.close();
        exit(EXIT_FAILURE); 
    }

    std::string basename = ExtractBaseName(filename);
    std::string extension = ExtractExtension(basename);
    std::string OutputFilename = "../Compressed/" + basename + extension;
    
    std::ofstream fout(OutputFilename);
    if(!fout.is_open()){
        std::cerr << "Could not create output file " << OutputFilename << ".\n";
        fin.close();
        fout.close();
        exit(EXIT_FAILURE);
    }

    char ch;
    while(fin.get(ch)){
        unsigned char uch = ch;
        freqs[(int)uch]++;
        istream.push_back(uch);
    }

    /* SETUP NEW HUFFMAN TREE */
    HuffmanTree *NewHuffmanTree = new HuffmanTree(freqs);
    *this = *NewHuffmanTree;


    for(unsigned char i : istream){
        encoded += mEncodeTable[(int)i];
    }

    for(char chr : encoded){
        bit = (chr=='1' ? 1 : 0);
        compressed.push_back(bit);
    }

    /* WRITING OUTPUT FILE */
    int csize = compressed.size();
    fout.write(reinterpret_cast<char*>(freqs), 256*sizeof(int));
    fout << "\n" << csize << "\n";

    for(int i=0; i<csize; ){
        unsigned char uch='\0';
        for(int j=0; j<csize && j<8; j++){
            if(compressed[i])
                uch |= 1<<j;
            i++;
        }
        fout << uch;
    }

    fin.close();
    fout.close();
    return;
}

void HuffmanTree::decompress(const std::string&filename){

    std::vector<unsigned char> decompressed;
    std::vector<bool> compressed;
    unsigned char uch;
    std::string line;
    int EncodedSize;
    int BitCount=0;
    int freqs[256] = {0};

    /* READ INPUT FILE */
    std::ifstream fin(filename);
    if(!fin.is_open()){
        std::cerr << "Could not open input file " << filename << ".\n";
        fin.close();
        exit(EXIT_FAILURE); 
    }

    std::string basename = ExtractBaseName(filename);
    std::string extension = ExtractExtension(basename);
    std::string OutputFilename = "../Decompressed/" + basename + extension;

    std::ofstream fout(OutputFilename);
    if(!fout.is_open()){
        std::cerr << "Could not create output file " << OutputFilename << ".\n";
        fin.close();
        fout.close();
        exit(EXIT_FAILURE);
    }
    
    /* GET COMPRESSED FILE STREAM OF BITS */
    fin.read(reinterpret_cast<char*>(freqs), 256*sizeof(int));    
    getline(fin, line);
    getline(fin, line);
    EncodedSize = atof(line.c_str());

    while(true){
        if(fin.eof()){
            break;
        }
        uch = fin.get();
        for(int i=0; i<8; i++){
            if(BitCount >= EncodedSize)
                break;
            bool bit;
            if( (uch & (1<<i)) != 0 )
                bit = true;
            else 
                bit = false;
            compressed.push_back(bit);
            BitCount++;
        }
    }

    HuffmanTree *NewHuffmanTree = new HuffmanTree(freqs);
    *this = *NewHuffmanTree; 

    /* DECOMPRESSING THE FILE... */
    if(compressed.empty()){
        return;
    }
    
    Node *root = mRoot;

    /* do a search in huffman tree 
    for every bit in the compressed file */
    for(bool bit : compressed){

        /* if isnt a leaf, walk in the tree */
        if(!this->IsLeaf(root)){
            root = bit==1 ? root->right : root->left; 
        }

        /* check again, if its leaf get the char
        and go back to the tree root */
        if(this->IsLeaf(root)){
            decompressed.push_back(root->leaf);
            root = mRoot;
        }
    }

    /* COPY DECOMPRESSED FILE INTO OUTPUT FILE */
    for(unsigned char chr : decompressed){
        fout << chr;
    }

    fin.close();
    fout.close();
}

/* PRIVATE METHODS */
void HuffmanTree::CreateHuffmanTree(std::priority_queue< Pair<Node*, int> >&pq){
    /* If the tree is empty, do nothing */
    if(pq.size() == 0){
        mRoot = nullptr;
        return;
    }

    /* Else create huffman tree */
    Node *SubTreeA;
    Node *SubTreeB;
    while(pq.size() > 1){

        SubTreeA = pq.top().first; pq.pop();
        SubTreeB = pq.top().first; pq.pop();
        
        int NewTreeFrequency = SubTreeA->frequency + SubTreeB->frequency;
        Node *NewTree = new Node(NewTreeFrequency);
        NewTree->left = SubTreeA;
        NewTree->right = SubTreeB;

        pq.push( Pair<Node*, int>(NewTree, NewTree->frequency) );
    }

    mRoot = pq.top().first;
}  

void HuffmanTree::GetHuffmanCode(Node *root, std::string encoding){
    if(root == nullptr)
        return;
    if(root->left)
        this->GetHuffmanCode(root->left, encoding+'0');
    if(root->right)
        this->GetHuffmanCode(root->right, encoding+'1');
    if( this->IsLeaf(root) )
        mEncodeTable[ (int)root->leaf ] = ( encoding.empty() ? std::string("0") : encoding );
}

bool HuffmanTree::IsLeaf(Node *node) const{
    return (node->right==nullptr && node->left==nullptr);
}

void HuffmanTree::Destroy(Node *root){
    /* return if its empty tree(base case) */
    if(root == nullptr)
        return;
    /* Destroy huffman tree recursively */
    Destroy(root->left);
    Destroy(root->right);
    delete root;
}
