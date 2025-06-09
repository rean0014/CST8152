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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

// Function to perform the Vigen�re cipher (encoding or decoding)
void vigenereFile(const q_str inputFileName, const q_str outputFileName, const q_str key, q_int encode) {
	// TO_DO: Define the input and output files (ex: FILE* inputFile, FILE* outputFile)
	FILE* inputFile = fopen(inputFileName, "r");
	FILE* outputFile = fopen(outputFileName, "w");
	// TO_DO: Use defensive programming (checking files)
	if (inputFile == NULL) {
		errorPrint("Error opening files.\n");
		if (inputFile != NULL) fclose(inputFile);
    if (outputFile != NULL) fclose(outputFile);
		exit(EXIT_FAILURE);
	}
	// TO_DO: Define local variables
	q_int cuChar;
	q_int keyIndex = 0;
	q_int keyLength = strlen(key);

	if (keyLength == 0) {
		errorPrint("Key cannot be empty.\n");
		fclose(inputFile);
		fclose(outputFile);
		exit(EXIT_FAILURE);
	}

	if (encode != CYPHER && encode != DECYPHER) {
		errorPrint("Invalid operation. Use 1 for cypher or 0 for decypher.\n");
		fclose(inputFile);
		fclose(outputFile);
		exit(EXIT_FAILURE);
	}

	// TO_DO: Logic: check if it is encode / decode to change the char (using Vigenere algorithm) - next function
	if (encode == CYPHER) {
		while ((cuChar = fgetc(inputFile)) != EOF) {
			if (cuChar >= ASCII_START && cuChar <= ASCII_END) {
				q_int keyChar = key[keyIndex % keyLength] - ASCII_START;
				q_int newChar = ((cuChar - ASCII_START + keyChar) % ASCII_RANGE) + ASCII_START;
				fputc(newChar, outputFile);
				keyIndex++;
			} else {
				fputc(cuChar, outputFile);
			}
		}
	} else if (encode == DECYPHER) {
		while ((cuChar = fgetc(inputFile)) != EOF) {
			if (cuChar >= ASCII_START && cuChar <= ASCII_END) {
				q_int keyChar = key[keyIndex % keyLength] - ASCII_START;
				q_int newChar = ((cuChar - ASCII_START - keyChar + ASCII_RANGE) % ASCII_RANGE) + ASCII_START;
				fputc(newChar, outputFile);
				keyIndex++;
			} else {
				fputc(cuChar, outputFile);
			}
		}
	} else {
		errorPrint("Invalid operation.\n");
		exit(EXIT_FAILURE);
	}

	// TO_DO: Close the files
	fclose(inputFile);
	fclose(outputFile);
}

// Function to perform the Vigen�re cipher (encoding or decoding)
q_str vigenereMem(const q_str inputFileName, const q_str key, q_int encode) {
	// TO_DO define the return type and local variables
	q_str output = NULL;
	q_str buffer = NULL;
	FILE* inputFile = NULL;
	q_int fileSize;
	q_int keyLength;
	q_int keyIndex = 0;

	// TO_DO: Check defensive programming
	if (inputFileName == NULL || key == NULL) {
		errorPrint("Input text or key cannot be NULL.\n");
		exit(EXIT_FAILURE);
	}

	if (encode != CYPHER && encode != DECYPHER) {
		errorPrint("Invalid operation. Use 1 for cypher or 0 for decypher.\n");
		exit(EXIT_FAILURE);
	}

	keyLength = strlen(key);
	if (keyLength == 0) {
		errorPrint("Key cannot be empty.\n");
		exit(EXIT_FAILURE);
	}

	fileSize = getSizeOfFile(inputFileName);
	if (fileSize <= 0) {
		errorPrint("Error getting file size.\n");
		exit(EXIT_FAILURE);
	}

	output = (q_str)malloc((fileSize + 1) * sizeof(char));
	if (output == NULL) {
		errorPrint("Memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}

	buffer = (q_str)malloc((fileSize + 1) * sizeof(char));
	if (buffer == NULL) {
		errorPrint("Memory allocation failed.\n");
		free(output);
		exit(EXIT_FAILURE);
	}

	inputFile = fopen(inputFileName, "r");
	if (inputFile == NULL) {
		errorPrint("Error opening file: %s\n", inputFileName);
		free(output);
		free(buffer);
		exit(EXIT_FAILURE);
	}

	size_t bytesRead = fread(buffer, sizeof(char), fileSize, inputFile);
	fclose(inputFile);

	buffer[bytesRead] = '\0';

	// TO_DO: Use the logic to code/decode - consider the logic about visible chars only
	if (encode == CYPHER) {
		for (q_int i = 0; i < bytesRead; i++) {
			if (buffer[i] >= ASCII_START && buffer[i] <= ASCII_END) {
				q_int keyChar = key[keyIndex % keyLength] - ASCII_START;
				output[i] = ((buffer[i] - ASCII_START + keyChar) % ASCII_RANGE) + ASCII_START;
				keyIndex++;
			} else {
				output[i] = buffer[i];
			}
		}
	} else if (encode == DECYPHER) {
		for (q_int i = 0; i < bytesRead; i++) {
			if (buffer[i] >= ASCII_START && buffer[i] <= ASCII_END) {
				q_int keyChar = key[keyIndex % keyLength] - ASCII_START;
				output[i] = ((buffer[i] - ASCII_START - keyChar + ASCII_RANGE) % ASCII_RANGE) + ASCII_START;
				keyIndex++;
			} else {
				output[i] = buffer[i];
			}
		}
	} else {
		errorPrint("Invalid operation.\n");
		free(output);
		free(buffer);
		exit(EXIT_FAILURE);
	}

	output[bytesRead] = '\0';

	free(buffer);

	return output;
}

// Function to encode (cypher)
void cypher(const q_str inputFileName, const q_str outputFileName, const q_str key) {
    vigenereFile(inputFileName, outputFileName, key, CYPHER);
}

// Function to decode (decypher)
void decypher(const q_str inputFileName, const q_str outputFileName, const q_str key) {
    vigenereFile(inputFileName, outputFileName, key, DECYPHER);
}

// TO_DO: Get file size (util method)
q_int getSizeOfFile(const q_str filename) {
    // TO_DO: Use the logic to get the size of the file
		q_int size = 0;
		FILE* file = fopen(filename, "r");
		if (file == NULL) {
			printf("Error opening file: %s\n", filename);
		}

		fseek(file, 0, SEEK_END);
		size = ftell(file);
		fclose(file);

    return size;
}
