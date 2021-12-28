#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "parseTree.h"

Stack* push(Stack *stack, symbol s)
{
  Stack *temp=(Stack *)malloc(sizeof(Stack));
  temp->sym=s;
  temp->next=stack;
  return temp;
}

symbol top(Stack *stack)
{
  return stack->sym;
}

Stack* pop(Stack *stack)
{
  Stack *temp=stack;
  stack=stack->next;
  free(temp);
  return stack;
}

Stack* pushLLrev(Stack* stack,Node* rule)
{
  Node* curr = rule;
  while(curr!=NULL)
  {
    stack = push(stack, curr->sym);
    curr = curr->nextSym;
  }
  return stack;
}

//function to get current node
parseTreeNode* traversal(parseTreeNode* root)
{
  if(root == NULL)
    return NULL;
  if(root->no_children == 0 && root->tNode.TorNT == 1)
    return root;
  else if(root->no_children == 0 && root->tNode.TorNT == 0)
  {
    parseTreeNode* curr = root ;
    while(curr!=NULL)
    {
      if(curr->tNode.TorNT == 0)
        curr = curr->next;
      else
        return curr;
    }
    parseTreeNode* temp =root;
    if(curr == NULL)
    {
      parseTreeNode* res = traversal(temp->parent->next);
      while(res==NULL)
      {
        temp=temp->parent;
        res = traversal(temp->parent->next);
      }
      return res;
    }
  }
  return NULL;
}
parseTreeNode* insertRule(parseTreeNode* parent, Node* rule)
{
  Node* temp = rule;
  parent -> leaforNot = 0;
  parseTreeNode* firstChild = (parseTreeNode*)malloc(sizeof(parseTreeNode));
  firstChild -> tNode = temp -> sym;
  firstChild -> parent = parent;
  firstChild -> leaforNot = 1;
  firstChild -> no_children = 0;
  firstChild -> children= NULL;
  firstChild -> nodeToken = NULL;
  firstChild -> next = NULL;
  parent -> no_children++;
  parseTreeNode* curr = firstChild;
  temp = temp->nextSym;
  //parent -> children = child;
  while(temp!=NULL)
  {
    parseTreeNode* sibling = (parseTreeNode*)malloc(sizeof(parseTreeNode));
    sibling->tNode = temp -> sym;
    sibling -> no_children = 0;
    sibling -> parent = parent;
    sibling -> leaforNot = 1;
    sibling -> children = NULL;
    sibling -> nodeToken = NULL;
    sibling  -> next = NULL;
    curr -> next = sibling;
    curr = curr -> next;
    parent -> no_children++;
    temp = temp->nextSym;
  }
  parent -> children = firstChild;
  return parent;
}

parseTreeNode* parseInputSourceCode(char *testcaseFile)
{
  parseTreeNode* PT = (parseTreeNode*)malloc(sizeof(parseTreeNode));
  PT -> tNode.e.nt = program;
  PT-> parent = NULL;
  PT-> leaforNot = 1;
  PT-> no_children = 0;
  PT-> children= NULL;
  PT-> nodeToken = NULL;
  PT-> next = NULL;

  FILE* fptr = fopen(testcaseFile, "r");
  fptr = getStream(fptr);
  Stack* stack = (Stack*)malloc(sizeof(Stack));
  symbol s, dollar;
  s.e.nt = program;                                     //Start Symbol = s
  s.TorNT = 1;
  dollar.e.t = TK_EOF;                           //Denotes i/p end and bottom element of stack
  dollar.TorNT = 0;
  stack = push(stack,dollar);
  stack = push(stack,s);
  Token* tk = getNextToken();
  parseTreeNode* currTNode = PT;
  while (tk->type != TK_EOF)
	{
    if(top(stack).TorNT)                              //This is for nonTerminal in stack
    {
      Node* rule = T[top(stack).e.nt][tk->type];
      if(rule != NULL && rule->sym.e.t != synNode->sym.e.t)           //it's a rule
      {
          currTNode = traversal(currTNode);
          currTNode -> tNode = top(stack);
          currTNode -> nodeToken = tk;
          currTNode = insertRule(currTNode, rule);
          stack = pop(stack);
          stack = pushLLrev(stack,rule);                       //push a LL in a stack in reverse order
      }
      else if(rule == NULL)                                   //it's an Error(from table)
      {
        printf("Error in line number %llu. Now I am handling it\n", tk -> lineNo);
        while(tk->type != TK_EOF)
        {
          if(T[top(stack).e.nt][tk->type]-> sym.e.t == synNode->sym.e.t)                 //syn --> pop
          {
            stack = pop(stack);
            break;
          }
          else if(T[top(stack).e.nt][tk->type]!=NULL)               //It's a rule again.(Handling error)
          {
            currTNode = traversal(currTNode);
            currTNode -> tNode = top(stack);
            currTNode -> nodeToken = tk;
            currTNode = insertRule(currTNode, T[top(stack).e.nt][tk->type]);
            stack = pushLLrev(stack,rule);               //push a LL in a stack in reverse order
            break;
          }
          tk = getNextToken();
        }

      }
      else
      {
        stack = pop(stack);
      }
    }
    else if(top(stack).TorNT == 0)      //if top(stack) is a Terminal
    {
      if(tk->type == top(stack).e.t)
      {
        pop(stack);
        currTNode = traversal(currTNode);
        currTNode -> tNode = top(stack);
        currTNode -> nodeToken = tk;
        tk = getNextToken();
      }                                      //Else case will never occur!!!!
    }                                        //Also other else won't occur as Stack can never be empty
  }
  if(top(stack).e.t == tk-> type && tk->type == TK_EOF)
  {
    printf("-------Syntactically Correct !! Successful Compilation-------");
  }
  return PT;
  //printf("%llu --> Line Number, %s --> lexeme, %u --> type\n",tk->lineNo, tk-> lexeme, tk-> type);
}

/*
void printParseTree(parseTreeNode* PT, char *outfile)
{
  if(PT == NULL)
    return;
  FILE
}
*/
