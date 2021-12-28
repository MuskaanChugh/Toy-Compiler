#include<stdio.h>
#include<stdlib.h>

#include "lookupDef.h"

int hash(char *key);
void insertNode(List* ls, char* key, tokenType tk_tp);
void createTable();
tokenType lookUp(char* lexeme);
void printTable();
