#include <iostream>
#include <string>
#include "../include/huffman.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage:\n";
        cerr << "  " << argv[0] << " compress <inputfile>\n";
        cerr << "  " << argv[0] << " decompress <inputfile.vzip>\n";
        return 1;
    }

    string command = argv[1];
    string filename = argv[2];

    if (command == "compress") {
        compressToFile(filename);
    } else if (command == "decompress") {
        decompressFromFile(filename);
    } else {
        cerr << "Unknown command: " << command << "\n";
        return 1;
    }

    return 0;
}
