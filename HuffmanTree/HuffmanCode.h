#ifndef _HUFFMANCODE_H_
#define _HUFFMANCODE_H_
#include "includes.h"

void buildHuffmanTree();
void encodeHuffmanTree(SLEncodeMap * EncodeMap, SLHuffmanList * TreeRoot, unsigned CurrentBufferLength);
void decodeHuffmanTreeAndWriteToFile();
void finalExecution(int Operation);

#endif