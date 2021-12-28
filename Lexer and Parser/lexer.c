#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "lookup.h"
//#include "lexerDef.h"
#define BUFFERSIZE 4096
int it = 0;                            //Traking the next token in getNextToken() function
unsigned long long lineno = 1;         //track the line number (used in getNextToken())
char* previous_buffer;
char* current_buffer;

FILE* getStream(FILE* fp)
{
  // exchanging the previous_buffer and current_buffer
	char* temp = previous_buffer;
	previous_buffer = current_buffer;
	current_buffer = temp;
  //initialize the current_buffer
	current_buffer = (char*)malloc(sizeof(char)*(BUFFERSIZE+1));
	previous_buffer = (char*)malloc(sizeof(char)*(BUFFERSIZE+1));
	memset(current_buffer,0,sizeof(char)*BUFFERSIZE);
  //loading the current buffer
	if(feof(fp))
	{
		printf("End of file\n");
		fclose(fp);
		return NULL;
	}
	if(fread(current_buffer, sizeof(char), BUFFERSIZE, fp) > 0)
	{
		current_buffer[BUFFERSIZE] = '\0';
		return fp;
	}
	else
	{
		fclose(fp);
		return NULL;
	}
}


Token* addToken(unsigned long long lineno, tokenType tp, char* lexeme)
{
	//printf("addToken Called\n");
	Token* tk = (Token*)malloc(sizeof(Token));
	tk->lineNo = lineno;
	tk-> type = tp;
	tk->lexeme = (char*)malloc(sizeof(char)*200);
	printf("%s  ", lexeme);
	strcpy(tk -> lexeme,lexeme);
	return tk;
}

