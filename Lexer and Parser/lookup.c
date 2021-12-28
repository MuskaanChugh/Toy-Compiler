
#include<stdio.h>
#include<stdlib.h>

#include "lookup.h"

int hash(char *key)
{
  int index=0;
  for (int i=0; key[i]!='\0';i++)
    index=((index*31)+ (key[i] - '0'))%41;
  return index;
}

void insertNode(List* ls, char* key, tokenType tk_tp)
{
  Node *newNode=(Node *)malloc(sizeof(Node));
  newNode->type=tk_tp;
  newNode->keyword=key;
  newNode->next=NULL;
  if (ls->head==NULL)
    ls->head=newNode;
  else
  {
    Node *curr=ls->head;
    while (curr->next)
      curr=curr->next;
    curr->next = newNode;
  }
  ls -> count++;
}

void createTable()
{
  lookupTable  = (List**)malloc(sizeof(List*)*no_slots);
  for(int i = 0;i<no_slots;i++)
  {
    lookupTable[i] = (List*)malloc(sizeof(List));
    lookupTable[i] -> head = NULL;
    lookupTable[i] -> count = 0;
  }
  insertNode(lookupTable[hash("with")], "with",TK_WITH);
  insertNode(lookupTable[hash("parameters")], "parameters",TK_PARAMETERS);
  insertNode(lookupTable[hash("end")], "end",TK_END);
  insertNode(lookupTable[hash("while")], "while",TK_WHILE);
  insertNode(lookupTable[hash("type")], "type",TK_TYPE);
  insertNode(lookupTable[hash("_main")], "_main",TK_MAIN);
  insertNode(lookupTable[hash("global")], "global",TK_GLOBAL);
  insertNode(lookupTable[hash("parameter")], "parameter",TK_PARAMETER);
  insertNode(lookupTable[hash("list")], "list",TK_LIST);
  insertNode(lookupTable[hash("input")], "input",TK_INPUT);
  insertNode(lookupTable[hash("output")], "output",TK_OUTPUT);
  insertNode(lookupTable[hash("int")], "int",TK_INT);
  insertNode(lookupTable[hash("real")], "real",TK_REAL);
  insertNode(lookupTable[hash("endwhile")], "endwhile",TK_ENDWHILE);
  insertNode(lookupTable[hash("if")], "if",TK_IF);
  insertNode(lookupTable[hash("then")], "then",TK_THEN);
  insertNode(lookupTable[hash("endif")], "endif",TK_ENDIF);
  insertNode(lookupTable[hash("read")], "read",TK_READ);
  insertNode(lookupTable[hash("write")], "write",TK_WRITE);
  insertNode(lookupTable[hash("return")], "return",TK_RETURN);
  insertNode(lookupTable[hash("call")], "call",TK_CALL);
  insertNode(lookupTable[hash("record")], "record",TK_RECORD);
  insertNode(lookupTable[hash("endrecord")], "endrecord",TK_ENDRECORD);
  insertNode(lookupTable[hash("else")], "else",TK_ELSE);


}

tokenType lookUp(char* lexeme)
{
  printf("%s ----- LOOKUP\n",lexeme);
  int hashValue = hash(lexeme);
  Node* temp = lookupTable[hashValue] -> head;
  while(temp!= NULL && strcmp(temp->keyword, lexeme) != 0)
    temp = temp -> next;
  if(temp == NULL)
  {
    if(lexeme[0] == '_')
      return TK_FUNID;
    else
      return TK_ID;
  }
  else
    return temp -> type;
}

void printTable()
{
  for(int i = 0;i<no_slots;i++)
  {
    Node* current = lookupTable[i] -> head;
    while(current)
    {
      printf("%u --> %s ", current->type, current -> keyword);
      current = current -> next;
    }
    printf("\n");
  }
}
/*
int main()
{
  createTable();
  printTable();
  printf("%u ----l",lookUp("end"));
}*/
