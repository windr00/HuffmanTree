#include "FileOperation.h"

int setOriginalFilePath(const char * Original) {
	FILE * TestFP;
	if ((TestFP = fopen(Original, "r+b")) == NULL) {
		printErr("Error: FILE ", Original, " NOT ACCESSIBLE.\n", "<EOF>");
		return 1;
	}
	OriginalFilePath = (char *)malloc(strlen(Original) + 1);
	strcpy(OriginalFilePath, Original);
	return 0;
}

int setCompressedFilePath(const char * Compressed) {
	FILE * TestFP;
	if ((TestFP = fopen(Compressed, "r+b")) == NULL) {
		printErr("Error: FILE ", Compressed, " NOT ACCESSIBLE.\n", "<EOF>");
		return 1;
	}
	CompressedFilePath = (char *)malloc(strlen(Compressed) + 1);
	strcpy(CompressedFilePath, Compressed);
	return 0;
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

}




void readFromOriginalFile() {
	FILE * OriginalFile = fopen(getOriginalFilePath(), "r+b");
	unsigned * WeightedArray = getWeightArray();
	unsigned char Character;
	while (fread(&Character, 1, 1, OriginalFile) != 0) {
		WeightedArray[Character]++;
	}
	fclose(OriginalFile);
	return 0;
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
	return 0;
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