Token* getNextToken()
{

	Token* tk;
	int state = 0;
	char* lexeme = (char*)malloc(sizeof(char)*200);
	int lexIt = 0;
	while(1)
	{
		//printf("%c  %d\n", current_buffer[it], state);
		switch(state)
		{
		case 0:
			if(current_buffer[it] == '~')
			{
				state = 1;
				tk = addToken(lineno, TK_NOT, "~");
				it++;
			}
			else if(current_buffer[it] == '<')
			{
				state = 2;
				it++;
			}
			else if(current_buffer[it] == '>')
			{
				state = 8;
				it++;
			}
			else if(current_buffer[it] == '=')
			{
				state = 11;
				it++;
			}
			else if(current_buffer[it] == '!')
			{
				state = 13;
				it++;
			}
			else if(current_buffer[it] == '%')
			{
				state = 15;
				tk = addToken(lineno,TK_COMMENT,"comment");
				while(current_buffer[it]!='\n' && current_buffer[it]!='\0')
				{
					it++;
				}

			}
			else if(current_buffer[it] == '@')
			{
				state = 16;
				it++;
			}
			else if(current_buffer[it] == '&')
			{
				state = 19;
				it++;
			}
			else if(current_buffer[it] == '+')
			{
				state = 23;
				tk = addToken(lineno, TK_PLUS, "+");
				it++;
			}
			else if(current_buffer[it] == '-')
			{
				state = 24;
				tk = addToken(lineno, TK_MINUS, "-");
				it++;
			}
			else if(current_buffer[it] == '*')
			{
				state = 25;
				tk = addToken(lineno, TK_MUL, "*");
				it++;
			}
			else if(current_buffer[it] == '/')
			{
				state = 26;
				tk = addToken(lineno, TK_DIV, "/");
				it++;
			}
			else if(current_buffer[it] == '(')
			{
				state = 27;
				tk = addToken(lineno, TK_OP, "(");
				it++;
			}
			else if(current_buffer[it] == ')')
			{
				state = 28;
				tk = addToken(lineno, TK_CL, ")");
				it++;
			}
			else if(current_buffer[it] == ',')
			{
				state = 29;
				tk = addToken(lineno, TK_COMMA, ",");
				it++;
			}
			else if(current_buffer[it] == ';')
			{
				state = 30;
				tk = addToken(lineno, TK_SEM, ";");
				it++;
			}
			else if(current_buffer[it] == ':')
			{
				state = 31;
				tk = addToken(lineno, TK_COLON, ":");
				it++;
			}
			else if(current_buffer[it] == '.')
			{
				state = 32;
				tk = addToken(lineno, TK_DOT, ".");
				it++;
			}
			else if(current_buffer[it] == '[')
			{
				state = 33;
				tk = addToken(lineno, TK_SQL, "[");
				it++;
			}
			else if(current_buffer[it] == ']')
			{
				state = 34;
				tk = addToken(lineno, TK_SQR, "]");
				it++;
			}
			else if(current_buffer[it] >= 'a' && current_buffer[it]<='z'&& current_buffer[it] != 'b' && current_buffer[it] != 'c' &&  current_buffer[it] != 'd')
			{
				state  = 35;
				lexeme[lexIt++] = current_buffer[it++];
			}
			else if(current_buffer[it] >= 'b' && current_buffer[it] <='d')
			{
				state = 36;
				lexeme[lexIt++] = current_buffer[it++];
			}
			else if(current_buffer[it] >= '0' && current_buffer[it] <='9')
			{
				state = 39;
				lexeme[lexIt++] = current_buffer[it++];
			}
			else if(current_buffer[it] == '#')
			{
				state = 43;
				lexeme[lexIt++] = current_buffer[it++];
			}
			else if(current_buffer[it] == '_')
			{
				state = 45;
				lexeme[lexIt++] = current_buffer[it++];
			}
			else if(current_buffer[it] == ' ' || current_buffer[it] == '\t')
			{
				//printf("this is space\n");
				it++;
				state = 0;
			}
			else if(current_buffer[it] == '\n')
			{
				it++;
				state = 0;
				lineno++;
			}
			else if(current_buffer[it] == '\0')
			{
				tk = addToken(lineno, TK_EOF, "eof");
				return tk;
			}


			//Add for new line etc etc
			else
			{
				state = 100;
			}break;

		case 1: return tk;
		case 2:
			if(current_buffer[it] == '=')
			{
				state = 4;
				tk = addToken(lineno,TK_LE,"<=");
				it++;
			}
			else if(current_buffer[it] == '-')
			{
				state = 5;
				it++;
			}
			else
			{
				state = 3;
		      tk = addToken(lineno,TK_LT,"<");               //Retracting state
			}break;
		case 3: return tk;
		case 4: return tk;
		case 5:
			if(current_buffer[it] == '-')
			{
				state = 6;
				it++;
			}
			else
		    {
		    	state = 100;                                   //ERROR STATE
		    }break;
		case 6:
			if(current_buffer[it] == '-')
			{
				state = 7;
				tk = addToken(lineno, TK_ASSIGNOP, "<---");
				it++;
			}
			else
			{
				state = 100;
			}break;
		case 7: return tk;
		case 8:
				if(current_buffer[it] == '=')
				{
					state = 10;
					tk = addToken(lineno, TK_GE, ">=");
					it++;
				}
				else
				{
					state = 9;
		      		tk = addToken(lineno, TK_GT, ">");             //Retracting state
				}break;
		case 9 : return tk;
		case 10: return tk;
		case 11:
				if(current_buffer[it] == '=')
				{
					state = 12;
					tk = addToken(lineno, TK_EQ, "==");
					it++;
				}
				else
				{
		       		state = 100;                                 //ERROR
				}break;
		case 12: return tk;
		case 13:
				if(current_buffer[it] == '=')
				{
					state = 12;
					tk = addToken(lineno, TK_NE, "!=");
					it++;
				}
				else
				{
		       		state = 100;                                 //ERROR
				}break;
		case 14: return tk;
		case 15: return tk;
		case 16:
				if(current_buffer[it] == '@')
				{
					state = 17;
					it++;
				}
				else
				{
		    	   state = 100;                                 //ERROR
				}break;
		case 17:
				if(current_buffer[it] == '@')
				{
					state = 18;
					tk = addToken(lineno, TK_OR, "@@@");
					it++;
				}
				else
				{
		       		state = 100;                                 //ERROR
				}break;
		case 18: return tk;
		case 19:
				if(current_buffer[it] == '&')
				{
					state = 20;
					it++;
				}
				else
				{
		       		state = 100;                                 //ERROR
				}break;
		case 20:
				if(current_buffer[it] == '&')
				{
					state = 21;
					tk = addToken(lineno, TK_AND, "&&&");
					it++;
				}
				else
				{
		       		state = 100;                                 //ERROR
				}break;
		case 21: return tk;
		case 23: return tk;
		case 24: return tk;
		case 25: return tk;
		case 26: return tk;
		case 27: return tk;
		case 28: return tk;
		case 29: return tk;
		case 30: return tk;
		case 31: return tk;
		case 32: return tk;
		case 33: return tk;
		case 34: return tk;
		case 35:
				while(current_buffer[it] >= 'a' && current_buffer[it] <= 'z')
				{
					//printf("%d --case35\n", lexIt);
					lexeme[lexIt++] = current_buffer[it++];

				}
				//printf("%s --case35\n", lexeme);
				tk = addToken(lineno, lookUp(lexeme), lexeme);
				return tk;
		case 36:
				if(current_buffer[it] >= '2' && current_buffer[it] <= '7')
				{
					state = 37;
					lexeme[lexIt++] = current_buffer[it++];
				}
				else if(current_buffer[it] >= 'a' && current_buffer[it] <= 'z')
				{
					state = 35;
					lexeme[lexIt++] = current_buffer[it++];
				}
				else
				{
					state = 100;
				}break;
		case 37:
				while(current_buffer[it] >= 'b' && current_buffer[it] <= 'd')
				{
					lexeme[lexIt++] = current_buffer[it++];
					if(lexIt > 20)
					{
						state = 100;
						break;
					}
				}
				if(current_buffer[it] >= '2' && current_buffer[it] <= '7')
				{
					state = 38;
					lexeme[lexIt++] = current_buffer[it++];
				}
				else
				{
					tk = addToken(lineno, lookUp(lexeme), lexeme);
					return tk;
				}break;
		case 38:
				while(current_buffer[it] >= '2' && current_buffer[it] <= '7')
				{
					lexeme[lexIt++] = current_buffer[it++];
					if(lexIt > 20)
					{
						state = 100;
						break;
					}
				}
				tk = addToken(lineno,lookUp(lexeme),lexeme);
				return tk;
		case 39:
				while(current_buffer[it] >= '0' && current_buffer[it] <= '9')
				{
					lexeme[lexIt++] = current_buffer[it++];
				}
				if(current_buffer[it] == '.')
				{
					state = 41;
					lexeme[lexIt++] = current_buffer[it++];
				}
				else
				{
					state = 40;
					tk = addToken(lineno, TK_NUM,lexeme);                          	  //Retracting state
				}break;
		case 40: return tk;
		case 41:
				if(current_buffer[it] >= '0' && current_buffer[it] <= '9')
				{
					state = 42;
					lexeme[lexIt++] = current_buffer[it++];
				}
				else
				{
					state = 100;
				}
		case 42:
				while(current_buffer[it] >= '0' && current_buffer[it] <= '9')
				{
					lexeme[lexIt++] = current_buffer[it++];
				}
				tk = addToken(lineno, TK_RNUM,lexeme);
				return tk;
		case 43:
				if(current_buffer[it] >= 'a' && current_buffer[it] <= 'z')
				{
					state = 44;
					lexeme[lexIt++] = current_buffer[it++];
				}
				else
				{
					state = 100;
				}
		case 44:
				while(current_buffer[it] >= 'a' && current_buffer[it] <= 'z')
				{
					lexeme[lexIt++] = current_buffer[it++];
				}
				tk = addToken(lineno, TK_RECORDID,lexeme);
				return tk;
		case 45:
				if((current_buffer[it] >= 'a' && current_buffer[it] <= 'z') || (current_buffer[it] >= 'A' && current_buffer[it] <= 'Z'))
				{
					state = 46;
					lexeme[lexIt++] = current_buffer[it++];
				}
				else
				{
					state = 100;
				}break;
		case 46:
				while((current_buffer[it] >= 'a' && current_buffer[it] <= 'z') || (current_buffer[it] >= 'A' && current_buffer[it] <= 'Z'))
				{
					state = 47;
					lexeme[lexIt++] = current_buffer[it++];
					if(lexIt > 30)
					{
						state = 100;
						break;
					}
				}
				if(current_buffer[it] >= '0' && current_buffer[it] <= '9')
				{
					state = 47;
					lexeme[lexIt++] = current_buffer[it++];
				}
				else
				{
					tk = addToken(lineno, lookUp(lexeme), lexeme);
					return tk;
				}break;
		case 47:
				while(current_buffer[it] >= '0' && current_buffer[it] <= '9')
				{
					lexeme[lexIt++] = current_buffer[it++];
					if(lexIt > 30)
					{
						state = 100;
						break;
					}
				}
				tk = addToken(lineno, lookUp(lexeme), lexeme);
				return tk;
		case 100:
				tk = addToken(lineno, TK_ERROR, NULL);
				return tk;
	}
}
}

