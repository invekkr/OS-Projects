#include "Huffman.hpp"
#include <iostream>

Huffman::Huffman() : root(nullptr) {}

Huffman::~Huffman() {
    deleteTree(root);
}

void Huffman::deleteTree(Node* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void Huffman::generateCodes(Node* node, string str) {
    if (!node) return;
    if (!node->left && !node->right) {
        huffmanCodes[node->ch] = str;
    }
    generateCodes(node->left, str + "0");
    generateCodes(node->right, str + "1");
}

void Huffman::buildTree() {
    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (auto pair : freqMap) {
        pq.push(new Node(pair.first, pair.second));
    }
    
    // Edge case: If file has only 1 unique character (e.g., "aaaa")
    // We must manually create a parent so it gets a code bit (e.g. "0")
    if (pq.size() == 1) {
        Node* left = pq.top(); pq.pop();
        Node* dummy = new Node('\0', left->freq, left, nullptr);
        pq.push(dummy);
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        
        int sum = left->freq + right->freq;
        // Internal node has '\0' char
        pq.push(new Node('\0', sum, left, right));
    }
    root = pq.top();
}

void Huffman::compress(string inputFile, string outputFile) {
    ifstream inFile(inputFile, ios::binary);
    if (!inFile.is_open()) {
        cerr << "Error: Could not open input file." << endl;
        return;
    }

    freqMap.clear();
    huffmanCodes.clear();

    // 1. Count Frequencies
    char ch;
    while (inFile.get(ch)) {
        freqMap[ch]++;
    }
    
    // Handle empty file
    if (freqMap.empty()) {
        inFile.close();
        ofstream outFile(outputFile, ios::binary);
        outFile.close(); 
        cout << "File is empty. Created empty output." << endl;
        return;
    }

    inFile.clear(); 
    inFile.seekg(0); 

    // 2. Build Tree & Codes
    buildTree();
    generateCodes(root, "");

    ofstream outFile(outputFile, ios::binary);
    
    // 3. Write Header
    writeHeader(outFile);

    // 4. Write Compressed Data
    unsigned char buffer = 0;
    int bitCount = 0;

    while (inFile.get(ch)) {
        string code = huffmanCodes[ch];
        for (char bit : code) {
            buffer = buffer << 1;
            if (bit == '1') {
                buffer = buffer | 1;
            }
            bitCount++;
            if (bitCount == 8) {
                outFile.put(buffer);
                buffer = 0;
                bitCount = 0;
            }
        }
    }

    // Padding
    if (bitCount > 0) {
        buffer = buffer << (8 - bitCount);
        outFile.put(buffer);
    }

    cout << "Compression successful! Saved to " << outputFile << endl;
    inFile.close();
    outFile.close();
}

void Huffman::writeHeader(ofstream &outFile) {
    size_t size = freqMap.size();
    outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

    for (auto pair : freqMap) {
        outFile.write(&pair.first, sizeof(char));
        outFile.write(reinterpret_cast<const char*>(&pair.second), sizeof(int));
    }
}

void Huffman::readHeader(ifstream &inFile) {
    size_t size;
    inFile.read(reinterpret_cast<char*>(&size), sizeof(size));

    freqMap.clear();
    for (size_t i = 0; i < size; ++i) {
        char ch;
        int freq;
        inFile.read(&ch, sizeof(char));
        inFile.read(reinterpret_cast<char*>(&freq), sizeof(int));
        freqMap[ch] = freq;
    }
}

void Huffman::decompress(string inputFile, string outputFile) {
    ifstream inFile(inputFile, ios::binary);
    if (!inFile.is_open()) {
        cerr << "Error: Could not open file." << endl;
        return;
    }

    // 1. Read Header
    readHeader(inFile);
    
    // Handle empty map (empty file case)
    if (freqMap.empty()) {
        inFile.close();
        ofstream outFile(outputFile, ios::binary);
        outFile.close();
        cout << "Decompressed empty file." << endl;
        return;
    }

    // 2. Rebuild Tree
    buildTree();

    ofstream outFile(outputFile, ios::binary);
    Node* curr = root;
    unsigned char byte; // Using unsigned char to prevent sign extension issues
    
    long long totalChars = root->freq; 
    long long count = 0;

    // 3. Read bits
    // Note: '>> 4' on a signed char can behave differently than unsigned
    // so we strictly use unsigned char for 'byte'
    while (inFile.read(reinterpret_cast<char*>(&byte), 1)) {
        for (int i = 7; i >= 0; i--) {
            if (count >= totalChars) break; 

            if ((byte >> i) & 1) {
                curr = curr->right;
            } else {
                curr = curr->left;
            }

            if (!curr->left && !curr->right) { 
                outFile.put(curr->ch);
                curr = root;
                count++;
            }
        }
    }

    cout << "Decompression successful! Saved to " << outputFile << endl;
    inFile.close();
    outFile.close();
}