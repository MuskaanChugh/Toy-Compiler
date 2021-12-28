#include<stdio.h>
#include<stdlib.h>

#include "lexerDef.h"

FILE* getStream(FILE* fp);
Token* addToken(unsigned long long lineno, tokenType tp, char* lexeme);
Token* getNextToken();
void removeComments(char *testcaseFile, char *cleanFile);
