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
* File name: Scanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS
#define SCANNER_MAX_LEX 128

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Step2Reader.h"
#endif

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
q_int line;								/* Current line number of the source code */
extern q_int errorNumber;				/* Defined in platy_st.c - run-time error number */

extern q_int stateType[NUM_STATES];
extern q_str keywordTable[KWT_SIZE];

extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern q_int transitionTable[NUM_STATES][CHAR_CLASSES];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
 /* TO_DO: Follow the standard and adjust datatypes */

q_int startScanner(BufferPointer psc_buf) {
	/* TO_DO: Start histogram */
	q_int i = 0;
	for (i=0; i<NUM_TOKENS;i++)
		scData.scanHistogram[i] = 0;
	/* Basic scanner initialization */
	/* in case the buffer has been read previously  */
	readerRecover(psc_buf);
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states 
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(q_void) {

	Token currentToken = { 0 };
	q_int c;
	q_int state = 0;
	q_int lexStart;
	q_int lexEnd;
	q_int lexLength;
	q_int i;
	q_char nextChar;

	q_str lexeme;
	lexeme = (q_str)malloc(VID_LEN * sizeof(q_char));
	if (!lexeme)
		return currentToken;
	lexeme[0] = EOS_CHR;

	while (1) {
		c = readerGetChar(sourceBuffer);

		if (c < 0 || c >= NCHAR)
			return currentToken;

		switch (c) {

			/* keep all your original cases exactly as you had them */
			case SPC_CHR:
			case TAB_CHR:
			  continue; // Ignore spaces and tabs
			case NWL_CHR:
				line++;
				continue;

			case SQT_CHR:
				currentToken.code = EOS_T;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			case LPR_CHR:
				currentToken.code = LPR_T;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			case RPR_CHR:
				currentToken.code = RPR_T;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			case LBR_CHR:
				currentToken.code = LBR_T;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			case RBR_CHR:
				currentToken.code = RBR_T;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;

			case EOS_CHR:
				currentToken.code = SEOF_T;
				scData.scanHistogram[currentToken.code]++;
				currentToken.attribute.seofType = SEOF_0;
				return currentToken;

			case (q_char)EOF_CHR:
				currentToken.code = SEOF_T;
				scData.scanHistogram[currentToken.code]++;
				currentToken.attribute.seofType = SEOF_255;
				return currentToken;

			case AMP_CHR:  // '&' detected
				lexStart = readerGetPosRead(sourceBuffer) - 1;
				q_char c2;
				lexemeBuffer = readerCreate(VID_LEN);
				if (!lexemeBuffer) {
						fprintf(stderr, "Scanner error: Failed to create lexeme buffer\n");
						exit(1);
				}
				readerAddChar(lexemeBuffer, AMP_CHR);  // Add '&'

				while (1) {
						c2 = readerGetChar(sourceBuffer);
						if ((isalnum(c2) || c2 == '_') && c2 != SQT_CHR) {
								readerAddChar(lexemeBuffer, c2);
						} else {
								readerRetract(sourceBuffer); // retract non-id char
								break;
						}
				}

				readerAddChar(lexemeBuffer, READER_TERMINATOR);
				lexeme = readerGetContent(lexemeBuffer, 0);

				currentToken.code = VAR_T;
				strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
				currentToken.attribute.idLexeme[VID_LEN] = '\0';
				scData.scanHistogram[currentToken.code]++;
				readerFree(lexemeBuffer);
				return currentToken;

			case LTN_CHR: { 	//'<'
					nextChar = readerGetChar(sourceBuffer);
					if (nextChar == GTN_CHR) {  // '>'
							currentToken.code = COMB_T;  // Define this token in your token codes enum
							scData.scanHistogram[currentToken.code]++;
							return currentToken;
					} else if (nextChar == EQL_CHR) {  // '<='
							currentToken.code = LTE_T;  // Define this token in your token codes enum
							scData.scanHistogram[currentToken.code]++;
							return currentToken;
					} else {
							// Not followed by '>', retract nextChar and return normal '<'
							readerRetract(sourceBuffer);
							currentToken.code = LTN_T; // Your existing token code for '<'
							scData.scanHistogram[currentToken.code]++;
							return currentToken;
					}
				}

			case GTN_CHR: { 	// '>'
				nextChar = readerGetChar(sourceBuffer);
				if (nextChar == EQL_CHR) {  // '>='
					currentToken.code = GTE_T;  
					scData.scanHistogram[currentToken.code]++;
					return currentToken;
				} else {
					readerRetract(sourceBuffer);
					currentToken.code = GTN_T; // Your existing token code for '>'
					scData.scanHistogram[currentToken.code]++;
					return currentToken;
				}
			}

			case EQL_CHR:   // '='
				nextChar = readerGetChar(sourceBuffer);
				if (nextChar == EQL_CHR) {  // '=='
					currentToken.code = EQL_T;
					currentToken.attribute.relationalOperator = OP_EQ;
					scData.scanHistogram[currentToken.code]++;
					return currentToken;
				} else {
					readerRetract(sourceBuffer); // retract nextChar
					currentToken.code = ASSIGN_T; // '='
					scData.scanHistogram[currentToken.code]++;
					return currentToken;
				}

			case NOT_CHR: 
				nextChar = readerGetChar(sourceBuffer);
				if (nextChar == EQL_CHR) {  // '!='
					currentToken.code = NEQ_T;
					currentToken.attribute.relationalOperator = OP_NE;
					scData.scanHistogram[currentToken.code]++;
					return currentToken;
				} else {
					readerRetract(sourceBuffer); // retract nextChar2
					currentToken.code = ERR_T; // '!'
					strncpy(currentToken.attribute.errLexeme, "Invalid token: !", ERR_LEN);
					currentToken.attribute.errLexeme[ERR_LEN] = '\0';
					scData.scanHistogram[currentToken.code]++;
					return currentToken;
				}

			case PLS_CHR:   // '+'
				currentToken.code = ART_T;
				currentToken.attribute.arithmeticOperator = OP_ADD;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;

			case MIN_CHR:   // '-'
				currentToken.code = ART_T;
				currentToken.attribute.arithmeticOperator = OP_SUB;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;

			case MUL_CHR:   // '*'
				currentToken.code = ART_T;
				currentToken.attribute.arithmeticOperator = OP_MUL;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;

			case DIV_CHR:   // '/'
				currentToken.code = ART_T;
				currentToken.attribute.arithmeticOperator = OP_DIV;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;

			case TIL_CHR:  // comment start '~'
				lexStart = readerGetPosRead(sourceBuffer) - 1; // mark start at '~'
				q_int foundClosing = 0;
				q_char ch;

				while (1) {
						ch = readerGetChar(sourceBuffer);
						if (ch == (q_char)EOF_CHR || ch == EOS_CHR) {
								fprintf(stderr, "Scanner error: Unterminated comment\n");
								currentToken.code = ERR_T;
								strncpy(currentToken.attribute.errLexeme, "Unterminated comment", ERR_LEN);
								currentToken.attribute.errLexeme[ERR_LEN] = '\0';
								return currentToken;
						}
						if (ch == TIL_CHR) {
								foundClosing = 1;
								break; // end of comment
						}
				}

				if (!foundClosing) {
						fprintf(stderr, "Scanner error: Unterminated comment\n");
						currentToken.code = ERR_T;
						strncpy(currentToken.attribute.errLexeme, "Unterminated comment", ERR_LEN);
						currentToken.attribute.errLexeme[ERR_LEN] = '\0';
						return currentToken;
				}

				lexEnd = readerGetPosRead(sourceBuffer);  // position after closing tilde
				lexLength = lexEnd - lexStart;

				lexemeBuffer = readerCreate(lexLength + 2);
				if (!lexemeBuffer) {
						fprintf(stderr, "Scanner error: Failed to create lexeme buffer\n");
						exit(1);
				}

				// Restore reading position to lexStart and re-read comment lexeme
				readerSetMark(sourceBuffer, lexStart);
				readerRestore(sourceBuffer);

				for (i = 0; i < lexLength; i++) {
						q_char ch2 = readerGetChar(sourceBuffer);
						readerAddChar(lexemeBuffer, ch2);
				}
				readerAddChar(lexemeBuffer, READER_TERMINATOR);

				lexeme = readerGetContent(lexemeBuffer, 0);
				if (!lexeme) {
						fprintf(stderr, "Scanner error: Lexeme is NULL\n");
						currentToken.code = ERR_T;
						strncpy(currentToken.attribute.errLexeme, "Null lexeme", ERR_LEN);
						currentToken.attribute.errLexeme[ERR_LEN] = '\0';
						readerFree(lexemeBuffer);
						return currentToken;
				}

				currentToken = funcCMT(lexeme);

				// **Now, set buffer reading position after the comment lexeme**
				// lexEnd points to position after closing tilde
				readerSetMark(sourceBuffer, lexEnd);

				readerFree(lexemeBuffer);

				return currentToken;

			case CMA_CHR:
				currentToken.code = COMMA_T;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;

			case QUT_CHR:
				currentToken.code = STR_T;
				scData.scanHistogram[currentToken.code]++;
				lexemeBuffer = readerCreate(SCANNER_MAX_LEX);
				if (!lexemeBuffer) {
						fprintf(stderr, "Scanner error: Failed to create lexeme buffer\n");
						exit(1);
				}

				// Collect characters up to the apostrophe (') which ends the string
				while (1) {
					c = readerGetChar(sourceBuffer);

					if (c == (q_char)EOF_CHR || c == EOS_CHR) {
							fprintf(stderr, "Scanner error: Unterminated string literal\n");
							currentToken.code = ERR_T;
							strncpy(currentToken.attribute.errLexeme, "Unterminated string literal", ERR_LEN);
							currentToken.attribute.errLexeme[ERR_LEN] = '\0';
							readerFree(lexemeBuffer);
							return currentToken;
					}

					if (c == QUT_CHR) {
							break; // End of string
					}

					if (c == '\\') {
							// Escape sequence
							q_char next = readerGetChar(sourceBuffer);
							switch (next) {
									case 'n':
											readerAddChar(lexemeBuffer, '\n');
											break;
									case 't':
											readerAddChar(lexemeBuffer, '\t');
											break;
									case '"':
											readerAddChar(lexemeBuffer, '\"');
											break;
									case '\'':
											readerAddChar(lexemeBuffer, '\'');
											break;
									case '\\':
											readerAddChar(lexemeBuffer, '\\');
											break;
									default:
                    readerAddChar(stringLiteralTable, lexeme[i]);
							}
					} else {
							readerAddChar(lexemeBuffer, c);
					}
				}

				readerAddChar(lexemeBuffer, READER_TERMINATOR);
				lexeme = readerGetContent(lexemeBuffer, 0);
				if (!lexeme) {
						fprintf(stderr, "Scanner error: Lexeme is NULL\n");
						currentToken.code = ERR_T;
						strncpy(currentToken.attribute.errLexeme, "Null lexeme", ERR_LEN);
						currentToken.attribute.errLexeme[ERR_LEN] = '\0';
						readerFree(lexemeBuffer);
						return currentToken;
				}

				currentToken = funcSL(lexeme); 
				readerFree(lexemeBuffer);
				return currentToken;

			default:
				state = nextState(state, c);
				lexStart = readerGetPosRead(sourceBuffer) - 1;
				readerSetMark(sourceBuffer, lexStart);

				while (stateType[state] == NOFS) {
						c = readerGetChar(sourceBuffer);

						// Stop scanning if we hit a delimiter or the single quote
						if (c == SPC_CHR || c == TAB_CHR || c == NWL_CHR || c == EOS_CHR ||
								c == (q_char)EOF_CHR || c == SQT_CHR || c == CMA_CHR ||
								c == LPR_CHR || c == RPR_CHR || c == LBR_CHR || c == RBR_CHR ||
								c == LTN_CHR || c == GTN_CHR || c == EQL_CHR ||
								c == PLS_CHR || c == MIN_CHR || c == MUL_CHR || c == DIV_CHR) {
								
								q_int currentPos = readerGetPosRead(sourceBuffer);
								if (currentPos > 0) {
										readerRetract(sourceBuffer);  // retract so the stopping char remains unconsumed
								} else {
										printf("ERROR... Skipping retract.\n");
								}
								break;
						}

						state = nextState(state, c);
				}

				q_int finalState = state;

				if (stateType[finalState] == FSWR) {
						readerRetract(sourceBuffer);
				}

				lexEnd = readerGetPosRead(sourceBuffer);
				lexLength = lexEnd - lexStart;

				if (lexLength <= 0 || lexLength > 100) {
						fprintf(stderr, "Scanner error: Invalid lexeme length = %d\n", lexLength);
						currentToken.code = ERR_T;
						strncpy(currentToken.attribute.errLexeme, "Invalid length", ERR_LEN);
						currentToken.attribute.errLexeme[ERR_LEN] = '\0';
						return currentToken;
				}

				// Create lexeme buffer
				lexemeBuffer = readerCreate((q_int)lexLength + 2);
				if (!lexemeBuffer) {
						fprintf(stderr, "Scanner error: Failed to create lexeme buffer\n");
						exit(1);
				}

				// Fill lexeme buffer from source buffer
				readerRestore(sourceBuffer);

				for (q_int i = 0; i < lexLength; i++) {
						q_char ch = readerGetChar(sourceBuffer);
						readerAddChar(lexemeBuffer, ch);
				}
				readerAddChar(lexemeBuffer, READER_TERMINATOR);

				// Get the lexeme string
				lexeme = readerGetContent(lexemeBuffer, 0);
				if (!lexeme) {
						fprintf(stderr, "Scanner error: Lexeme is NULL\n");
						currentToken.code = ERR_T;
						strncpy(currentToken.attribute.errLexeme, "Null lexeme", ERR_LEN);
						currentToken.attribute.errLexeme[ERR_LEN] = '\0';
						return currentToken;
				}

				// Validate final state function pointer
				if (finalState < 0 || finalState >= NUM_STATES || finalStateTable[finalState] == NULL) {
						fprintf(stderr, "Scanner error: Invalid final state %d\n", finalState);
						currentToken.code = ERR_T;
						strncpy(currentToken.attribute.errLexeme, "Invalid final state", ERR_LEN);
						currentToken.attribute.errLexeme[ERR_LEN] = '\0';
						return currentToken;
				}

				// Call the final state function to get the token
				currentToken = (*finalStateTable[finalState])(lexeme);

				readerRestore(lexemeBuffer);
				return currentToken;	
		}

	}
}

/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */
 /* TO_DO: Just change the datatypes */

q_int nextState(q_int state, q_char c) {
 	q_int col = nextClass(c);
  q_int next = transitionTable[state][col];
  assert(next != FS);
  return next;
}


/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */
/* TO_DO: Use your column configuration */

/* Adjust the logic to return next column in TT */
/*    [A-z],[0-9],    _,    &,   \', SEOF,    #, other
	   L(0), D(1), U(2), M(3), Q(4), E(5), C(6),  O(7) */

q_int nextClass(q_char c) {
	q_int val = -1;

	switch (c) {
	case SPC_CHR:     // space
	case TAB_CHR:     // tab
		val = 999;      // special signal to ignore whitespace
		break;

	case AMP_CHR:     // &
		val = 2;
		break;
	case SQT_CHR:     // '
		val = 3;
		break;
	case TIL_CHR:     // ~
	case NWL_CHR:     // newline
		val = 5;
		break;
	case EOS_CHR:
	case (q_char) EOF_CHR:
		val = 4;
		break;
	case PRD_CHR:     // .
		val = 7;
		break;
	case LTN_CHR:     // <
		val = 8;
		break;
	case GTN_CHR:     // >
		val = 9;
		break;
	case EQL_CHR:     // =
		val = 10;
		break;
	case NOT_CHR:     // !
		val = 11;
		break;
	case PLS_CHR:     // +, -, *, /
	case MIN_CHR:
	case MUL_CHR:
	case DIV_CHR:
		val = 12;
		break;
	default:
		if (isalpha(c))
			val = 0;
		else if (isdigit(c))
			val = 1;
		else
			val = 6; // other
	}

	if (val == -1)
		val = 6; // defensive fallback

	return val;
}



/*
 ************************************************************
 * Acceptance State Function COM
 *		Function responsible to identify COM (comments).
 ***********************************************************
 */
/* TO_DO: Adjust the function for IL */

Token funcCMT(q_str lexeme) {
    Token token = {0};
    token.code = CMT_T;
		for (q_int i = 0; lexeme[i] != EOS_CHR; i++) {
			if (lexeme[i] == NWL_CHR) { 
				line++;
			}
    }
    scData.scanHistogram[token.code]++;
    return token;
}

/*
 ************************************************************
 * Acceptance State Function IL
 *	- Function responsible to identify IL (integer literals).
 * - It is necessary respect the limit (ex: 2-byte integer in C).
 * - In the case of larger lexemes, error shoul be returned.
 * - Only first ERR_LEN characters are accepted and eventually,
 *   additional three dots (...) should be put in the output.
 ***********************************************************
 */
/* TO_DO: Adjust the function for IL */

Token funcIL(q_str lexeme) {
    Token currentToken = {0};
    long long val;
    int chars_read = 0;
    int result;

    result = sscanf(lexeme, "%lld%n", &val, &chars_read);

    if (result != 1 || chars_read == 0 || (size_t)chars_read != strlen(lexeme)) {
        currentToken.code = ERR_T;
        snprintf(currentToken.attribute.errLexeme, ERR_LEN, "Invalid integer literal format: '%s'", lexeme);
        currentToken.attribute.errLexeme[ERR_LEN] = EOS_CHR;
        return currentToken;
    }

    if (val > INT_MAX || val < INT_MIN) {
        currentToken.code = ERR_T;
        snprintf(currentToken.attribute.errLexeme, ERR_LEN, "Integer literal out of range for q_int: '%s'", lexeme);
        currentToken.attribute.errLexeme[ERR_LEN] = EOS_CHR;
        return currentToken;
    }

    currentToken.code = INL_T;
    currentToken.attribute.intValue = (q_int)val;
    return currentToken;
}

/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */
/* TO_DO: Adjust the function for ID */

Token funcID(q_str lexeme) {
	Token currentToken = {0};

	// Must start with letter or underscore
    if (!isalpha(lexeme[0]) && lexeme[0] != '_') {
        currentToken.code = ERR_T;
        snprintf(currentToken.attribute.errLexeme, ERR_LEN, "Invalid ID start: '%s'", lexeme);
        currentToken.attribute.errLexeme[ERR_LEN] = '\0';
        return currentToken;
    }

		q_char c2;
		while (1) {
				c2 = readerGetChar(sourceBuffer);
				if ((isalnum(c2) || c2 == '_') && c2 != SQT_CHR) {
						readerAddChar(lexemeBuffer, c2);
				} else {
						readerRetract(sourceBuffer); // retract non-id char
						break;
				}
		}

		readerAddChar(lexemeBuffer, READER_TERMINATOR);
		lexeme = readerGetContent(lexemeBuffer, 0);

    // Default: identifier
    currentToken.code = ID_T;
    strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
    currentToken.attribute.idLexeme[VID_LEN] = '\0';

    return currentToken;
}



/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table 
 *   (stringLiteralTable). You need to include the literals in 
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */
/* TO_DO: Adjust the function for SL */

Token funcSL(q_str lexeme) {
    Token t;
    t.code = STR_T;

    // Get the current offset in the string literal table
		q_int offset = readerGetChar(stringLiteralTable);

    // Add the string characters to the table
    for (int i = 0; lexeme[i] != '\0'; i++) {
        if (!readerAddChar(stringLiteralTable, lexeme[i])) {
            fprintf(stderr, "Scanner error: Failed to add character to stringLiteralTable\n");
            t.code = ERR_T;
            strncpy(t.attribute.errLexeme, lexeme, ERR_LEN);
            t.attribute.errLexeme[ERR_LEN] = '\0';
            return t;
        }
    }

		// Append a newline character
    if (!readerAddChar(stringLiteralTable, '\n')) {
        fprintf(stderr, "Scanner error: Failed to add newline to stringLiteralTable\n");
        t.code = ERR_T;
        strncpy(t.attribute.errLexeme, lexeme, ERR_LEN);
        t.attribute.errLexeme[ERR_LEN] = '\0';
        return t;
    }

    // Add null terminator to end the string
    if (!readerAddChar(stringLiteralTable, READER_TERMINATOR)) {
        fprintf(stderr, "Scanner error: Failed to terminate stringLiteralTable\n");
        t.code = ERR_T;
        strncpy(t.attribute.errLexeme, lexeme, ERR_LEN);
        t.attribute.errLexeme[ERR_LEN] = '\0';
        return t;
    }

    // Set the offset in the token
    t.attribute.contentString = offset;
    return t;
}



/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */
/* TO_DO: Adjust the function for Keywords */

Token funcKEY(q_str lexeme) {
	Token currentToken;
	currentToken.code = KW_T;

	for (int i = 0; i < KWT_SIZE; i++) {
		if (strcmp(lexeme, keywordTable[i]) == 0) {
			currentToken.attribute.keywordIndex = i;
			return currentToken;
		}
	}

	currentToken.code = ID_T;
	if (strlen(lexeme) > VID_LEN)
		lexeme[VID_LEN] = '\0'; 
	strcpy(currentToken.attribute.idLexeme, lexeme);

	return currentToken;
}

/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */
/* TO_DO: Adjust the function for Errors */

Token funcErr(q_str lexeme) {
	Token currentToken = { 0 };
	q_int i = 0, len = (q_int)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = EOS_CHR;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == NWL_CHR)
			line++;
	currentToken.code = ERR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}

