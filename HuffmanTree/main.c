#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define WEIGHT_ARRAY_MAX_SIZE 256

typedef struct SLHuffmanList {
	unsigned char Character;
	unsigned NodeWeight;
	struct SLHuffmanList * Next;
	struct SLHuffmanList * LeftNode;
	struct SLHuffmanList * RightNode;
}SLHuffmanList;

SLHuffmanList * getHuffmanListHead() {
	static SLHuffmanList Head;
	return &Head;
}

typedef struct SLEncodeMap {
	unsigned char * BitBuffer;
}SLEncodeMap;

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

void buildHuffmanTree() {
	SLHuffmanList * head = getHuffmanListHead();
	SLHuffmanList * NewNode;
	SLHuffmanList * tmp;
	while (head->Next->Next != NULL) {
		tmp = head->Next;
		if (tmp->Next != NULL) {
			head->Next = tmp->Next->Next;
			NewNode = (SLHuffmanList *)malloc(sizeof(SLHuffmanList));
			NewNode->Character = '\0';
			NewNode->NodeWeight = tmp->NodeWeight + tmp->Next->NodeWeight;
			NewNode->RightNode = tmp->Next;
			NewNode->RightNode->Next = NULL;
			NewNode->LeftNode = tmp;
			NewNode->LeftNode->Next = NULL;
			for (tmp = head; tmp->Next != NULL; tmp = tmp->Next) {
				if (tmp->Next->NodeWeight >= NewNode->NodeWeight) {
					break;
				}
			}
			if (tmp->Next == NULL) {
				tmp->Next = NewNode;
				NewNode->Next = NULL;
			}
			else {

				NewNode->Next = tmp->Next->Next;
				tmp->Next->Next = NewNode;
			}
		}
	}
}

void encodeHuffmanTree(SLEncodeMap * EncodeMap, SLHuffmanList * TreeRoot, unsigned CurrentBufferLength) {
	static unsigned char tmpString[WEIGHT_ARRAY_MAX_SIZE];
	if (!(TreeRoot->LeftNode && TreeRoot->RightNode))
	{
		unsigned char * tmpOperation;
		tmpOperation = (unsigned char *)malloc(CurrentBufferLength + 1);
		memcpy(tmpOperation, tmpString, CurrentBufferLength);
		tmpOperation[CurrentBufferLength] = '\0';
		EncodeMap[(int)TreeRoot->Character].BitBuffer = tmpOperation;
		return;
	}

	tmpString[CurrentBufferLength] = '0';
	tmpString[CurrentBufferLength + 1] = 0;
	encodeHuffmanTree(EncodeMap, TreeRoot->LeftNode, CurrentBufferLength + 1);

	tmpString[CurrentBufferLength] = '1';
	tmpString[CurrentBufferLength + 1] = 0;
	encodeHuffmanTree(EncodeMap, TreeRoot->RightNode, CurrentBufferLength + 1);

}



//void decodeHuffmanTree(FILE * RFP, FILE * WFP, SLHuffmanList * TreeRoot, int CurrentBit) {
//	static char OneBit = '0';
//	static char BitBuffer;
//	static int BitIndex = 7;
//	if (CurrentBit == 0) {
//		fread(&BitBuffer, 1, 1, RFP);
//		CurrentBit = 7;
//	}
//	if (!(TreeRoot->LeftNode && TreeRoot->RightNode)) {
//		fwrite(&TreeRoot->Character, 1, 1, WFP);
//		TreeRoot = getHuffmanListHead() -> Next;
//	}
//	OneBit = (BitBuffer >> CurrentBit & 0x01) + 48;
//	BitIndex--;
//	if (OneBit == '0') {
//		decodeHuffmanTree(RFP, WFP, TreeRoot->LeftNode, CurrentBit - 1);
//	}
//	else
//	{
//		decodeHuffmanTree(RFP, WFP, TreeRoot->RightNode, CurrentBit - 1);
//	}
//}
//


static char * OriginalFilePath;
static char * CompressedFilePath;

void setOriginalFilePath(const char * Original) {
	OriginalFilePath = Original;
}

void setCompressedFilePath(const char * Compressed) {
	CompressedFilePath = Compressed;
}

const char * getOriginalFilePath() {
	return OriginalFilePath;
}

const char * getCompressedFilePath() {
	return CompressedFilePath;
}

void skipCompressedFileHead(FILE * Readable) {
	int32_t FileLength = 0;
	int16_t SkipCount = 0;
	unsigned char Char = 0;
	unsigned Number = 0;

	fread(&FileLength, sizeof(int32_t), 1, Readable);
	fread(&SkipCount, sizeof(int16_t), 1, Readable);
	fseek(Readable, SkipCount * (sizeof(unsigned char)+sizeof(unsigned)), SEEK_CUR);
	/*for (int i = 0; i < SkipCount; i++) {
	fread(&Char, 1, 1, Readable);
	fread(&Number, sizeof(unsigned), 1, Readable);
	}*/

}

