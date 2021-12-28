#include "lexer.h"

#define no_rules 50
#define no_NT 50
#define no_T 58

/*
enum sym { program, mainFunction, otherFunctions, function, input_par,
			output_par, parameter_list, dataType, primitiveDatatype, constructedDatatype,
			remaining_list, stmts, typeDefinitions, typeDefinition, fieldDefinitions,
			fieldDefinition, moreFields, declarations, declaration, global_or_not,
			otherStmts, stmt, assignmentStmt, singleOrRecId, funCallStmt,
			outputParameters, inputParameters, iterativeStmt, conditionalStmt, elsePart,
			ioStmt, allVar, arithmeticExpression, expPrime, term,
			termPrime, factor, highPrecedenceOperator, lowPrecedenceOperators, all,
			temp, booleanExpression, var, logicalOp, relationalOp,
			returnStmt, optionalReturn, idList, more_ids,                                  //49 Non terminals
			TK_ASSIGNOP, TK_COMMENT,  TK_FIELDID,  TK_ID,  TK_NUM,
			TK_RNUM,  TK_FUNID,  TK_RECORDID,  TK_WITH,  TK_PARAMETERS,
			TK_END,  TK_WHILE,  TK_TYPE,  TK_MAIN,  TK_GLOBAL,
  			TK_PARAMETER,  TK_LIST,  TK_SQL,  TK_SQR,  TK_INPUT,
  			TK_OUTPUT,  TK_INT,  TK_REAL,  TK_COMMA,  TK_SEM,
  			TK_COLON, TK_DOT,  TK_ENDWHILE,  TK_OP,  TK_CL,
  			TK_IF,  TK_THEN,  TK_ENDIF,  TK_READ,  TK_WRITE,
  			TK_RETURN,  TK_PLUS,  TK_MINUS,  TK_MUL,  TK_DIV,
  			TK_CALL, TK_RECORD,  TK_ENDRECORD,  TK_ELSE,  TK_AND,
  			TK_OR,  TK_NOT,  TK_LT,  TK_LE,  TK_EQ,
  			TK_GT,  TK_GE,  TK_NE};															//53 Terminals
typedef enum sym sym;
*/
enum nonTerm { program, mainFunction, otherFunctions, function, input_par,
				output_par, parameter_list, dataType, primitiveDatatype, constructedDatatype,
				remaining_list, stmts, typeDefinitions, typeDefinition, fieldDefinitions,
				fieldDefinition, moreFields, declarations, declaration, global_or_not,
				otherStmts, stmt, assignmentStmt, singleOrRecId, new_24, funCallStmt,
				outputParameters, inputParameters, iterativeStmt, conditionalStmt, elsePart,
				ioStmt, allVar, arithmeticExpression, expPrime, term,
				termPrime, factor, highPrecedenceOperators, lowPrecedenceOperators, all,
				temp, booleanExpression, var, logicalOp,
				relationalOp, returnStmt, optionalReturn, idList, more_ids};
typedef enum nonTerm nonTerminal;
typedef tokenType terminal;

char* Terminals[]={"TK_ASSIGNOP", "TK_COMMENT",  "TK_FIELDID",  "TK_ID",  "TK_NUM",
			"TK_RNUM",  "TK_FUNID",  "TK_RECORDID",  "TK_WITH",  "TK_PARAMETERS",
			"TK_END",  "TK_WHILE",  "TK_TYPE",  "TK_MAIN",  "TK_GLOBAL",
  			"TK_PARAMETER",  "TK_LIST",  "TK_SQL",  "TK_SQR",  "TK_INPUT",
  			"TK_OUTPUT",  "TK_INT",  "TK_REAL",  "TK_COMMA",  "TK_SEM",
  			"TK_COLON", "TK_DOT",  "TK_ENDWHILE",  "TK_OP",  "TK_CL",
  			"TK_IF",  "TK_THEN",  "TK_ENDIF",  "TK_READ",  "TK_WRITE",
  			"TK_RETURN",  "TK_PLUS",  "TK_MINUS",  "TK_MUL",  "TK_DIV",
  			"TK_CALL", "TK_RECORD",  "TK_ENDRECORD",  "TK_ELSE",  "TK_AND",
  			"TK_OR",  "TK_NOT",  "TK_LT",  "TK_LE",  "TK_EQ",
  			"TK_GT",  "TK_GE",  "TK_NE","DOLLAR","syn",
  			"TK_ERROR", "TK_EOF", "eps"};

char* NonTerminals[]={ "program","mainFunction", "otherFunctions", "function", "input_par",
				"output_par", "parameter_list", "dataType", "primitiveDatatype", "constructedDatatype",
				"remaining_list", "stmts", "typeDefinitions", "typeDefinition", "fieldDefinitions",
				"fieldDefinition", "moreFields", "declarations", "declaration", "global_or_not",
				"otherStmts", "stmt", "assignmentStmt", "singleOrRecId","new_24",
				"funCallStmt", "outputParameters", "inputParameters", "iterativeStmt", "conditionalStmt",
				"elsePart",	"ioStmt", "allVar", "arithmeticExpression", "expPrime",
				"term","termPrime", "factor", "highPrecedenceOperators", "lowPrecedenceOperators",
				"all", "temp", "booleanExpression", "var", "logicalOp",
				"relationalOp", "returnStmt", "optionalReturn", "idList", "more_ids"};



union element{
	terminal t;
	nonTerminal nt;
};
typedef union element element;

struct symbol
{
	element e;
	int TorNT;
};
typedef struct symbol symbol;

struct node{
	symbol sym;
	struct node* nextSym;
};
typedef struct node Node;

struct right{
	Node* rule;
//	int ruleNo;
	struct right* nextRule;
};
typedef struct right Right;

struct rhead{
//	nonTerm nonTermName;
	symbol sym;
	Right* head;
	int count;
};
typedef struct rhead rHead;
typedef rHead* Grammar;



 //FirstAndFollowSet DataStructure
struct row
{
  Node* headFirst;
  Node* headFollow;
};
typedef struct row row;
typedef row* FirstAndFollow;

//parseTable structure
//typedef Node* table;
Node* T[no_NT][no_T];
Node* synNode;
FirstAndFollow f;
