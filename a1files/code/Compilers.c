/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: David Lainez & Bianca Reaney Ibarra
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
# ECHO "[COMPILER SCRIPT .......................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: compilers.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main program of Compilers Project
* Function list: main().
************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/*
***********************************************************
* Function name: main
* Purpose: Main function
* Author: Paulo Sousa
* History/Versions: Ver F22
* Called functions: mainReader(), mainScanner(), mainParser()
* Parameters: Command line arguments - argc, argv
* Return value: Status
* Algorithm: -
*************************************************************
*/

q_int main(int argc, char** argv) {

	q_int i;
	printLogo();
	if (DEBUG) {
		for (i = 0; i < argc; ++i)
			printf("argv[%d] = %s\n", i, argv[i]);
	}
	if (argc < 2) {
		printf("%s%c%s%c%s%c%s%c%s%c%s", "OPTIONS:\n* [",
			PGM_CDR, "] - Coder\n* [",
			PGM_RDR, "] - Reader\n* [",
			PGM_SCN, "] - Scanner\n* [",
			PGM_PSR, "] - Parser\n* [",
			PGM_WRT, "] - Writer\n");
		return EXIT_FAILURE;
	}
	q_char option = argv[1][0];
	switch (option) {
	case PGM_CDR:
		printf("%s%c%s", "\n[Option '", PGM_CDR, "': Starting CODER .....]\n\n");
		main1Coder(argc, argv);
		break;
	default:
		printf("%s%c%s%c%s%c%s%c%s%c%s", "OPTIONS:\n* [",
			PGM_CDR, "] - Coder\n* [",
			PGM_RDR, "] - Reader\n* [",
			PGM_SCN, "] - Scanner\n* [",
			PGM_PSR, "] - Parser\n* [",
			PGM_WRT, "] - Writer\n");
		break;
	}
	return EXIT_SUCCESS;
}

/*
***********************************************************
* Function name: printLogo
* Purpose: Print Logo
* Author: Paulo Sousa
* History/Versions: Ver S23
* Called functions: -
* Parameters: -
* Return value: (Null)
* Algorithm: -
*************************************************************
*/

q_void printLogo() {
	printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"\t=---------------------------------------=\n",
		"\t|  COMPILERS - ALGONQUIN COLLEGE (S25)  |\n",
		"\t=---------------------------------------=\n",
		"\t    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n",
		"\t    @@          ________          @@    \n",
		"\t    @@      /%&@@@@@@*@@ @%\\      @@    \n",
		"\t    @@     % (@ %% &&@@  &@@@     @@    \n",
		"\t    @@    @& @@/        \\%%  @    @@    \n",
		"\t    @@    @ @*@          @ @ @    @@    \n",
		"\t    @@    & @ @          @|  @    @@    \n",
		"\t    @@    @/ @@          @%  @    @@    \n",
		"\t    @@    @* &@          @ @ @    @@    \n",
		"\t    @@    @| %@          @@@*@    @@    \n",
		"\t    @@    @|  @\\ \\&&&\\  /@@@ @    @@    \n",
		"\t    @@     @**@@@ &\\&&&\\ @* @/    @@    \n",
		"\t    @@      \\%@ %&&@@ \\&&&\\       @@    \n",
		"\t    @@                  \\&&&\\     @@    \n",
		"\t    @@         Q U A R K          @@    \n",
		"\t    @@                            @@    \n",
		"\t    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n",
		"\t                                        \n",
		"\t[COMPILER SCRIPT ......................]\n",
		"\t                                        \n"
	);
}

/*
************************************************************
* Error printing function with variable number of arguments
* Params: Variable arguments, using formats from C language.
*	- Internal vars use list of arguments and types from stdarg.h
*   - NOTE: The format is using signature from C Language
************************************************************
*/

q_void errorPrint(q_str fmt, ...) {
	/* Initialize variable list */
	va_list ap;
	va_start(ap, fmt);

	(q_void)vfprintf(stderr, fmt, ap);
	va_end(ap);

	/* Move to new line */
	if (strchr(fmt, '\n') == NULL)
		fprintf(stderr, "\n");
}
