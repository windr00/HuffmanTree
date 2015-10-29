#include "CommandDispatch.h"

int getUserCommand(int argc, const char ** argv) {
	switch (argc)
	{
	case 4:
		if (strcmp(argv[1],"-c") == 0) {
			if ((setOriginalFilePath(argv[2]) |
				setCompressedFilePath(argv[3])) == 1) {
				return 1;
			}
			encodeFile();
			return 0;
		}
		else if (strcmp(argv[1], "-u") == 0) {
			if ((setOriginalFilePath(argv[3]) |
				setCompressedFilePath(argv[2])) == 1) {
				return 1;
			}
			decodeFile();
			return 0;
		}
	default:
		printErr("Usage: -c original-file compressed-file\n", 
			     "       -u compressed-file original-file\n",
				 "<EOF>");
		return 1;
		break;
	}
}

static void encodeFile() {
	readFromOriginalFile();
	initHuffmanList();
	buildHuffmanTree();
	finalExecution(0);
	writeToFile();
}

static void decodeFile() {
	readFromCompressedFile();
	initHuffmanList();
	buildHuffmanTree();
	finalExecution(1);
}