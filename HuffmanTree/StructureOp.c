
#include "StructureOp.h"

SLHuffmanList * getHuffmanListHead() {
	static SLHuffmanList Head;
	return &Head;
}

SLEncodeMap * getEncodeMap() {
	static SLEncodeMap EncodeMap[WEIGHT_ARRAY_MAX_SIZE];
	return &EncodeMap;
}

void initEncodeMap() {
	SLEncodeMap * TempMap = getEncodeMap();
	for (int i = 0; i < WEIGHT_ARRAY_MAX_SIZE; i++) {
		TempMap[i].BitBuffer = NULL;
	}
}

unsigned * getWeightArray() {
	static unsigned WeightArray[WEIGHT_ARRAY_MAX_SIZE];
	return WeightArray;
}

void initHuffmanList() {
	SLHuffmanList * head = getHuffmanListHead();
	SLHuffmanList * NewNode;
	unsigned * WeightArray = getWeightArray();
	head->Next = NULL;
	head->LeftNode = NULL;
	head->RightNode = NULL;
	for (int i = 0; i < WEIGHT_ARRAY_MAX_SIZE; i++) {
		if (WeightArray[i] != 0) {
			for (head = head; head->Next != NULL; head = head->Next) {
				if (WeightArray[i] <= head->Next->NodeWeight) {
					break;
				}
			}
			NewNode = (SLHuffmanList *)malloc(sizeof(SLHuffmanList));
			NewNode->Character = i;
			NewNode->NodeWeight = WeightArray[i];
			NewNode->LeftNode = NULL;
			NewNode->RightNode = NULL;
			NewNode->Next = head->Next;
			head->Next = NewNode;
		}
	}

}