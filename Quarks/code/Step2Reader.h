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
	# ECHO "[CODER SCRIPT ..........................]"
	# ECHO "                                         "
	*/

	/*
	************************************************************
	* File name: Reader.h
	* Compiler: MS Visual Studio 2022
	* Course: CST 8152 � Compilers, Lab Section: [011, 012]
	* Assignment: Quarks2.
	* Date: Jan 01 2025
	* Professor: Paulo Sousa
	* Purpose: This file is the main header for Reader (.h)
	************************************************************
	*/

	/*
	*.............................................................................
	* MAIN ADVICE:
	* Please check the "TODO" labels to develop your activity.
	*.............................................................................
	*/

	#ifndef COMPILERS_H_
	#include "Compilers.h"
	#endif

	#ifndef READER_H_
	#define READER_H_

	/* TIP: Do not change pragmas, unless necessary .......................................*/
	//#pragma warning(1:4001) /*to enforce C89 type comments  - to make //comments an warning */

	/* standard header files */
	#include <stdio.h>  /* standard input/output */
	#include <stdlib.h> /* for dynamic memory allocation */
	#include <limits.h> /* implementation-defined data type ranges and limits */

	/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

	/* Constants about controls (not need to change) */
	#define READER_ERROR				(-1)	/* General error message */
	#define READER_TERMINATOR			'\0'	/* General EOF */

	/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME .................................. */

	/* TO_DO: You should add your own constant definitions here */
	#define READER_MAX_SIZE		65536		/* maximum capacity */ 

	#define READER_DEFAULT_SIZE	100			/* default initial buffer reader capacity */

	#define NCHAR				128			/* Chars from 0 to 127 */

	#define EOF_CODE			(-2)		/* EOF Code for Reader */

	/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME .................................. */

	/* TODO: Adjust datatypes */

	/* Offset declaration */
	typedef struct position {
		q_int read;			/* the offset to the get a char position (in chars) */
		q_int write;			/* the offset to the add chars (in chars) */
		q_int mark;			/* the offset to the mark position (in chars) */
	} Position;

	typedef struct flag {
		q_boo isEmpty; /* checks if there is no content */
		q_boo isFull; /* the content is using all size */
		q_boo isRead; /* all content was read */
		q_boo isMoved; /* the content was moved in reallocation */
	} Flag;

	/* Buffer structure */
	typedef struct bufferReader {
		q_str		content;			/* pointer to the beginning of character array (character buffer) */
		q_int		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
		Flag		flags;				/* contains character array reallocation flag and end-of-buffer flag */
		Position	position;			/* Offset / position field */
		q_int		histogram[NCHAR];	/* Statistics of chars */
		q_int		numReaderErrors;	/* Number of errors from Reader */
		q_byte		checkSum;			/* Sum of bytes(chars) */
	} Buffer, *BufferPointer;

	/* FUNCTIONS DECLARATION:  .................................. */

	/* General Operations */
	BufferPointer	readerCreate		(q_int);
	BufferPointer	readerAddChar		(BufferPointer const, q_char);
	q_boo		readerClear		    (BufferPointer const);
	q_boo		readerFree		    (BufferPointer const);
	q_boo		readerIsFull		(BufferPointer const);
	q_boo		readerIsEmpty		(BufferPointer const);
	q_boo		readerSetMark		(BufferPointer const, q_int);
	q_int		readerPrint		    (BufferPointer const);
	q_int		readerLoad			(BufferPointer const, q_str);
	q_boo		readerRecover		(BufferPointer const);
	q_boo		readerRetract		(BufferPointer const);
	q_boo		readerRestore		(BufferPointer const);
	q_int		readerChecksum		(BufferPointer const);
	/* Getters */
	q_char		readerGetChar		(BufferPointer const);
	q_str		readerGetContent	(BufferPointer const, q_int);
	q_int		readerGetPosRead	(BufferPointer const);
	q_int		readerGetPosWrte	(BufferPointer const);
	q_int		readerGetPosMark	(BufferPointer const);
	q_int		readerGetSize		(BufferPointer const);
	q_byte		readerGetFlags		(BufferPointer const);
	q_void		readerPrintStat		(BufferPointer const);
	q_int		readerNumErrors		(BufferPointer const);

	#endif
