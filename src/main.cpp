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

void writeBitsToFile(const string& bitString, const string& filename) {
    ofstream outFile(filename, ios::binary);
    if (!outFile) {
        cerr << "Error opening output file!\n";
        return;
    }

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
        buffer <<= (8 - bitCount); // Pad remaining bits with 0s
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

void huffmanEncode(string& text){
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

    for (auto pair : huffmanCode) {
        cout << "'" << (string(1, pair.first)) << "' : " << pair.second << "\n";
    }
    
    string encoded;
    for(char ch : text){
        encoded += huffmanCode[ch];
    }
    //cout<< "Encoded: " << encoded << endl;
    cout<< "\n" << encoded.length() << " " << text.length() * 8 <<endl;
    writeBitsToFile(encoded,"text.bin");
    encoded.clear();
}

int main(int argc, char const *argv[])
{
    ifstream inFile("text.txt",ios::binary | ios::in);
    stringstream strStream;
    strStream << inFile.rdbuf();
    string text = strStream.str();
    huffmanEncode(text);
    return 0;
}
