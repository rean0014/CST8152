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
* File name: Writer.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A12.
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

#ifndef WRITER_H_
#define WRITER_H_

#define MAX_VARS 100
#define MAX_EXPR_LEN 256
#define MAX_LINES 100

#define WRITE "qout"

#define EOS '\0'
#define ZERO 0.0
#define STREMPTY ""
#define LPAR '('
#define RPAR ')'
#define PLUS '+'
#define MINUS '-'
#define MULT '*'
#define DIV '/'
#define AMP '&'

#define EQUALS '='
#define MOD '%'
#define SPACE ' '
#define TAB '\t'
#define NEWLINE '\n'
#define RETURN '\r'
#define QUOTES '"'
#define QUOTE '\''

#define TRUE "true"
#define FALSE "false"

typedef enum { NUMERIC, STRING, BOOLEAN, CHAR } VarType;

typedef struct {
    q_char name[32];
    VarType type;
    union {
        q_doub num_value;
        q_char str_value[256];
        q_int bool_value; // For BOOLEAN type (1 for true, 0 for false)
        q_char char_value; // For CHAR type
    } value;
} Variable;

q_int find_variable(const q_str name);
q_int add_variable(const q_str name, VarType type);
q_void set_variable(const q_str name, VarType type, q_void* value);
q_doub evaluate_simple_expression(const q_char *expr);

q_void assign_string_variable(const q_str name, const q_str value);
const q_str get_string_value(const q_str name);

q_void handle_write(q_str expression);
q_void process_file(const q_str filename);
q_void process_content(q_str content);

#endif
