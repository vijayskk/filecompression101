#include "../include/huffman.h"

int main(int argc, char const *argv[])
{
    compressToFile("text.txt");
    decompressFromFile("text.vzip");
    return 0;
}