/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */
q_void printToken(Token t) {
	extern q_str keywordTable[]; /* link to keyword table */

	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;

	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case EQL_T:
		printf("EQL_T\t\t\n");
		break;
	
	case NEQ_T:
		printf("NEQ_T\t\t\n");
		break;

	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;

	case INL_T:
		printf("INL_T\t\t%d\n", t.attribute.intValue);
		break;

	case FLT_T:
    printf("FLT_T\t\t%f\n", t.attribute.floatValue);
    break;

	case STR_T:
		printf("STR_T\t\t\n");
		break;

	case LPR_T:
		printf("LPR_T\n");
		break;

	case RPR_T:
		printf("RPR_T\n");
		break;

	case LBR_T:
		printf("LBR_T\n");
		break;

	case RBR_T:
		printf("RBR_T\n");
		break;

	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.keywordIndex]);
		break;

	case CMT_T:
		printf("CMT_T\n");
		break;

	case EOS_T:
		printf("EOS_T\n");
		break;

	case ASSIGN_T:
		printf("ASSIGN_T\n");
		break;

	case COMB_T:
		printf("COMB_T\n");
		break;

	case VAR_T:
		printf("VAR_T\t\t%s\n", t.attribute.idLexeme);
		break;

	case LTE_T:
		printf("LTE_T\n");
		break;

	case GTE_T:
		printf("GTE_T\n");
		break;

	case LTN_T:
		printf("LTN_T\n");
		break;

	case GTN_T:
		printf("GTN_T\n");
		break;
	
	case ID_T:
		printf("ID_T\t\t%s\n", t.attribute.idLexeme);
		break;

	case ART_T:
		// Optionally, print which arithmetic operator it is
		switch (t.attribute.arithmeticOperator) {
		case OP_ADD:
			printf("ART_T\t\t+\n");
			break;
		case OP_SUB:
			printf("ART_T\t\t-\n");
			break;
		case OP_MUL:
			printf("ART_T\t\t*\n");
			break;
		case OP_DIV:
			printf("ART_T\t\t/\n");
			break;
		default:
			printf("ART_T\t\tUnknown operator\n");
			break;
		}
		break;

	case COMMA_T:
    printf("COMMA_T\n");
    break;

	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
		break;
	}
}


