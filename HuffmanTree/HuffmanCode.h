#include "StructureOp.h"
#include <stdint.h>

void buildHuffmanTree();
void encodeHuffmanTree(SLEncodeMap * EncodeMap, SLHuffmanList * TreeRoot, unsigned CurrentBufferLength);
void decodeHuffmanTreeAndWriteToFile();
void finalExecution(int Operation);