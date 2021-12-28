#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "parser.h"

Node* insertNode(Node* h, symbol sym)
{
	Node *temp=(Node*)malloc(sizeof(Node));
	temp->sym=sym;
	temp->nextSym=NULL;
	Node* n=h;
	if (h==NULL){
		h=temp;
		return h;
	}
	while (n->nextSym!=NULL)
		n=n->nextSym;
	n->nextSym=temp;
	return h;
}

int length_rule(Node* head){
	Node* temp = head;
	int count =0;
	while(temp!=NULL){
		temp=temp->nextSym;
		count++;
	}
	return count;

}


Node* computeFirst(int nonTerm, Grammar G, Node* firstHead)
{
	if(f[nonTerm].headFirst!=NULL){
		return f[nonTerm].headFirst;
	}
	Right* rules = G[nonTerm].head;
	int eps_count=0;
	while(rules!=NULL)
	{
		Node* curr = rules->rule;
		if(curr->sym.TorNT == 0)
		{
			firstHead = insertNode(firstHead, curr->sym);
		}
		else 															//Non-terminal
		{
			eps_count=0;
			while(curr!=NULL)
			{
				Node* dHead = computeFirst(curr->sym.e.nt,G,firstHead);
				Node* temp = dHead;
				int eps_flag=0;
				while(temp!=NULL)
				{
					//firstHead = insertInLL(firstHead, temp -> sym);
					if(temp->sym.e.t!=eps){
						firstHead = insertNode(firstHead, temp->sym);
					}
					else{
						eps_flag=1;
					}
					temp=temp->nextSym;
				}
				if(eps_flag){
					eps_count++;
					curr=curr->nextSym;
				}
			}

		}
		if(eps_count==length_rule(rules->rule));
		rules = rules->nextRule;
	}
}

Node* computeFollow(int nonTerm, Grammar G, Node* followHead, int visited[]) {
	visited[nonTerm]=1;
	for (int i=0; i<no_NT;i++) {
		Right* rules = G[i].head;
		while(rules!=NULL){
			Node* currSymNode = rules->rule;
			while(currSymNode!=NULL){
				if(currSymNode->sym.e.nt!=nonTerm){
					currSymNode=currSymNode->nextSym;
				}
				else{
					Node* checkNext = currSymNode->nextSym;
					Node* tempFollow=NULL;
					// 3 cases
					// case 1 : null
					if(checkNext==NULL){
						if(i<nonTerm){
							tempFollow = f[i].headFollow;
						}
						else{
							if(visited[i]==0)
								tempFollow = computeFollow(i,G,tempFollow,visited);
						}
						while(tempFollow!=NULL){
							followHead = insertNode(followHead,tempFollow->sym);
							tempFollow=tempFollow->nextSym;
						}
					}
					
					// case 2 : terminal
					else if(checkNext->sym.TorNT == 0){
						followHead = insertNode(followHead, checkNext->sym);
					}

					// case 3 : non terminal 
					else if(checkNext->sym.TorNT==1){
						int eps_flag=1;
						while(eps_flag){
							if(checkNext==NULL){
								Node* tempFollow;
								if(i<nonTerm){
									tempFollow = f[i].headFollow;
								}
								else{
									if(visited[i]==0)
										tempFollow = computeFollow(i,G,tempFollow,visited);
								}
								while(tempFollow!=NULL){
									followHead = insertNode(followHead,tempFollow->sym);
									tempFollow=tempFollow->nextSym;
								}
								break;
							}
							eps_flag=0;
							Node* firstNextSym = f[checkNext->sym.e.nt].headFirst;
							Node* temp = firstNextSym;
							while(temp!=NULL){
								if(temp->sym.e.t!=eps){
									followHead = insertNode(followHead, temp->sym);
								}
								else{
									eps_flag=1;
								}
								temp=temp->nextSym;
							}
							if(eps_flag){
								checkNext = checkNext->nextSym;
							}
						}
						
					}
					//  		case 2a:  first of this non terminal does not contain eps
					//			case 2b:  first of this non terminal contains eps

				}
			}
			
		}
	}
return followHead;

}


