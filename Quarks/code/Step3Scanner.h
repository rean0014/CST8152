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
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define the number of tokens */
#define NUM_TOKENS 24

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	INL_T,		/*  1: Integer literal token */
	STR_T,		/*  2: String literal token */
	LPR_T,		/*  3: Left parenthesis token */
	RPR_T,		/*  4: Right parenthesis token */
	LBR_T,		/*  5: Left brace token */
	RBR_T,		/*  6: Right brace token */
	KW_T,			/*  7: Keyword token */
	ID_T,			/*  8: Identifier token */
	EOS_T,		/*  9: End of statement (single quote) */
	RTE_T,		/* 10: Run-time error token */
	SEOF_T,		/* 11: Source end-of-file token */
	CMT_T,		/* 12: Comment token */
	ASSIGN_T,	/* 13: Assignment operator '=' */
	COMB_T, 	/* 14: Custom quark combination operator '<>' */
	VAR_T, 		/* 15: Variable identifier token */
	LTE_T, 		/* 16: Less than or equal to operator '<=' */
	GTE_T,		/* 17: Greater than or equal to operator '>=' */
	LTN_T,		/* 18: Less than operator '<' */
	GTN_T,		/* 19: Greater than operator '>' */
	ART_T,		/* 20: Arithmetic operator token */
	FLT_T,		/* 21: Floating-point literal token */
	REL_T,		/* 22: Relational operator token*/
	AMP_T,		/* 23: Ampersand token (&) */
};

