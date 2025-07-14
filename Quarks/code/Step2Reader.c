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
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013]
* Assignment: Quarks2.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (Quarks2)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#include <ctype.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

/*
***********************************************************
* Function name: readerCreate
* Purpose: Creates the buffer reader according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: S22
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to reader)
* Algorithm: Allocation of memory according to inicial (default) values.
* TODO ......................................................
*	- Adjust datatypes for your LANGUAGE.
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/

BufferPointer readerCreate(q_int size) {	

	BufferPointer readerPointer = NULL;

	/* TO_DO: Defensive programming: size */
	if (size <= 0) {
		size = READER_DEFAULT_SIZE;
	}
	
	/* TO_DO: readerPointer allocation */
	readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));

	/* TO_DO: Defensive programming: readerPointer */
	if (readerPointer == 	NULL) {
		return NULL;
	}
	/* TO_DO: content allocation */
	readerPointer->content = (q_str)malloc(size * sizeof(q_char));

	/* TO_DO: Defensive programming: content */
	if (readerPointer->content == NULL) {
			free(readerPointer);
			return NULL;
	}

	/* TO_DO: Initialize the histogram */
	for (q_int i = 0; i < NCHAR; ++i) {
		readerPointer->histogram[i] = 0;
	}
	/* TO_DO: Initialize errors */
	readerPointer->numReaderErrors = 0;
	readerPointer->checkSum = 0;

	/* TO_DO: Update the properties */
	readerPointer->size = size;
	readerPointer->position.read = 0;
	readerPointer->position.write = 0;
	readerPointer->position.mark = 0;

	/* TO_DO: Initialize flags */
	readerPointer->flags.isFull = Q_FALSE;
	readerPointer->flags.isRead = Q_FALSE;
	readerPointer->flags.isMoved = Q_FALSE;
	
	/* TO_DO: The created flag must be signalized as EMP */
	readerPointer->flags.isEmpty = Q_TRUE;
	
	return readerPointer;
}	


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

BufferPointer readerAddChar(BufferPointer const readerPointer, q_char ch) {
	q_str tempReader = NULL;
	q_int newSize = 0;
	/* TO_DO: Defensive programming */
	if (!readerPointer || readerPointer->content == NULL) {
		return NULL;
	}
	/* TO_DO: Test the inclusion of chars */
	if (readerPointer->position.write >= readerPointer->size) {
		/* TO_DO: Buffer not full: set flag */
		readerPointer->flags.isFull = Q_TRUE;

		if (readerPointer->size >= READER_MAX_SIZE) {
			return NULL;
		}

		/* TO_DO: Adjust the size to be duplicated */
		newSize = readerPointer->size * 2;
		if (newSize > READER_MAX_SIZE) {
			newSize = READER_MAX_SIZE;
		}

		/* TO_DO: Defensive programming */
		tempReader = (q_str)realloc(readerPointer->content, newSize * sizeof(q_char));
		if (!tempReader) {
			return NULL;
		}

		/* Update buffer properties after successful realloc */
		if (tempReader != readerPointer->content) {
				readerPointer->flags.isMoved = Q_TRUE;
		}
		readerPointer->content = tempReader;
		readerPointer->size = newSize;
		readerPointer->flags.isFull = Q_FALSE;
	} else {
		/* TO_DO: Buffer not full: reset flag */
		readerPointer->flags.isFull = Q_FALSE;
	}

	/* TO_DO: Add the char */
	readerPointer->content[readerPointer->position.write++] = ch;
	readerPointer->flags.isEmpty = Q_FALSE;

	if (readerPointer->position.write == readerPointer->size) {
		readerPointer->flags.isFull = Q_TRUE;
	}	


	/* TO_DO: Updates histogram */
	if ((unsigned char)ch < NCHAR) {
		readerPointer->histogram[(unsigned char)ch]++;
	} else {
		readerPointer->numReaderErrors++;
	}
	
	return readerPointer;

}

/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
q_boo readerClear(BufferPointer const readerPointer) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) { 
		return Q_FALSE;
	}
	
	/* TO_DO: Adjust positions to zero */
	readerPointer->position.read = 0;
	readerPointer->position.write = 0;
	readerPointer->position.mark = 0;

	/* TO_DO: Adjust flags original */
	readerPointer->flags.isFull = Q_FALSE;
	readerPointer->flags.isEmpty = Q_TRUE;
	readerPointer->flags.isRead = Q_FALSE;
	readerPointer->flags.isMoved = Q_FALSE;		
	
	return Q_TRUE;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