FirstAndFollow ComputeFirstAndFollowSets (Grammar G)
{
	f = (row*) malloc(sizeof(row)*no_NT);
	for(int i= 0;i<no_NT;i++){
		f[i].headFirst = NULL;
		f[i].headFollow = NULL;
	}
	for(int i = 0;i<no_NT;i++)
	{
		f[i].headFirst = computeFirst(i,G,f[i].headFirst);
	}

	for(int i = 0;i<no_NT;i++)
	{
		int visited[no_NT];
		for(int j=0;j<no_NT;j++){
			visited[j]=0;
		}
		f[i].headFollow = computeFollow(i,G,f[i].headFollow,visited);
	}
	return f;

}

int SearchInArray(char* array[], char* ele, int size){
	for(int i=0;i<size;i++){
		if(strcmp(array[i],ele)==0){
			return i;
		}
	}
	return -1;
}



Grammar insertInGrammar(Grammar G, char** line){

	int flag=1;
	int index = SearchInArray(NonTerminals, line[0],no_NT);
	if(index==-1){
		index = SearchInArray(Terminals, line[0],no_T);
		flag=0;
		if(index==-1){
			printf("error in lhs\n");
			exit(1);
		}
	}

	G[index].sym.TorNT=flag;
	if(flag==0){
		G[index].sym.e.t=index;
	}
	else{
		G[index].sym.e.nt=index;
	}
	G[index].head=NULL;
	G[index].count=0;



	Right* temp;


	for(int i=1;strcmp(line[i],"\n")!=0;){
		G[index].count++;

		if(G[index].head==NULL){
			G[index].head=(Right*)malloc(sizeof(Right));
			temp=G[index].head;
			temp->nextRule=NULL;
		}
		else{
			temp=G[index].head;
			while(temp->nextRule!=NULL){
				temp=temp->nextRule;
			}
			temp->nextRule=(Right*)malloc(sizeof(Right));
			temp=temp->nextRule;
			temp->nextRule=NULL;
		}
		while(strcmp(line[i],"|")!=0 && strcmp(line[i],"\n")!=0){
			symbol s;
			int f=1;
			int ind = SearchInArray(NonTerminals, line[i],no_NT);
			if(ind==-1){
				ind = SearchInArray(Terminals, line[i],no_T);
				f=0;
				if(ind==-1){
					printf("%s\n",line[i]);
					printf("error in rhs\n");
					exit(1);
				}
			}
			s.TorNT=f;
			if(f==0){
				s.e.t=ind;
			}
			else{
				s.e.nt=ind;
			}


			temp->rule=insertNode(temp->rule,s);
			i++;
		}
		if(strcmp(line[i],"\n")!=0)
			i++;
	}


return G;

//	printf("%d\n",G[index].sym.e);
}

