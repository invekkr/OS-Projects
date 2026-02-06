
**Overview**
This project is a file compression tool built in C++. It uses a technique called **Huffman Coding** to compress text files (making them smaller) without losing any data. When you decompress the file later, you get back the exact original text. This is the same basic logic used in tools like ZIP or GZIP.

**Key Features**

* **Lossless Compression:** It shrinks the file size, but the original data is preserved perfectly.
* **Bitwise Operations:** Instead of writing "0" or "1" as text characters (which takes up space), the program writes actual binary bits, which is what makes the file smaller.
* **Self-Contained:** It saves the "decoding key" (the frequency map) at the start of the compressed file. This means you can send the compressed file to anyone, and the program can figure out how to decompress it on its own.

**How to Run It**

1. **Compile:** Use a C++ compiler like g++ to turn the code into an executable program.
* Command: `g++ main.cpp -o huffman`


2. **Run:** Start the program.
* Command: `./huffman`


3. **Use the Menu:**
* Choose **Option 1** to compress a file (e.g., turn `data.txt` into `test.huff`).
* Choose **Option 2** to decompress a file (e.g., turn `test.huff` back into `original.txt`).



---

### Understanding the Concept: Huffman Coding

The main idea behind Huffman coding is efficiency. In a standard text file, every letter uses the same amount of space (8 bits), whether it's a common letter like 'e' or a rare one like 'z'.

Huffman coding changes this by using **variable lengths**:

1. **Frequent characters** (like 'e' or 'a') get very short binary codes (like `10` or `0`).
2. **Rare characters** (like 'z' or 'q') get longer binary codes (like `110101`).

By using short codes for the things that appear most often, the total file size drops significantly.

**The Process:**

1. **Count:** The program reads your file and counts how many times each character appears.
2. **Build a Tree:** It organizes these characters into a binary tree. The most frequent characters end up near the top (shorter path), and rare ones are at the bottom (longer path).
3. **Assign Codes:** It traces the path from the top of the tree to each character. Going left is a `0`, going right is a `1`. This creates the unique binary code for each letter.

---

### Understanding the Code

Here is how the C++ files work together:

**1. The Header File (`Huffman.hpp`)**
This file acts as the blueprint. It defines:

* **`Node` struct:** A small block of memory that holds a character, its frequency, and pointers to its left and right children. This is the building block of the tree.
* **`Compare` struct:** A rule for sorting. It ensures that when we build the tree, the characters with the lowest frequency are processed first. It also includes a "tie-breaker" rule to ensure the tree is built exactly the same way every time (deterministic), which prevents corruption.

**2. The Logic File (`Huffman.cpp`)**
This is where the actual work happens.

* **`buildTree()`:** This function uses a Priority Queue (a sorted list) to build the Huffman tree from the bottom up. It keeps combining the two smallest nodes until only one big tree remains.
* **`compress()`:**
* It reads your text file.
* It calculates the codes (e.g., 'a' = 01).
* It packs these codes into bytes. Since computers can't write just 3 bits, we have to bundle them into groups of 8 (a byte) using bit-shifting (`<<`) and bitwise OR (`|`).
* It writes the "header" (the frequency counts) first, then the compressed data.


* **`decompress()`:**
* It reads the header first to rebuild the exact same tree used during compression.
* It reads the raw binary data bit by bit, moving left or right down the tree. When it hits a leaf (a character), it writes that character to the output file and goes back to the top of the tree.



**3. The Main File (`main.cpp`)**
This is just the user interface. It asks you what you want to do (Compress or Decompress), takes the file names you type in, and calls the functions in `Huffman.cpp` to do the job.