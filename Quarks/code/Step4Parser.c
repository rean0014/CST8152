﻿/*
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
* File name: Parser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

#ifndef PARSER_H_
#include "Step4Parser.h"
#endif

/* Parser data */
extern ParserData psData; /* BNF statistics */

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
/* TO_DO: This is the function to start the parser - check your program definition */

q_void startParser() {
	/* TO_DO: Initialize Parser data */
	q_int i = 0;
	for (i = 0; i < NUM_BNF_RULES; i++) {
		psData.parsHistogram[i] = 0;
	}
	/* Proceed parser */
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}

/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
/* TO_DO: This is the main code for match - check your definition */
q_void matchToken(q_int tokenCode, q_int tokenAttribute) {  //7 	3
	q_int matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType != tokenAttribute) {
			matchFlag = 0;
			break;
		}
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T) {
		return;
	}
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
/* TO_DO: This is the function to handler error - adjust basically datatypes */
q_void syncErrorHandler(q_int syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
/* TO_DO: This is the function to error printing - adjust basically datatypes */

q_void printError() {
  extern q_int numParserErrors;     
  Token t = lookahead;
  printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
  printf("***** Token code:%3d Attribute: ", t.code);
  switch (t.code) {
  case ERR_T:
    printf("*ERROR*: %s\n", t.attribute.errLexeme);
    break;
  case SEOF_T:
    printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
    break;
  case ID_T:
    printf("ID_T:\t\t%s\t\n", t.attribute.idLexeme);
    break;
  case STR_T:
    printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
    break;
  case KW_T:
    printf("KW_T: %s\n", keywordTable[t.attribute.keywordIndex]); 
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
    printf("VAR_T:\t\t%s\n", t.attribute.idLexeme);
    break;
  case ART_T:
    switch (t.attribute.arithmeticOperator) {
    case OP_ADD: printf("ART_T\t\t+\n"); break;
    case OP_SUB: printf("ART_T\t\t-\n"); break;
    case OP_MUL: printf("ART_T\t\t*\n"); break;
    case OP_DIV: printf("ART_T\t\t/\n"); break;
    default: printf("ART_T\t\tUnknown operator\n"); break;
    }
    break;
  case INL_T:
    printf("INL_T\t\t%d\n", t.attribute.intValue);
    break;
  case COMMA_T:
    printf("COMMA_T\n");
    break;
  case FLT_T:
    printf("FLT_T\t\t%f\n", t.attribute.floatValue);
    break;
  case REL_T:
    printf("REL_T\t\t%u\n", t.attribute.relationalOperator);
    break;
  case AMP_T:
    printf("AMP_T\n");
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
  default:
    printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
    numParserErrors++; 
  }
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> main& { <opt_statements> }
 * FIRST(<program>)= {CMT_T, ID_T (main&), SEOF_T}.
 ***********************************************************
 */
q_void program() {
	psData.parsHistogram[BNF_program]++;

	while (lookahead.code != SEOF_T) {
		switch (lookahead.code) {
		case CMT_T:
			comment();
			break;

		case KW_T:
			if (lookahead.attribute.codeType == KW_function) {
				matchToken(KW_T, KW_function); // 'function'
				matchToken(ID_T, NO_ATTR);     // function name
				matchToken(LPR_T, NO_ATTR);    // (
				optParams();
				matchToken(RPR_T, NO_ATTR);    // )
				matchToken(LBR_T, NO_ATTR);    // {
				optionalStatements();          // function body
				matchToken(RBR_T, NO_ATTR);    // }
			} else {
				matchToken(KW_T, lookahead.attribute.codeType); // e.g., 'par'
				matchToken(ID_T, NO_ATTR); // identifier
				matchToken(ASSIGN_T, NO_ATTR); // '='
				expression(); // statements
			}
			break;
		
		case VAR_T:
			matchToken(VAR_T, NO_ATTR); // variable declaration
			matchToken(ASSIGN_T, NO_ATTR); // '='
			expression(); // statements
			break;

		// case ID_T:
		// 	if (strncmp(lookahead.attribute.idLexeme, LANG_MAIN, 5) == 0) {
		// 		matchToken(ID_T, NO_ATTR);
		// 		matchToken(LPR_T, NO_ATTR);
		// 		optParams();
		// 		matchToken(RPR_T, NO_ATTR);
		// 		matchToken(LBR_T, NO_ATTR);
		// 		dataSession();
		// 		codeSession();
		// 		matchToken(RBR_T, NO_ATTR);
		// 	} else {
		// 		printError();
		// 		lookahead = tokenizer(); // recover from bad ID_T
		// 	}
		// 	break;

		default:
			printError();
			lookahead = tokenizer(); // consume bad token
			break;
		}
	}

	// At this point, lookahead.code == SEOF_T
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}


/*
 ************************************************************
 * comment
 * BNF: comment
 * FIRST(<comment>)= {CMT_T}.
 ***********************************************************
 */
q_void comment() {
	psData.parsHistogram[BNF_comment]++;
	matchToken(CMT_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
}

/*
 ************************************************************
 * optParams
 * BNF: <optParams> -> <paramList> | e
 * FIRST(<optParams>) = { e, KW_T (KW_int), KW_T (KW_real), KW_T (KW_string)}.
 ***********************************************************
 */
q_void optParams() {
    psData.parsHistogram[BNF_expression]++;

    // Handle zero or more parameters
    while (lookahead.code == KW_T &&
           (lookahead.attribute.codeType == KW_par ||
            lookahead.attribute.codeType == KW_nuc ||
            lookahead.attribute.codeType == KW_atom ||
            lookahead.attribute.codeType == KW_mol)) {

        matchToken(KW_T, lookahead.attribute.codeType);  // e.g., 'par', 'mol'

        if (lookahead.code == ID_T) {
            matchToken(ID_T, NO_ATTR);  // e.g., &thing1
        } else {
            printError();  // Unexpected token after 'par'
            return;
        }

        if (lookahead.code == COMMA_T) {
            matchToken(COMMA_T, NO_ATTR);  // More parameters coming
        } else {
            break;  // No comma, so end of parameter list
        }
    }

    printf("%s%s\n", STR_LANGNAME, ": Optional parameters parsed");
}


/*
 ************************************************************
 * paramList
 * BNF: <paramList> -> <opt_varlist_declarations>
 * FIRST(<paramList>) = { KW_T (KW_int), KW_T (KW_real), KW_T (KW_string)}.
 ***********************************************************
 */
q_void paramList() {
	matchToken(VAR_T, NO_ATTR);

	while (lookahead.code == COMMA_T) {
		matchToken(COMMA_T, NO_ATTR);
		matchToken(VAR_T, NO_ATTR);
	}
}


/*
 ************************************************************
 * dataSession
 * BNF: <dataSession> -> data { <opt_varlist_declarations> }
 * FIRST(<program>)= {KW_T (KW_data)}.
 ***********************************************************
 */
// q_void dataSession() {
// 	psData.parsHistogram[BNF_dataSession]++;
// 	switch (lookahead.code) {
// 	case CMT_T:
// 		comment();
// 	default:
// 		matchToken(KW_T, KW_nuc);
// 		matchToken(LBR_T, NO_ATTR);
// 		optVarListDeclarations();
// 		matchToken(RBR_T, NO_ATTR);
// 		printf("%s%s\n", STR_LANGNAME, ": Data Session parsed");
// 	}
// }

/*
 ************************************************************
 * Optional Var List Declarations
 * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
 * FIRST(<opt_varlist_declarations>) = { e, KW_T (KW_int), KW_T (KW_real), KW_T (KW_string)}.
 ***********************************************************
 */
q_void optVarListDeclarations() {
	psData.parsHistogram[BNF_optVarListDeclarations]++;
	switch (lookahead.code) {
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}

/*
 ************************************************************
 * codeSession statement
 * BNF: <codeSession> -> code { <opt_statements> }
 * FIRST(<codeSession>)= {KW_T (KW_code)}.
 ***********************************************************
 */
// q_void codeSession() {
// 	psData.parsHistogram[BNF_codeSession]++;
// 	switch (lookahead.code) {
// 	case CMT_T:
// 		comment();
// 	default:
// 		matchToken(KW_T, KW_function);
// 		matchToken(LBR_T, NO_ATTR);
// 		optionalStatements();
// 		matchToken(RBR_T, NO_ATTR);
// 		printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
// 	}
// }

/* TO_DO: Continue the development (all non-terminal functions) */

/*
 ************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *				KW_T(KW_while), ID_T(print&), ID_T(input&) }
 ***********************************************************
 */
q_void optionalStatements() {
	psData.parsHistogram[BNF_optionalStatements]++;
	switch (lookahead.code) {
	case CMT_T:
		comment();
		// fall through to allow statements after comment
		break;
	case KW_T:
		// Start statements if keyword is one that begins a statement
		switch (lookahead.attribute.codeType) {
		case KW_if:
		case KW_while:
		case KW_return:
			statements();
			break;
		default:
			; // no statement start
		}
		break;
	case VAR_T:
		// Variables can start statements like assignments
		statements();
		break;
	default:
		; // empty - no statements
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *		KW_T(KW_while), ID_T(input&), ID_T(print&) }
 ***********************************************************
 */
q_void statements() {
	psData.parsHistogram[BNF_statements]++;
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime> -> <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T, 
 *		KW_T(KW_if), KW_T(KW_while), ID_T(input&), ID_T(print&) }
 ***********************************************************
 */
q_void statementsPrime() {
    psData.parsHistogram[BNF_statementsPrime]++;
    switch (lookahead.code) {
        case ID_T:
        case KW_T:
            if (lookahead.code == KW_T && 
                (lookahead.attribute.codeType == KW_if || lookahead.attribute.codeType == KW_while)) {
                statements();
                break;
            }
            else if (lookahead.code == ID_T) {
                statements();
                break;
            }
            // else fallthrough to default (empty)
        default:
            ; // epsilon (do nothing)
    }
}

/*
 ************************************************************
 * Single statement
 * BNF: <statement> -> <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if), KW_T(KW_while),
 *			ID_T(input&), ID_T(print&) }
 ***********************************************************
 */
q_void statement() {
    psData.parsHistogram[BNF_statement]++;
    switch (lookahead.code) {
    case KW_T:
        if (lookahead.attribute.codeType == KW_return) {
						matchToken(KW_T, KW_return);
            returnStatement();
        } else {
            printError();
        }
        break;

    case ID_T:
				matchToken(ID_T, NO_ATTR);
        assignmentStatement();
        break;

		case VAR_T:
				matchToken(VAR_T, NO_ATTR);
				assignmentStatement();
				break;

    default:
        printError();
        break;
    }

    printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 ************************************************************
 * Output Statement
 * BNF: <output statement> -> print& (<output statementPrime>);
 * FIRST(<output statement>) = { ID_T(print&) }
 ***********************************************************
 */
// q_void outputStatement() {
// 	psData.parsHistogram[BNF_outputStatement]++;
// 	matchToken(ID_T, NO_ATTR);
// 	matchToken(LPR_T, NO_ATTR);
// 	outputVariableList();
// 	matchToken(RPR_T, NO_ATTR);
// 	matchToken(EOS_T, NO_ATTR);
// 	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
// }

/*
 ************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<opt_variable_list>) = { IVID_T, FVID_T, SVID_T, ϵ }
 ***********************************************************
 */
// q_void outputVariableList() {
// 	psData.parsHistogram[BNF_outputVariableList]++;
// 	switch (lookahead.code) {
// 	case STR_T:
// 		matchToken(STR_T, NO_ATTR);
// 		break;
// 	default:
// 		;
// 	}
// 	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
// }

/*
 ************************************************************
 * The function prints statistics of BNF rules
 * Param:
 *	- Parser data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */

q_void printBNFData(ParserData psData) {
	/* Print Parser statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	q_int cont;
	for (cont = 0; cont < NUM_BNF_RULES; cont++) {
		if (psData.parsHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", BNFStrTable[cont], "]=", psData.parsHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}

/*
 ************************************************************
 * Return Statement
 * BNF: <returnStatement> → return <expression>';
 ************************************************************
 */
q_void returnStatement() {
	psData.parsHistogram[BNF_returnStatement]++;
	q_int loop = 1;

	switch (lookahead.code) {
		case VAR_T:
		case INL_T:
			matchToken(lookahead.code, lookahead.attribute.codeType);
			expression(); // Parse the expression
			break;

		default:
			printError();
			loop = 0; // Exit loop on error
			return;
	}
	printf("%s%s\n", STR_LANGNAME, ": Return statement parsed");
}

/*
 ************************************************************
 * Assignment Statement
 * BNF: <assignmentStatement> → ID_T = <expression> ';
 ************************************************************
 */
q_void assignmentStatement() {
	psData.parsHistogram[BNF_assignmentStatement]++;
	matchToken(KW_T, KW_par);          
	matchToken(ID_T, NO_ATTR);
	matchToken(ASSIGN_T, NO_ATTR);
	expression();                     
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Assignment statement parsed");
}

/*
 ************************************************************
 * Expression
 * BNF: <expression> → VAR_T [ART_T VAR_T]?
 ************************************************************
 */
q_void expression() {
	psData.parsHistogram[BNF_expression]++;
	q_int loop = 1;

	while (loop)
	{
		switch (lookahead.code) {
			case KW_T:
				switch (lookahead.attribute.codeType)
				{
				case KW_qkBottom:
				case KW_qkUp:
				case KW_qkDown:
				case KW_qkTop:
				case KW_qkStrange:
				case KW_qkCharm:
					break;
				
				default:
					printError();
					loop = 0; // End of expression
					break;
				}

			case VAR_T:
			case INL_T:
			case FLT_T:
				matchToken(lookahead.code, lookahead.attribute.codeType);
				break;

			case ART_T:  
				switch (lookahead.attribute.arithmeticOperator) {
					case OP_ADD:
					case OP_SUB:
					case OP_MUL:
					case OP_DIV:
						matchToken(ART_T, lookahead.attribute.arithmeticOperator);
						if (lookahead.code == EOS_T) {
							printError();
							loop = 0; // End of expression
							return;
						}
						break;
					default:
						printError();
						loop = 0; // End of expression
						return;
				}
				break;  

			case COMB_T:      
				matchToken(COMB_T, NO_ATTR);
				if (lookahead.code == EOS_T) {
					printError();
					loop = 0; // End of expression
					return;
				}
				break;

			case LPR_T:      
				matchToken(LPR_T, NO_ATTR);
				if ((lookahead.code == EOS_T) || (lookahead.code == RPR_T)) {
					printError();
					loop = 0; // End of expression
					return;
				}
				expression(); // Recursive call for nested expressions
				matchToken(RPR_T, NO_ATTR);
				break;

			case RPR_T:
				return; // End of expression, return to previous level

			case EOS_T:      
				matchToken(EOS_T, NO_ATTR);
				loop = 0; // End of expression
				break;

			default:
				printError();
				loop = 0; // End of expression
				return;
		}
	}

	printf("%s%s\n", STR_LANGNAME, ": Expression parsed");
}