Grammar  extractGrammar(Grammar G){

	G = (Grammar)malloc(sizeof(rHead)*no_rules);
	char *s = (char*)malloc(150);
    FILE *fptr;

    if ((fptr = fopen("Grammar.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }
	int i=0;
	fgets(s,150, fptr);
	while(!feof(fptr)){
		i=0;
		char** line = (char**)malloc(sizeof(char*)*50);
		for(int j=0;j<50;j++)
			line[j]=(char*)malloc(sizeof(char)*150);
		char* token = strtok(s, " ");
		while (token != NULL) {

			line[i++]=token;
			token = strtok(NULL, " ");
		}
/*		for(int z=0;z<=i;z++){
			printf("%s ",line[z]);
		}
		printf("\n");
*/
		G=insertInGrammar(G,line);

		fgets(s,150, fptr);


	}
    fclose(fptr);
return G;
}


void createParseTable(FirstAndFollow F,  Grammar G){
	synNode = (Node*)malloc(sizeof(Node));
	synNode -> sym.e.t = TK_SYN;
	synNode -> sym.TorNT = 0;
	synNode -> nextSym = NULL;
	//imitializing parse table with error
	//deal with syn
	for(int i=0;i<no_NT;i++){
		for(int j=0;j<no_T;j++){
			T[i][j]=NULL;
		}
	}
	for(int i=0;i<no_NT;i++){
			Right* temp = G[i].head;
			for(int j=0;j<G[i].count && temp!=NULL;j++){
				Node* rulehead = temp->rule;
				symbol s = rulehead->sym;
				printf("i= %d\n",i);
				printf("Symbol %d\n",s.e);
				if(s.e.t ==eps){

					Node* curr = F[s.e.nt].headFollow;  	//using row structure
					while(curr!=NULL){
						printf("%d --- curr->sym.e.t\n ", curr->sym.e.t);
						T[i][curr->sym.e.t]=rulehead;
						curr=curr->nextSym;
					}

					temp=temp->nextRule;
				}
				else{
					if (s.TorNT==1) {
						Node* curr = F[s.e.nt].headFirst;  	//using row structure
						while(curr!=NULL && curr->sym.e.t!=eps){

							T[i][curr->sym.e.t]=rulehead;
							curr=curr->nextSym;
						}
					}
					else
					{
						T[i][s.e.t]=rulehead;
					}
					temp=temp->nextRule;
				}

			}
			Node* followTrv = F[i].headFollow;
			while(followTrv!=NULL)
			{
				if(T[i][followTrv->sym.e.t] == NULL)
				{
					T[i][followTrv->sym.e.t] =synNode;
				}
				followTrv = followTrv->nextSym;
			}
		}
	}

int main(){
	Grammar G;
	G=extractGrammar(G);
	f  = ComputeFirstAndFollowSets (G);
	createParseTable(f,G);

printf("here\n");
	//T = createParseTable(F, T, G);

return 0;
}

//	printf("%u\n",G[program].head->rule->sym.e.t);
/*
	FirstAndFollow firstFollowTable = (FirstAndFollow)malloc((no_NT)*sizeof(row));

firstFollowTable[2].headFirst = (Node*)malloc(sizeof(Node));
firstFollowTable[2].headFirst->sym.e.t = TK_FUNID;
firstFollowTable[2].headFollow = (Node*)malloc(sizeof(Node));
firstFollowTable[2].headFollow->sym.e.t = TK_MAIN;
firstFollowTable[2].headFirst->nextSym = (Node*)malloc(sizeof(Node));
firstFollowTable[2].headFirst->nextSym->sym.e.t = eps;
firstFollowTable[2].headFirst->nextSym->nextSym=NULL;
firstFollowTable[2].headFollow->nextSym=NULL;

firstFollowTable[0].headFirst = (Node*)malloc(sizeof(Node));
firstFollowTable[0].headFirst->sym.e.t = TK_FUNID;
firstFollowTable[0].headFollow = (Node*)malloc(sizeof(Node));
firstFollowTable[0].headFollow->sym.e.t = TK_DOLLAR;
firstFollowTable[0].headFollow->nextSym=NULL;
firstFollowTable[0].headFirst->nextSym=NULL;

firstFollowTable[1].headFirst = (Node*)malloc(sizeof(Node));
firstFollowTable[1].headFirst->sym.e.t = TK_MAIN;
firstFollowTable[1].headFollow = (Node*)malloc(sizeof(Node));
firstFollowTable[1].headFollow->sym.e.t = TK_DOLLAR;
firstFollowTable[1].headFollow->nextSym=NULL;
firstFollowTable[1].headFirst->nextSym=NULL;

firstFollowTable[3].headFirst = (Node*)malloc(sizeof(Node));
firstFollowTable[3].headFirst->sym.e.t = TK_FUNID;
firstFollowTable[3].headFirst->nextSym=NULL;
firstFollowTable[3].headFollow = (Node*)malloc(sizeof(Node));
firstFollowTable[3].headFollow->sym.e.t = TK_MAIN;
firstFollowTable[3].headFollow->nextSym = (Node*)malloc(sizeof(Node));
firstFollowTable[3].headFollow->nextSym->sym.e.t = TK_FUNID;
firstFollowTable[3].headFollow->nextSym->nextSym=NULL;

firstFollowTable[4].headFirst = (Node*)malloc(sizeof(Node));
firstFollowTable[4].headFirst->sym.e.t = TK_INPUT;
firstFollowTable[4].headFirst->nextSym=NULL;

firstFollowTable[4].headFollow = (Node*)malloc(sizeof(Node));
firstFollowTable[4].headFollow->sym.e.t = TK_OUTPUT;
firstFollowTable[4].headFollow->nextSym = (Node*)malloc(sizeof(Node));
firstFollowTable[4].headFollow->nextSym->sym.e.t = TK_SEM;
firstFollowTable[4].headFollow->nextSym->nextSym=NULL;



*/