void decodeHuffmanTreeAndWriteToFile() {
	FILE * Readable = fopen(getCompressedFilePath(), "r+b");
	FILE * Writable = fopen(getOriginalFilePath(), "w+b");
	SLHuffmanList * TreeRoot = getHuffmanListHead()->Next;
	unsigned char BitBuffer = 0;
	unsigned char CurrentBit = 0;
	int BitIndex = 7;
	unsigned NodeWeight = 0;
	int16_t Count = 0;
	int32_t FileLength;
	int length = 0;
	int ReadFileLength = 0;
	fread(&FileLength, sizeof(int32_t), 1, Readable);
	fseek(Readable, 0, SEEK_END);
	ReadFileLength = ftell(Readable);
	fseek(Readable, 0, SEEK_SET);
	skipCompressedFileHead(Readable);
	fread(&BitBuffer, 1, 1, Readable);
	while (FileLength != length) {
		if (!(TreeRoot->LeftNode && TreeRoot->RightNode)) {
			fwrite(&TreeRoot->Character, 1, 1, Writable);
			TreeRoot = getHuffmanListHead()->Next;
		}
		CurrentBit = BitBuffer >> BitIndex & 1;
		BitIndex--;
		if (BitIndex == -1) {
			if (ReadFileLength != ftell(Readable)) {
				fread(&BitBuffer, 1, 1, Readable);
			}
			else {
				break;
			}
			BitIndex = 7;
		}
		if (CurrentBit == 1) {
			TreeRoot = TreeRoot->RightNode;
		}
		else {
			TreeRoot = TreeRoot->LeftNode;
		}
		length = ftell(Writable);
	}
	fclose(Readable);
	fclose(Writable);
}


void readFromOriginalFile() {
	FILE * OriginalFile = fopen(getOriginalFilePath(), "r+b");
	unsigned * WeightedArray = getWeightArray();
	unsigned char Character;
	while (fread(&Character, 1, 1, OriginalFile) != 0) {
		WeightedArray[Character]++;
	}
	fclose(OriginalFile);
}

void readFromCompressedFile() {
	FILE * CompressedFile = fopen(getCompressedFilePath(), "r+b");
	unsigned * WeightedArray = getWeightArray();
	int32_t FileLength = 0;
	int16_t Count = 0;
	unsigned char Character = 0;
	unsigned NodeWeight = 0;
	fread(&FileLength, sizeof(int32_t), 1, CompressedFile);
	fread(&Count, sizeof(int16_t), 1, CompressedFile);
	for (int i = 0; i < Count; i++) {
		fread(&Character, 1, 1, CompressedFile);
		fread(&NodeWeight, sizeof(unsigned), 1, CompressedFile);
		WeightedArray[Character] = NodeWeight;
		printf("%c (%d)\t%u\n", Character, Character, NodeWeight);
	}
	fclose(CompressedFile);
}


void finalExecution(int Operation) {
	if (Operation == 0) {
		initEncodeMap();
		SLEncodeMap * EncodeMap = getEncodeMap();
		encodeHuffmanTree(EncodeMap, getHuffmanListHead()->Next, 0);
		for (int i = 0; i < WEIGHT_ARRAY_MAX_SIZE; i++) {
			if (EncodeMap[i].BitBuffer != NULL) {
				printf("%c (%d)\t%s\n", (unsigned char)i, i, EncodeMap[i].BitBuffer);
			}
		}
	}
	else {
		decodeHuffmanTreeAndWriteToFile();
	}
}



void writeToFile() {
	FILE * Writable = fopen(getCompressedFilePath(), "w+b");
	FILE * Readable = fopen(getOriginalFilePath(), "r+b");
	unsigned * WeightTable = getWeightArray();
	SLEncodeMap * EncodeMap = getEncodeMap();
	int16_t Count = 0;
	int32_t FileEndIndex = 0;
	fseek(Readable, 0, SEEK_END);
	FileEndIndex = ftell(Readable);
	fseek(Readable, 0, SEEK_SET);
	fwrite(&FileEndIndex, sizeof(int32_t), 1, Writable);
	for (int i = 0; i < WEIGHT_ARRAY_MAX_SIZE; i++) {
		if (WeightTable[i] != 0) {
			Count++;
		}
	}
	fwrite(&Count, sizeof(int16_t), 1, Writable);
	for (int i = 0; i < WEIGHT_ARRAY_MAX_SIZE; i++) {
		if (WeightTable[i] != 0) {
			fwrite(&i, 1, 1, Writable);
			fwrite(WeightTable + i, sizeof(unsigned), 1, Writable);
		}
	}
	unsigned char BitCode = 0;
	unsigned char CharCode = 0;
	char * BitCodeBuffer = NULL;
	int BitIndex = 0;
	int BitCount = 0;
	fread(&CharCode, 1, 1, Readable);
	BitCodeBuffer = EncodeMap[(unsigned)CharCode].BitBuffer;

	while (1) {
		if (BitCount == 8) {
			fwrite(&BitCode, 1, 1, Writable);
			BitCount = 0;
			continue;
		}

		if (BitIndex == strlen(BitCodeBuffer)) {
			if (FileEndIndex != ftell(Readable)) {
				fread(&CharCode, 1, 1, Readable);
			}
			else {
				break;
			}
			BitCodeBuffer = EncodeMap[(unsigned)CharCode].BitBuffer;
			BitIndex = 0;
		}

		if (BitCodeBuffer[BitIndex] == '0') {
			BitCode <<= 1;
			BitIndex++;
			BitCount++;
		}
		else {
			BitCode = (BitCode << 1) | 1;
			BitIndex++;
			BitCount++;
		}
	}

	if (BitCount != 8) {
		BitCode <<= 8 - BitCount;
		fwrite(&BitCode, 1, 1, Writable);
	}
	fclose(Writable);
	fclose(Readable);
}

void encodeFile() {
	setOriginalFilePath("D:\\img13.jpg");
	setCompressedFilePath("D:\\test.hiz");
	readFromOriginalFile();
	initHuffmanList();
	buildHuffmanTree();
	finalExecution(0);
	writeToFile();
}

void decodeFile() {
	setCompressedFilePath("D:\\test.hiz");
	setOriginalFilePath("D:\\temp.jpg");
	readFromCompressedFile();
	initHuffmanList();
	buildHuffmanTree();
	finalExecution(1);
}

int main(int argc, char ** argv) {
	encodeFile();
	system("pause");
	decodeFile();
	system("pause");
	return 0;
}
