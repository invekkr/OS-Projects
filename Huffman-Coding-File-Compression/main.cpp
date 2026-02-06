#include <iostream>
#include "Huffman.hpp"
#include "Huffman.cpp" // Include implementation for single-step compilation if needed

using namespace std;

int main() {
    Huffman h;
    int choice;
    string inputFile, outputFile;

    cout << "=== Huffman File Compressor ===\n";
    cout << "1. Compress File\n";
    cout << "2. Decompress File\n";
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1) {
        cout << "Enter input filename (e.g., data.txt): ";
        cin >> inputFile;
        cout << "Enter output filename (e.g., compressed.huff): ";
        cin >> outputFile;
        h.compress(inputFile, outputFile);
    } else if (choice == 2) {
        cout << "Enter compressed filename (e.g., compressed.huff): ";
        cin >> inputFile;
        cout << "Enter output filename (e.g., original.txt): ";
        cin >> outputFile;
        h.decompress(inputFile, outputFile);
    } else {
        cout << "Invalid choice.\n";
    }

    return 0;
}