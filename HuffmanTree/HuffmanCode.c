#include "HuffmanCode.h"

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

void finalExecution(int Operation) {
	if (Operation == 0) {
		initEncodeMap();
		SLEncodeMap * EncodeMap = getEncodeMap();
		encodeHuffmanTree(EncodeMap, getHuffmanListHead()->Next, 0);
	}
	else {
		decodeHuffmanTreeAndWriteToFile();
	}
}