#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "parser.h"

struct parseTreeNode{
  symbol tNode;
  struct parseTreeNode* parent;
  int leaforNot;
  int no_children;
  Token* nodeToken;
  struct parseTreeNode* children;
  struct parseTreeNode* next;
};
typedef struct parseTreeNode parseTreeNode;

struct Stack{
  symbol sym;
  struct Stack *next;
};

typedef struct Stack Stack;

//Functions
Stack* push(Stack *stack, symbol s);
symbol top(Stack *stack);
Stack* pop(Stack *stack);
Stack* pushLLrev(Stack* stack,Node* rule);
parseTreeNode* traversal(parseTreeNode* root);
parseTreeNode* insertRule(parseTreeNode* parent, Node* rule);
parseTreeNode* parseInputSourceCode(char *testcaseFile);
