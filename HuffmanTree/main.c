//
//  Anothermain.c
//  HuffmanTree
//
//  Created by 冉惟之 on 15/10/18.
//  Copyright © 2015年 WindR. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>


#define WEIGHTED_ARRAY_SIZE 256

typedef struct SLWeightTable {
	char * DynamicCharacterArray;
	unsigned NodeWeight;
	int CharacterCount;
}SLWeigtTable;

typedef struct SLHuffmanTree{
	SLWeigtTable CharacterAttribute;
	struct SLHuffmanTree * ParreleRoot;
	struct SLHuffmanTree * LeftNode;
	struct SLHuffmanTree * RightNode;
}SLHuffmanTree;

SLWeigtTable * getWeightedArray() {
	static SLWeigtTable WeightedArray[WEIGHTED_ARRAY_SIZE];
	system("cls");
	for (int i = 0; i < WEIGHTED_ARRAY_SIZE; i++) {
		printf("C:%s\tCC:%d\tNW:%u\n"); 
	}
	return WeightedArray;
}



void initWeightedArray() {
	SLWeigtTable * TempArray = getWeightedArray();
	for (int i = 0; i < WEIGHTED_ARRAY_SIZE; i++) {
		TempArray[i].DynamicCharacterArray = (char *)malloc(2);
		memset(TempArray[i].DynamicCharacterArray, 0, 2);
		TempArray[i].DynamicCharacterArray[0] = (char)i;
		TempArray[i].NodeWeight = 0;
		TempArray[i].CharacterCount = 1;
	}
}

int appendNewCharacter(const char NewCharacter) {
	SLWeigtTable * TempArray = getWeightedArray();
	if ((int)NewCharacter > WEIGHTED_ARRAY_SIZE) {
		return -1;
	}
	TempArray[NewCharacter].NodeWeight++;
	return 0;
}

unsigned getCharacterWeightFromArray(const char Character) {
	SLWeigtTable * TempArray = getWeightedArray();
	return TempArray[Character].NodeWeight;
}

SLWeigtTable getOneMinimumWeightedChar() {
	SLWeigtTable * TempArray = getWeightedArray();
	unsigned MinimumWeight = UINT_MAX;
	int MinimumIndex = 0;
	SLWeigtTable MinimumWeightedChar;
	MinimumWeightedChar.DynamicCharacterArray = NULL;
	int i = 0;
	for (i = 0; i < WEIGHTED_ARRAY_SIZE; i++) {
		if (TempArray[i].NodeWeight < MinimumWeight && TempArray[i].NodeWeight != UINT_MAX &&
			TempArray[i].DynamicCharacterArray != NULL && TempArray[i].NodeWeight > 0) {

			MinimumWeight = TempArray[i].NodeWeight;
			MinimumWeightedChar = TempArray[i];
			MinimumWeightedChar.DynamicCharacterArray = (char *)malloc(strlen(TempArray[i].DynamicCharacterArray) + 1);
			memset(MinimumWeightedChar.DynamicCharacterArray, 0, strlen(TempArray[i].DynamicCharacterArray) + 1);
			memcpy(MinimumWeightedChar.DynamicCharacterArray, TempArray[i].DynamicCharacterArray, strlen(TempArray[i].DynamicCharacterArray));
			MinimumIndex = i;
		}
	}
	TempArray[MinimumIndex].NodeWeight = UINT_MAX;
	return MinimumWeightedChar;
}

char * strMerge(char * CharOne, char * CharTwo) {
	char * RetStr = (char *)malloc(strlen(CharOne) + strlen(CharTwo) + 2);

	memset(RetStr, 0, strlen(CharOne) + strlen(CharTwo) + 2);

	strcpy(RetStr, CharOne);
	strcat(RetStr, ",");
	strcat(RetStr, CharTwo);

	return RetStr;
}

void mergeCharacterWeight(SLWeigtTable CharOne, SLWeigtTable CharTwo) {

	SLWeigtTable * TempArray = getWeightedArray();

	for (int i = 0; i < WEIGHTED_ARRAY_SIZE; i++) {
		if (TempArray[i].DynamicCharacterArray != NULL && strcmp(TempArray[i].DynamicCharacterArray, CharOne.DynamicCharacterArray) == 0) {
			free(TempArray[i].DynamicCharacterArray);
			TempArray[i].DynamicCharacterArray = (char *)malloc(strlen(CharOne.DynamicCharacterArray) + strlen(CharTwo.DynamicCharacterArray) + 2);

			TempArray[i].DynamicCharacterArray = strMerge(CharOne.DynamicCharacterArray, CharTwo.DynamicCharacterArray);


			TempArray[i].NodeWeight = CharOne.NodeWeight + CharTwo.NodeWeight;
			TempArray[i].CharacterCount = CharOne.CharacterCount + CharTwo.CharacterCount;
		}

		if (TempArray[i].DynamicCharacterArray != NULL && strcmp(TempArray[i].DynamicCharacterArray, CharTwo.DynamicCharacterArray) == 0) {
			free(TempArray[i].DynamicCharacterArray);
			TempArray[i].DynamicCharacterArray = NULL;
		}

	}
}

SLHuffmanTree * initTreeNode() {
	SLHuffmanTree * TreeNode;

	TreeNode = (SLHuffmanTree *)malloc(sizeof(SLHuffmanTree));

	TreeNode->CharacterAttribute.DynamicCharacterArray = NULL;

	TreeNode->ParreleRoot = NULL;

	TreeNode->CharacterAttribute.NodeWeight = 0;
	TreeNode->CharacterAttribute.CharacterCount = 0;

	TreeNode->LeftNode = NULL;
	TreeNode->RightNode = NULL;

	return TreeNode;
}


