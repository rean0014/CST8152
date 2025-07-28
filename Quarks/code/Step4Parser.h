/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "    @@          ________          @@    �
# ECHO "    @@      /%&@@@@@@*@@ @%\      @@    �
# ECHO "    @@     % (@ %% &&@@  &@@@     @@    �
# ECHO "    @@    @& @@/        \%%  @    @@    �
# ECHO "    @@    @ @*@          @ @ @    @@    �
# ECHO "    @@    & @ @          @|  @    @@    �
# ECHO "    @@    @/ @@          @%  @    @@    �
# ECHO "    @@    @* &@          @ @ @    @@    �
# ECHO "    @@    @| %@          @@@*@    @@    �
# ECHO "    @@    @|  @\ \&&&\  /@@@ @    @@    �
# ECHO "    @@     @**@@@ &\&&&\ @* @/    @@    �
# ECHO "    @@      \%@ %&&@@ \&&&\       @@    �
# ECHO "    @@                  \&&&\     @@    �
# ECHO "    @@         Q U A R K          @@    �
# ECHO "    @@                            @@    �
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "                                         "
# ECHO "[CODER SCRIPT ..........................]"
# ECHO "                                         "
*/


/*
************************************************************
* File name: Parser.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
************************************************************
*/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Reader.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token			lookahead;
extern BufferPointer	stringLiteralTable;
extern q_int			line;
extern Token			tokenizer(q_void);
extern q_str			keywordTable[KWT_SIZE];
static q_int			syntaxErrorNumber = 0;

#define LANG_WRTE		"print&"
#define LANG_READ		"input&"
#define LANG_MAIN		"main&"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,    /* No attribute */
	KW_par,			 /* 0: par */
	KW_nuc,			 /* 1: nuc */
	KW_atom,		 /* 2: atom */
	KW_mol,			 /* 3: mol */
	KW_function,	 /* 4: function */
	KW_if,			 /* 5: if */
	KW_then,		 /* 6: then */
	KW_else,		 /* 7: else */
	KW_while,		 /* 8: while */
	KW_do,			 /* 9: do */
	KW_return,		 /* 10: return */
	KW_qkUp,		 /* 11: qkUp */
	KW_qkDown,		 /* 12: qkDown */
	KW_qkStrange,	 /* 13: qkStrange */
	KW_qkCharm,		 /* 14: qkCharm */
	KW_qkTop,		 /* 15: qkTop */
	KW_qkBottom,		 /* 16: qkBottom */
	KW_qout 		 /* 17: qout */
};

/* TO_DO: Define the number of BNF rules */
#define NUM_BNF_RULES 17

/* Parser */
typedef struct parserData {
	q_int parsHistogram[NUM_BNF_RULES];	/* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
q_int numParserErrors;

/* Scanner data */
ParserData psData;

/* Function definitions */
q_void startParser();
q_void matchToken(q_int, q_int);
q_void syncErrorHandler(q_int);
q_void printError();
q_void printBNFData(ParserData psData);

/* List of BNF statements */
enum BNF_RULES {
	BNF_error,										/*  0: Error token */
	BNF_codeSession,							/*  1 */
	BNF_comment,									/*  2 */
	BNF_dataSession,							/*  3 */
	BNF_optVarListDeclarations,		/*  4 */
	BNF_optionalStatements,				/*  5 */
	BNF_outputStatement,					/*  6 */
	BNF_outputVariableList,				/*  7 */
	BNF_program,									/*  8 */
	BNF_statement,								/*  9 */
	BNF_statements,								/* 10 */
	BNF_statementsPrime,					/* 11 */
	BNF_optParams,								/* 12 */
	BNF_returnStatement,   				/* 13 */
	BNF_assignmentStatement, 			/* 14 */
	BNF_expression,								/* 15 */
	BNF_paramList									/* 16 */
};


/* TO_DO: Define the list of keywords */
static q_str BNFStrTable[NUM_BNF_RULES] = {
	"BNF_error",
	"BNF_codeSession",
	"BNF_comment",
	"BNF_dataSession",
	"BNF_optVarListDeclarations",
	"BNF_optionalStatements",
	"BNF_outputStatement",
	"BNF_outputVariableList",
	"BNF_program",
	"BNF_statement",
	"BNF_statements",
	"BNF_statementsPrime",
	"BNF_optParams",
	"BNF_returnStatement",
	"BNF_assignmentStatement",
	"BNF_expression",
	"BNF_paramList"
};

/* TO_DO: Place ALL non-terminal function declarations */
q_void comment();
q_void optionalStatements();
q_void outputVariableList();
q_void program();
q_void statement();
q_void statements();
q_void statementsPrime();
q_void optParams();
q_void paramList();
q_void returnStatement();
q_void assignmentStatement();
q_void expression();
q_void recursionData();

#endif
