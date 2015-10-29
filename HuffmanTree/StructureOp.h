#ifndef _STRUCTUREOP_H_
#define _STRUCTUREOP_H_
#include <stdio.h>
#define WEIGHT_ARRAY_MAX_SIZE 256

typedef struct SLHuffmanList {
	unsigned char Character;
	unsigned NodeWeight;
	struct SLHuffmanList * Next;
	struct SLHuffmanList * LeftNode;
	struct SLHuffmanList * RightNode;
}SLHuffmanList;

typedef struct SLEncodeMap {
	unsigned char * BitBuffer;
}SLEncodeMap;


SLHuffmanList * getHuffmanListHead();
SLEncodeMap * getEncodeMap();
void initEncodeMap();
unsigned * getWeightArray();
void initHuffmanList();

#endif