SLHuffmanTree * buildHuffmanTree() {
	SLHuffmanTree * TreeRoot = initTreeNode();
	SLWeigtTable CharOne;
	SLWeigtTable CharTwo;
	CharOne = getOneMinimumWeightedChar();
	CharTwo = getOneMinimumWeightedChar();
	while (CharOne.DynamicCharacterArray != NULL && CharTwo.DynamicCharacterArray != NULL) {

		SLHuffmanTree * NewTree;
		NewTree = initTreeNode();
		NewTree->LeftNode = initTreeNode();
		NewTree->RightNode = initTreeNode();
		NewTree->LeftNode->CharacterAttribute = CharOne;
		NewTree->RightNode->CharacterAttribute = CharTwo;

		NewTree->CharacterAttribute.DynamicCharacterArray = strMerge(CharOne.DynamicCharacterArray, CharTwo.DynamicCharacterArray);

		NewTree->CharacterAttribute.NodeWeight = CharOne.NodeWeight + CharTwo.NodeWeight;

		NewTree->CharacterAttribute.CharacterCount = CharTwo.CharacterCount + CharOne.CharacterCount;

		mergeCharacterWeight(CharOne, CharTwo);

		if (TreeRoot->CharacterAttribute.DynamicCharacterArray != NULL && (strcmp(TreeRoot->CharacterAttribute.DynamicCharacterArray, CharOne.DynamicCharacterArray) == 0)) {
			NewTree->LeftNode = TreeRoot;
			if (TreeRoot->ParreleRoot != NULL) {
				NewTree->RightNode = TreeRoot->ParreleRoot;
				TreeRoot->ParreleRoot = NULL;
			}
			TreeRoot = NewTree;
			CharOne = getOneMinimumWeightedChar();
			CharTwo = getOneMinimumWeightedChar();
			continue;
		}

		else if (TreeRoot->CharacterAttribute.DynamicCharacterArray != NULL && (strcmp(TreeRoot->CharacterAttribute.DynamicCharacterArray, CharTwo.DynamicCharacterArray) == 0)) {
			NewTree->RightNode = TreeRoot;
			TreeRoot = NewTree;
			CharOne = getOneMinimumWeightedChar();
			CharTwo = getOneMinimumWeightedChar();
			continue;
		}

		else if ((TreeRoot->LeftNode == NULL && TreeRoot->RightNode == NULL)) {

			TreeRoot = NewTree;
			CharOne = getOneMinimumWeightedChar();
			CharTwo = getOneMinimumWeightedChar();
			continue;
		}
		else {
			TreeRoot->ParreleRoot = NewTree;
			CharOne = getOneMinimumWeightedChar();
			CharTwo = getOneMinimumWeightedChar();
		}
	}
	return TreeRoot;
}

char * encodeHuffman(SLHuffmanTree * TreeRoot, const char TargetCharacter, int NodeFlag) {
	static char * BitBuffer = "";
	static unsigned BufferLength = 1;
	char * TempStore = NULL;
	char * retValue = NULL;
	if (TreeRoot != NULL) {
		if ((retValue = encodeHuffman(TreeRoot->RightNode, TargetCharacter, 1) != NULL)) {
			return retValue;
		}
		if ((retValue = encodeHuffman(TreeRoot->LeftNode, TargetCharacter, 0)) != NULL) {
			return retValue;
		}
		if (0 == NodeFlag || 1 == NodeFlag) {
			TempStore = BitBuffer;
			BitBuffer = (char *)malloc(BufferLength + 1);
			memset(BitBuffer, 0, BufferLength + 1);
			memcpy(BitBuffer, TempStore, BufferLength);
			BitBuffer[BufferLength - 1] = (char)(NodeFlag + 48);
			BufferLength++;
		}
		if (TreeRoot->LeftNode == NULL && TreeRoot->RightNode == NULL && TreeRoot->CharacterAttribute.DynamicCharacterArray[0] == TargetCharacter) {
			retValue = (char *)malloc(BufferLength);
			memset(retValue, 0, BufferLength);
			memcpy(retValue, BitBuffer, strlen(BitBuffer));
			BitBuffer = "";
			BufferLength = 1;
			return retValue;
		}
	}
	BitBuffer = "";
	BufferLength = 1;
	return NULL;
}

void readFromFile(const char * FilePath) {
	FILE * FilePointer = fopen(FilePath, "r+b");
	char TempChar;

	while ((TempChar = fgetc(FilePointer)) != EOF){
		appendNewCharacter(TempChar);
	}
}

void printResult() {
	int CharacterCount = 0;
	SLWeigtTable * WeightedTable = getWeightedArray();
	SLHuffmanTree * TreeRoot = buildHuffmanTree();
	char * AllCharacters = NULL;
	for (int i = 0; i < WEIGHTED_ARRAY_SIZE; i++) {
		if (WeightedTable[i].DynamicCharacterArray != NULL && strcmp(WeightedTable[i].DynamicCharacterArray, "\0") && WeightedTable[i].NodeWeight != 0) {
			CharacterCount = WeightedTable[i].CharacterCount;
			AllCharacters = WeightedTable[i].DynamicCharacterArray;
			break;
		}
	}

	printf("%s", encodeHuffman(TreeRoot, 'l', 2));
	printf("%s", encodeHuffman(TreeRoot, 'a', 2));

	/*for (int i = 0; i < 2 * CharacterCount; i += 2) {
	char * retValue = encodeHuffman(TreeRoot, AllCharacters[i], 2);
	printf("%c\t%s\n", AllCharacters[i], retValue);
	}*/
}


int main(int argc, const char ** argv) {
	initWeightedArray();
	readFromFile("F:\\test.txt");
	printResult();
	system("pause");
	return 0;
}