/* TO_DO: Define the list of keywords */
static q_str tokenStrTable[NUM_TOKENS] = {
	"ERR_T",
	"INL_T",
	"STR_T",
	"LPR_T",
	"RPR_T",
	"LBR_T",
	"RBR_T",
	"KW_T",
	"ID_T",
	"EOS_T",
	"RTE_T",
	"SEOF_T",
	"CMT_T",
	"ASSIGN_T",
	"COMB_T",
	"VAR_T",
	"LTE_T",
	"GTE_T",
	"LTN_T",
	"GTN_T",
	"ART_T",
	"FLT_T",
	"REL_T",
	"AMP_T"
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	q_int codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	q_int intValue;				/* integer literal attribute (value) */
	q_int keywordIndex;			/* keyword index in the keyword table */
	q_int contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	q_float floatValue;				/* floating-point literal attribute (value) */
	q_char idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	q_char errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttributes {
	q_byte flags;			/* Flags information */
	union {
		q_int
 		intValue;				/* Integer value */
		q_float floatValue;			/* Float value */
		q_str stringContent;		/* String value */
	} values;
} IdAttributes;

/* Token declaration */
typedef struct Token {
	q_int code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttributes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	q_int scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;
///////////////////////////////////////////////////////////////////////////////////////////////////////

/* TO_DO: Define lexeme FIXED classes */
/* EOF definitions */
#define EOS_CHR '\0'	// CH00
#define EOF_CHR 0xFF	// CH01
#define EQL_CHR '='		// CH02
#define AMP_CHR '&'		// CH03
#define QUT_CHR '"'	  // CH04
#define MIN_CHR '-'		// CH05
#define TAB_CHR '\t'	// CH06
#define SPC_CHR ' '		// CH07
#define NWL_CHR '\n'	// CH08
#define CMA_CHR ','		// CH09
#define LPR_CHR '('		// CH10
#define RPR_CHR ')'		// CH11
#define LBR_CHR '{'		// CH12
#define RBR_CHR '}'		// CH13
#define SQT_CHR '\''	// CH14
#define LTN_CHR '<'		// CH15
#define GTN_CHR '>'		// CH16
#define PLS_CHR '+'		// CH17
#define DIV_CHR '/'		// CH18
#define TIL_CHR '~'		// CH19
#define MUL_CHR '*'		// CH20
#define PRD_CHR '.'		// CH21
#define NOT_CHR '!'		// CH22

/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */


/* TO_DO: Error states and illegal state */
#define ESNR  8   /* Error state with no retract */
#define ESWR  9   /* Error state with retract */
#define FS    999    /* Illegal state */

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

 /* TO_DO: State transition table definition */
#define NUM_STATES    25
#define CHAR_CLASSES  13

/* TO_DO: Transition table - type of states defined in separate table */
static q_int transitionTable[NUM_STATES][CHAR_CLASSES] = {
/* [A-z], [0-9],  &,   ',   EOF,   ~,   other,   .,   <,    >,    =,    !,   + - * /
      L(0), D(1), M(2), Q(3), E(4), C(5), O(6), P(7),LT(8),GT(9),EQ(10),EX(11),AR(12) */
  {     1,   10,   22,    4, ESWR,    6, ESNR, ESNR,  11,   13,   15,   17,    19},  // S0: Start
  {     1,    1, ESNR,    3, ESWR, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR},  // S1: Identifier/Keyword (Fixed: '&',' not part of ID, use ESNR/FS)
  {    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS,   FS,   FS,    FS},  // S2: ASNR (MVID)
  {    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS,   FS,   FS,    FS},  // S3: ASWR (KEY)
  {     4,    4,    4,    5, ESWR,    4,    4,    4,    4,   4,    4,    4,     4},  // S4: String content (reads any char, single quote goes to S5, EOF is error)
  {    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS,   FS,   FS,    FS},  // S5: ASNR (String literal - after closing quote)
  {     6,    6,    6,    6, ESNR,    7,    6,    6,    6,   6,    6,    6,     6},  // S6: Tilde comment start (reading comment content)
  {    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS,   FS,   FS,    FS},  // S7: ASNR (Comment)
  {    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS,   FS,   FS,    FS},  // S8: ASNR (Err - no retract)
  {    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS,   FS,   FS,    FS},  // S9: ASWR (Err - retract)
  {  FSWR,   10, FSWR, FSWR, FSWR, FSWR, FSWR,   20, FSWR, FSWR, FSWR, FSWR, FSWR},  // S10: Integer / potential Float starter (FSWR on non-digit/dot, . to S20)
  {    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   14,  14,   FS,   FS,    FS},  // S11: '<'
  {    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS,   FS,   FS,    FS},  // S12: (Original Decimal digits state, now essentially handled by S21)
  {    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  14,   FS,   FS,    FS},  // S13: '>'
  {    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS,   FS,   FS,    FS},  // S14: Accept comparison (<=, <>, >=)
  {    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS,   16,   FS,    FS},  // S15: '='
  {    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS,   FS,   FS,    FS},  // S16: '=='
  {    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS,   FS,   18,    FS},  // S17: '!'
  {    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS,   FS,   FS,    FS},  // S18: '!='
  {    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS,   FS,   FS,    FS},  // S19: Arithmetic (+ - * /)
  {  FSWR,   21, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR},  // S20: Float dot state (expects digits for S21. FSWR on others to accept 3.)
  {  FSWR,   21, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR},  // S21: Float digits state (Accepts FLT_T, retracts non-digit)
  { 	 23, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR},  // S22: Ampersand token (&) - expects a letter for variable, others are errors.
  { 	 23, 	 23, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR, FSWR}   // S23: Variable after Ampersand (&var)
};

/* TO_DO: Define list of acceptable states */
static q_int stateType[NUM_STATES] = {
	NOFS, // 00: Start
  NOFS, // 01: Identifier/Keyword
  FSNR, // 02: MVID (funcID)
  FSWR, // 03: KEY (funcKEY)
  NOFS, // 04: String content
  FSNR, // 05: String literal (funcSL)
  NOFS, // 06: Tilde comment start
  FSNR, // 07: Comment (funcCMT)
  FSNR, // 08: Error no retract (funcErr)
  FSWR, // 09: Error with retract (funcErr)
  NOFS, // 10: INL_T / FLT_T start (funcIL)
  NOFS, // 11: '<' seen
  NOFS, // 12: (Unused / Error)
  NOFS, // 13: '>' seen
  FSWR, // 14: Comparison operator (funcID, funcRelational) - Assuming funcID handles combined operators
  NOFS, // 15: '=' seen
  FSWR, // 16: '==' (funcID, funcAssignment)
  NOFS, // 17: '!' seen
  FSWR, // 18: '!=' (funcID, funcRelational)
  FSWR, // 19: Arithmetic (+ - * /) (funcID, funcArithmetic)
  FSNR, // 20: Float dot state (e.g., '3.') (funcIL)
  FSWR, // 21: Float digits state (e.g., '3.5') (funcIL)
  FSWR,  // 22: VAR_T (funcAmpID)
	FSWR  // 23: Ampersand variable (funcAmp)
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
q_int			startScanner(BufferPointer psc_buf);
static q_int	nextClass(q_char c);					/* character class function */
static q_int	nextState(q_int, q_char);		/* state machine function */
q_void	printScannerData(ScannerData scData);
Token	tokenizer(q_void);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(q_str lexeme);

/* Declare accepting states functions */
Token funcSL	(q_str lexeme);
Token funcIL	(q_str lexeme);
Token funcID	(q_str lexeme);
Token funcCMT   (q_str lexeme);
Token funcKEY	(q_str lexeme);
Token funcErr	(q_str lexeme);
Token funcAmpID(q_str lexeme);
Token funcAmp(q_str lexeme);
Token funcFLT(q_str lexeme);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	/*  0 */ NULL,        // Start state (non-accepting)
	/*  1 */ funcID,      // Identifier (valid)
	/*  2 */ funcErr,     // Invalid mixed identifier
	/*  3 */ funcID,      // Identifier that may be a keyword
	/*  4 */ NULL,        // Inside string literal
	/*  5 */ funcSL,      // String literal
	/*  6 */ NULL,        // Inside comment
	/*  7 */ funcCMT,     // Complete comment
	/*  8 */ funcErr,     // Error, no retract
	/*  9 */ funcErr,     // Error, with retract
	/* 10 */ funcIL,      // Integer literal
	/* 11 */ funcKEY,     // '<' or partial relational op
	/* 12 */ funcKEY,     // '>' or partial relational op
	/* 13 */ funcKEY,     // '=' or partial relational op
	/* 14 */ funcID,     // Combined relational op (<=, >=, <>)
	/* 15 */ funcKEY,     // '=' (again, assign)
	/* 16 */ funcKEY,     // '==' exact match
	/* 17 */ funcKEY,     // '!' or partial '!='
	/* 18 */ funcKEY,     // '!='
	/* 19 */ funcKEY,     // Arithmetic operator (+, -, *, /)
	/* 20 */ funcFLT,     // Float part (you don't have funcFLT yet)
	/* 21 */ funcFLT,     // Float complete (same)
	/* 22 */ funcAmp,     // Lone ampersand (&)
	/* 23 */ funcAmpID,   // Ampersand-prefixed identifier (&name)
	/* 24 */ funcFLT			// Float literal
};


/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 17

/* TO_DO: Define the list of keywords */
static q_str keywordTable[KWT_SIZE] = {
	"par",			/* KW01 */
	"nuc",			/* KW02 */
	"atom",			/* KW03 */
	"mol",			/* KW04 */
	"function",		/* KW05 */
	"if",			/* KW06 */
	"then",			/* KW07 */
	"else",			/* KW08 */	
	"while",		/* KW09 */
	"do",			/* KW10 */
	"return",		/* KW11 */
	"qkUp",	/* KW12 */
	"qkDown", /* KW13 */
	"qkStrange", /* KW14 */
	"qkCharm", /* KW15 */
	"qkTop", /* KW16 */
	"qkBottom", /* KW17 */
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT TAB_CHR  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	q_char indentationCharType;
	q_int indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
q_int numScannerErrors;

/* Scanner data */
ScannerData scData;

#endif
