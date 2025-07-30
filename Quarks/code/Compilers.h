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
	* File name: Compilers.h
	* Compiler: MS Visual Studio 2022
	* Course: CST 8152 � Compilers, Lab Section: [011, 012]
	* Assignment: Quarks-A5.
	* Date: Jan 01 2025
	* Professor: Paulo Sousa
	* Purpose: This file defines the functions called by main function.
	* Function list: mainReader(), mainScanner(), mainParser().
	*************************************************************/

	#ifndef COMPILERS_H_
	#define COMPILERS_H_

	#define DEBUG 0

	/*
	* ............................................................................
	* ADVICE 1:
	* Please check the "TODO" labels to develop your activity.
	*
	* ADVICE 2: This file must be updated according to each assignment
	* (see the progression: reader > scanner > parser).
	* ............................................................................
	*/

	/* TO_DO: Adjust your language (cannot be "") */
	/* Language name */
	#define STR_LANGNAME	"Quark"

	/* Logical constants - adapt for your language */
	#define Q_TRUE  1
	#define Q_FALSE 0

	#define INVALID NULL
	#define EOF_CHAR '\0'

	/*
	------------------------------------------------------------
	Data types definitions
	NOTE: Some types may not be directly used by your language,
			but they can be necessary for conversions.
	------------------------------------------------------------
	*/

	/* TO_DO: Define your typedefs */
	typedef char			q_char;
	typedef char*			q_str;
	typedef int				q_int;
	typedef float			q_float;
	typedef void			q_void;

	typedef unsigned char	q_boo;
	typedef unsigned char	q_byte;

	typedef long			q_long;
	typedef double			q_doub;
	/*
	------------------------------------------------------------
	Programs:
	1: Coder - invokes Main1Coder code
	2: Reader - invokes Main2Reader code
	3: Scanner - invokes Main3Scanner code
	4: Parser - invokes Main4Parser code
	5: Writer - invokes Main5Writer code
	------------------------------------------------------------
	*/
	enum PROGRAMS {
		PGM_CDR = '1', // Coder:	05pt
		PGM_RDR = '2', // Reader:	05pt
		PGM_SCN = '3', // Scanner:	15pt
		PGM_PSR = '4', // Parser:	15pt
		PGM_WRT = '5', // Writer:	10pt
	};

	/*
	------------------------------------------------------------
	Main functions signatures
	(Code will be updated during next assignments)
	------------------------------------------------------------
	*/
	q_int main1Coder	(q_int argc, q_str* argv);
	q_int main2Reader	(q_int argc, q_str* argv);
	q_int main3Scanner	(q_int argc, q_str* argv);
	q_int main4Parser	(q_int argc, q_str* argv);
	q_int main5Writer	(q_int argc, q_str* argv);

	q_void printLogo();
	q_void errorPrint(q_str fmt, ...);

	#endif

	/*
			"args": [
			1,
			1,
			"README.txt",
			"CODED.txt"
			]
			"args": [
			4,
			"CODED.txt"
			]
	*/
