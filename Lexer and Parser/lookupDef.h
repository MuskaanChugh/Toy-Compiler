#include<stdio.h>
#include<string.h>
#include<stdio.h>

#include "lexer.h"
#define no_slots 41
#define no_key 24
struct node
{
  tokenType type;
  char* keyword;
  struct node* next;
};
typedef struct node Node;

struct list
{
  Node* head;
  int count;
};
typedef struct list List;

List** lookupTable;
