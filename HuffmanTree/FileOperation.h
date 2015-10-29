#ifndef _FILEOPERATION_H_
#define _FILEOPERATION_H_
#include "includes.h"

static char * OriginalFilePath;
static char * CompressedFilePath;
int setOriginalFilePath(const char * Original);
int setCompressedFilePath(const char * Compressed);
int ifOnlyOneCharacter(unsigned char * TheOnlyChar);
int ifCompressedOnlyOneCharacter(unsigned char * TheOnlyChar);
const char * getOriginalFilePath();
const char * getCompressedFilePath();
void skipCompressedFileHead(FILE * Readable);
void readFromOriginalFile();
void readFromCompressedFile();
void writeToFile();
static int checkFile(const char * FilePath);
#endif