void removeComments(char *testcaseFile, char *cleanFile)
{
	FILE* rawFile = fopen(testcaseFile, "r");
	FILE* output = fopen(cleanFile, "w");
	if(rawFile == NULL)
	{
		printf("Error while opening the file\n");
		return;
	}
  //loading buffer
	rawFile = getStream(rawFile);
  //int commentStart = 0;
	for(int i = 0; current_buffer[i] != '\0';i++)
	{
		printf("%c ",current_buffer[i]);
		if(current_buffer[i] == '%')
		{
      //commentStart = i;
			while(current_buffer[i] != '\n' && current_buffer[i] != '\0')
				{i++;}
      //commentStart = 0;
		}
		char c = current_buffer[i];
		fwrite(&c, sizeof(char), 1, output);
	}
  //fwrite(current_buffer, sizeof(char), BUFFERSIZE+1, output);
	fclose(rawFile);
	fclose(output);
}

int main()
{
	createTable();
	char* a = (char*)malloc(sizeof(char)*50);
	a = "testcase1.txt";
	FILE* fp = fopen(a,"r");
	fp = getStream(fp);
	while (current_buffer[it]!='\0')
	{
		Token* tk=getNextToken();
		printf("%llu --> Line Number, %s --> lexeme, %u --> type\n",tk->lineNo, tk-> lexeme, tk-> type);
	}
	fclose(fp);
}
