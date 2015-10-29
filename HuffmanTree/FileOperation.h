#include "HuffmanCode.h"

static char * OriginalFilePath;
static char * CompressedFilePath;

void setOriginalFilePath(const char * Original);
void setCompressedFilePath(const char * Compressed);
const char * getOriginalFilePath();
const char * getCompressedFilePath();
void skipCompressedFileHead(FILE * Readable);
void readFromOriginalFile();
void readFromCompressedFile();
void writeToFile();