#include "parserDef.h"

Node* insertNode(Node* h, symbol sym);
Node* computeFirst(int nonTerm, Grammar G, Node* firstHead);
FirstAndFollow ComputeFirstAndFollowSets (Grammar G);
int SearchInArray(char* array[], char* ele, int size);
Grammar insertInGrammar(Grammar G, char** line);
Grammar  extractGrammar(Grammar G);
void createParseTable(FirstAndFollow F,  Grammar G);
