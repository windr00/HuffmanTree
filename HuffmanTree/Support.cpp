#include "Support.h"

void printErr(const char * FirstStr, ...) {
	va_list ArgList;
	char * Temp = NULL;
	va_start(ArgList, FirstStr);
	printf("%s", FirstStr);
	while ((Temp = va_arg(ArgList, char *)) != END) {
		printf("%s", Temp);
	}
	va_end(ArgList);
}