/*
 ************************************************************
 * The function prints statistics of tokens
 * Param:
 *	- Scanner data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
q_void printScannerData(ScannerData scData) {
	/* Print Scanner statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_TOKENS; cont++) {
		if (scData.scanHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", tokenStrTable[cont], "]=", scData.scanHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}

/*
TO_DO: (If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/

/*
 ************************************************************
 * Acceptance State Function for Ampersand (AMP_T)
 * This function handles the standalone '&' token.
 ***********************************************************
 */

/*
 ************************************************************
 * Function name: funcAmpID
 * Purpose: Handle ampersand-prefixed variables like &name
 * Author: David Lainez
 * History/Version: S25
 ***********************************************************
 */
Token funcAmpID(q_str lexeme) {
    Token token = {0};
    token.code = VAR_T;

    strncpy(token.attribute.idLexeme, lexeme, VID_LEN);
    token.attribute.idLexeme[VID_LEN] = EOS_CHR;

    scData.scanHistogram[token.code]++;
    return token;
}

Token funcAmp(q_str lexeme) {
    Token token = {0};
    token.code = ERR_T;

    strncpy(token.attribute.errLexeme, lexeme, ERR_LEN);
    token.attribute.errLexeme[ERR_LEN] = EOS_CHR;

    scData.scanHistogram[token.code]++;
    return token;
}

Token funcFLT(q_str lexeme) {
    Token t;
    t.code = FLT_T;
    return t;
}