q_boo readerFree(BufferPointer const readerPointer) {

	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) { 
		return Q_FALSE;
	}

	/* Free memory (buffer/content) */
	if (readerPointer->content != NULL) {
		free(readerPointer->content);
	}

	free(readerPointer);
	
	return Q_TRUE;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
q_boo readerIsFull(BufferPointer const readerPointer) {

	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) { 
		return Q_FALSE;
	}

	/* TO_DO: Check flag if buffer is FULL */
	if (readerPointer->flags.isFull == Q_TRUE) {
		return Q_TRUE;
	}
	
	return Q_FALSE;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
q_boo readerIsEmpty(BufferPointer const readerPointer) {

	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) { 
		return Q_FALSE;
	}

	/* TO_DO: Check flag if buffer is EMPTY */
	if (readerPointer->flags.isEmpty == Q_TRUE) {
		return Q_TRUE;
	}

	return Q_FALSE;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
q_boo readerSetMark(BufferPointer const readerPointer, q_int mark) {

	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) { 
		return Q_FALSE;
	}

	/* TO_DO: Adjust mark */
	if (mark < 0 || mark >= readerPointer->size) {
		return Q_FALSE; // Invalid mark position
	}
	readerPointer->position.mark = mark;

	return Q_TRUE;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
q_int readerPrint(BufferPointer const readerPointer) {

	/* TO_DO: Defensive programming (including invalid chars) */
	if (readerPointer == NULL || readerPointer->content == NULL) {
		return READER_ERROR; // Invalid reader or content
	}

	/* TO_DO: Print the buffer content */
	q_int charsPrinted = 0;
	for (q_int i = 0; i < readerPointer->position.write; ++i) {

		// Allow tabs and newlines to print normally
		if (readerPointer->content[i] == '\n' || readerPointer->content[i] == '\t' 
			|| readerPointer->content[i]  == ' ' || isprint(readerPointer->content[i] )) {
			putchar(readerPointer->content[i] );
		} else {
			putchar(' '); // Prints if char is invalid
		}
		charsPrinted++;
	}
	
	return charsPrinted;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
q_int readerLoad(BufferPointer const readerPointer, q_str fileName) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL || fileName == NULL) {
		return READER_ERROR;
	}
	/* TO_DO: Loads the file */
	q_str decrypted = vigenereMem(fileName, STR_LANGNAME, DECYPHER);
	if (decrypted == NULL) {
		return READER_ERROR; // Error loading file
	}
	/* TO_DO: Creates the string calling vigenereMem(fileName, STR_LANGNAME, DECYPHER) */
	q_int i = 0;
	while (decrypted[i] != '\0') {
		if (!readerAddChar(readerPointer, decrypted[i++])) {
			free(decrypted);
			return READER_ERROR; // Error adding char to buffer
		}
	}
	free(decrypted);
	return i;
}

/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
q_boo readerRecover(BufferPointer const readerPointer) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) { 
		return Q_FALSE;
	}

	/* TO_DO: Adjust read and mark to zero */
	readerPointer->position.read = 0;
	readerPointer->position.mark = 0;
	
	return Q_TRUE;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
q_boo readerRetract(BufferPointer const readerPointer) {

	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) { 
		return Q_FALSE;
	}

	/* TO_DO: Retract (return 1 pos read) */
	if (readerPointer->position.read > 0) {
		readerPointer->position.read--;
		readerPointer->flags.isRead = Q_FALSE; // Reset read flag
		return Q_TRUE; // Successfully retracted
	}
	
	return Q_FALSE;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
q_boo readerRestore(BufferPointer const readerPointer) {

	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) { 
		return Q_FALSE;
	}

	/* TO_DO: Restore read to mark */
	if (readerPointer->position.mark >= 0 && readerPointer->position.mark < readerPointer->size) {
		readerPointer->position.read = readerPointer->position.mark;
		readerPointer->flags.isRead = Q_TRUE; // Set read flag
	} else {
		return Q_FALSE; // Invalid mark position
	}

	return Q_TRUE;
}



