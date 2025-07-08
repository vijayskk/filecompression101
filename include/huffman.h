#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class Node{  // This is the Node class
    public:
    int freq;
    char ch;
    Node * left, *right;
    Node(char c,int f):ch(c),freq(f),left(nullptr),right(nullptr){}
};

struct Compare{
    bool operator()(Node * node1, Node * node2){
        return node1->freq > node2->freq;
    }
};

void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

string processFileName(string filename){
    return filename.substr(0,filename.find('.'));
}

void writeBitsToFile(const string& bitString, const unordered_map<char, int>& freq, const string& filename) {
    ofstream outFile(filename, ios::binary);
    if (!outFile) {
        cerr << "Error opening output file!\n";
        return;
    }

    int tableSize = freq.size();
    outFile.write(reinterpret_cast<const char*>(&tableSize), sizeof(int));

    for (const auto& pair : freq) {
        outFile.write(&pair.first, sizeof(char));
        outFile.write(reinterpret_cast<const char*>(&pair.second), sizeof(int));
    }

    int totalBits = bitString.length();
    outFile.write(reinterpret_cast<const char*>(&totalBits), sizeof(int));

    unsigned char buffer = 0;
    int bitCount = 0;
    for (char bit : bitString) {
        buffer = (buffer << 1) | (bit - '0');
        bitCount++;
        if (bitCount == 8) {
            outFile.put(buffer);
            buffer = 0;
            bitCount = 0;
        }
    }
    if (bitCount > 0) {
        buffer <<= (8 - bitCount);
        outFile.put(buffer);
    }

    outFile.close();
}


void buildCodeMap(Node * root,string code,unordered_map<char,string> & huffmanCode){
    if (!root)
    {
        return;
    }
    if (!root->left && !root->right)
    {
        huffmanCode[root->ch] = code;
    }
    buildCodeMap(root->left,code + "0",huffmanCode);
    buildCodeMap(root->right,code + "1",huffmanCode);
}

int compressToFile(string inputfile){

    ifstream inFile(inputfile,ios::binary | ios::in);
    if (!inFile)
    {
        return -1;
    }
    
    stringstream strStream;
    strStream << inFile.rdbuf();
    string text = strStream.str();

    unordered_map<char,int> freq;
    for(char ch : text){
        freq[ch]++;
    }
    
    priority_queue<Node* ,vector<Node*>,Compare> pq;
    for(auto pair : freq){
        pq.push(new Node(pair.first,pair.second));
    }

    while (pq.size() > 1)
    {
        Node * left = pq.top();
        pq.pop();
        Node * right = pq.top();
        pq.pop();

        Node * merged = new Node('\0' ,left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        pq.push(merged);
    }


    unordered_map<char,string> huffmanCode;
    buildCodeMap(pq.top(),"",huffmanCode);
    
    string encoded;
    for(char ch : text){
        encoded += huffmanCode[ch];
    }
    string outputfile = processFileName(inputfile) + ".vzip";
    writeBitsToFile(encoded,freq,outputfile);
    encoded.clear();
    cout << "Compressed to " << outputfile << "\n";
    freeTree(pq.top());
    return 0;
}

int decompressFromFile(const string& filename) {
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "Error opening compressed file!\n";
        return 1;
    }

    // Step 1: Read frequency table
    int tableSize;
    inFile.read(reinterpret_cast<char*>(&tableSize), sizeof(int));
    unordered_map<char, int> freq;
    for (int i = 0; i < tableSize; ++i) {
        char ch;
        int f;
        inFile.read(&ch, sizeof(char));
        inFile.read(reinterpret_cast<char*>(&f), sizeof(int));
        freq[ch] = f;
    }

    // Step 2: Rebuild Huffman tree
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto& pair : freq)
        pq.push(new Node(pair.first, pair.second));

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* merged = new Node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        pq.push(merged);
    }
    Node* root = pq.top();

    // Step 3: Read number of bits
    int totalBits;
    inFile.read(reinterpret_cast<char*>(&totalBits), sizeof(int));

    // Step 4: Read bitstream
    vector<unsigned char> data((istreambuf_iterator<char>(inFile)), {});
    string bitString;
    for (unsigned char byte : data) {
        for (int i = 7; i >= 0 && bitString.length() < totalBits; --i) {
            bitString += ((byte >> i) & 1) ? '1' : '0';
        }
    }

    // Step 5: Decode
    string decoded;
    Node* current = root;
    for (char bit : bitString) {
        current = (bit == '0') ? current->left : current->right;
        if (!current->left && !current->right) {
            decoded += current->ch;
            current = root;
        }
    }
    string outputFile = processFileName(filename) + ".txt";
    ofstream out(outputFile, ios::binary);
    out << decoded;
    out.close();
    decoded.clear();
    cout << "Decompressed to " << outputFile << "\n";
    freeTree(pq.top());
    return 0;
}

