#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <fstream>

using namespace std;

// Tree Node Structure
struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f, Node* l = nullptr, Node* r = nullptr) 
        : ch(c), freq(f), left(l), right(r) {}
};

// Comparator for Priority Queue (Now Deterministic)
struct Compare {
    bool operator()(Node* l, Node* r) {
        // Higher frequency comes later (Min-Heap behavior)
        if (l->freq != r->freq) {
            return l->freq > r->freq;
        }
        // If frequencies are equal, use character value to break tie
        // This ensures the tree is always built the exact same way
        return l->ch > r->ch;
    }
};

class Huffman {
private:
    Node* root;
    map<char, string> huffmanCodes; // Changed to map for order
    map<char, int> freqMap;         // Changed to map for order

    void generateCodes(Node* node, string str);
    void deleteTree(Node* node);
    void buildTree();
    void writeHeader(ofstream &outFile);
    void readHeader(ifstream &inFile);

public:
    Huffman();
    ~Huffman();
    void compress(string inputFile, string outputFile);
    void decompress(string inputFile, string outputFile);
};

#endif