/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
q_char readerGetChar(BufferPointer const readerPointer) {

	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) { 
		return Q_FALSE;
	}

	/* TO_DO: Returns size in the read position and updates read */
	if (readerPointer->position.read < readerPointer->position.write) {
		q_char ch = readerPointer->content[readerPointer->position.read++];
		readerPointer->flags.isRead = Q_TRUE; // Set read flag
		return ch;
	}
	
	return '\0';
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
q_str readerGetContent(BufferPointer const readerPointer, q_int pos) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL || pos < 0 || pos >= readerPointer->position.write) {
		return Q_FALSE; // Invalid reader or position
	}
	/* TO_DO: Return content (string) */
	return &readerPointer->content[pos]; // Return pointer to char at position
}

/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
q_int readerGetPosRead(BufferPointer const readerPointer) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) { 
		return READER_ERROR;
	}

	/* TO_DO: Return read */
	return readerPointer->position.read;

}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
q_int readerGetPosWrte(BufferPointer const readerPointer) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) { 
		return READER_ERROR;
	}
	
	/* TO_DO: Return wrte */
	return readerPointer->position.write;

}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
	q_int readerGetPosMark(BufferPointer const readerPointer) {
		
		/* TO_DO: Defensive programming */
		if (readerPointer == NULL) { 
			return READER_ERROR;
		}
		
		/* TO_DO: Return mark */
		return readerPointer->position.mark;

	}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
q_int readerGetSize(BufferPointer const readerPointer) {

	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) { 
		return READER_ERROR;
	}

	/* TO_DO: Return size */
	return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
q_byte readerGetFlags(BufferPointer const ptr_Buffer) {
    q_byte flags_byte = 0;

    if (ptr_Buffer->flags.isEmpty) flags_byte |= 0x01;  // bit 0
    if (ptr_Buffer->flags.isFull)  flags_byte |= 0x02;  // bit 1
    if (ptr_Buffer->flags.isRead)  flags_byte |= 0x04;  // bit 2
    if (ptr_Buffer->flags.isMoved) flags_byte |= 0x08;  // bit 3

    return flags_byte;
}

/*
***********************************************************
* Function name: readerShowStat
* Purpose: Shows the char statistic.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
q_void readerPrintStat(BufferPointer const readerPointer) {

	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) { 
		return;
	}

	/* TO_DO: Print statistics */
	for (q_int i = 0; i < NCHAR; ++i) {
		if (readerPointer->histogram[i] > 0) {
			if (i == '\n') {
				printf("B[NEW]=%d, ", readerPointer->histogram[i]);
			} else if (i == '\t') {
				printf("B[TAB]=%d, ", readerPointer->histogram[i]);
			} else if (i == ' ') {
				printf("B[SPACE]=%d, ", readerPointer->histogram[i]);
			} else if (i >= 32 && i <= 126) {
				// Printable ASCII characters (excluding control characters)
				printf("B[%c]=%d, ", (char)i, readerPointer->histogram[i]);
			}
		}
	}
	
}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
q_int readerNumErrors(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) { 
		return READER_ERROR; // Invalid reader
	}
	/* TO_DO: Return the number of errors */
	return readerPointer->numReaderErrors;
}

/*
***********************************************************
* Function name: readerChecksum
* Purpose: Sets the checksum of the reader (4 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	[None]
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

q_int readerChecksum(BufferPointer readerPointer) {
	
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) { 
		return Q_FALSE;
	}
	
	/* TO_DO: Return the checksum (given by the content) */
	q_int checksum = 0;
	for (q_int i = 0; i < readerPointer->position.write; ++i) {
		checksum += (unsigned char)readerPointer->content[i];
	}
	readerPointer->checkSum = checksum % 16; // Keep it within 4 bits
	readerPointer->checkSum = (readerPointer->checkSum < 0) ? 0 : readerPointer->checkSum; // Ensure non-negative
	if (readerPointer->checkSum < 0 || readerPointer->checkSum > 15) {
		readerPointer->numReaderErrors++;
		return READER_ERROR; // Invalid checksum
	}
	printf("Checksum: %d\n", readerPointer->checkSum);
	
	return 0;
}
