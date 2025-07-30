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
***********************************************************
* File name: Writer.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef WRITER_H_
#include "Step5Writer.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

#ifndef PARSER_H_
#include "Step4Parser.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Global variables */
Variable variables[MAX_VARS];
q_int var_count = 0;
q_int initial_phase = 1; // Flag to track the initial phase
q_char output_buffer[MAX_EXPR_LEN * 10] = { 0 }; // Buffer to store write output

/* Finds variables */
q_int find_variable(const q_str name) {
    q_int i = 0;
    for (i = 0; i < var_count; ++i) {
        if (strcmp(variables[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

/* Assign string variable */
q_void assign_string_variable(const q_str name, const q_str value) {
    q_int idx = find_variable(name);
    if (idx == -1) {
        idx = var_count++;
        strncpy(variables[idx].value.str_value, value, sizeof(variables[idx].value.str_value) - 1);
        variables[idx].value.str_value[sizeof(variables[idx].value.str_value) - 1] = EOS;
    }
    variables[idx].type = STRING;
    strncpy(variables[idx].value.str_value, value, sizeof(variables[idx].value.str_value) - 1);
    variables[idx].value.str_value[sizeof(variables[idx].value.str_value) - 1] = EOS;
}

/* Gets string variable */
const q_str get_string_value(const q_str name) {
    q_int idx = find_variable(name);
    if (idx != -1 && variables[idx].type == STRING) {
        return variables[idx].value.str_value;
    }
    return STREMPTY;
}

/* Write output */
q_void handle_write(q_str expression) {
    q_char buffer[MAX_EXPR_LEN] = { 0 };
    q_str start = strchr(expression, LPAR) + 1;
    q_str end = strrchr(expression, RPAR);
    if (start != NULL && end != NULL && start < end) {
        *end = EOS;
        while (*start != EOS) {
            if (*start == QUOTES) {
                start++;
                while (*start != QUOTES && *start != EOS) {
                    strncat(buffer, start, 1);
                    start++;
                }
                start++;
            }
            else if (*start == PLUS) {
                start++; // skip '+'
            }
            else if (*start == AMP) {
                start++; // skip '&'

                q_char var_name[32] = { 0 };
                q_int i = 0;

                // Read variable name
                while (isalnum(*start) || *start == '_') {
                    var_name[i++] = *start++;
                }
                var_name[i] = EOS;

                q_int var_idx = find_variable(var_name);

                if (var_idx != -1) {
                    Variable var = variables[var_idx];

                    // Append variable value as string to buffer
                    if (var.type == STRING) {
                        strncat(buffer, var.value.str_value, MAX_EXPR_LEN - strlen(buffer) - 1);

                    } else if (var.type == NUMERIC) {
                        q_char temp[64];
                        snprintf(temp, sizeof(temp), "%g", var.value.num_value);
                        strncat(buffer, temp, MAX_EXPR_LEN - strlen(buffer) - 1);

                    } else if (var.type == BOOLEAN) {
                        strncat(buffer, var.value.bool_value ? "true" : "false", MAX_EXPR_LEN - strlen(buffer) - 1);
                    
                    } else if (var.type == CHAR) {
                        q_char temp[2] = { var.value.char_value, '\0' };
                        strncat(buffer, temp, MAX_EXPR_LEN - strlen(buffer) - 1);
                    }
                }
            }
            else if (isalpha(*start)) {
                q_char var_name[32] = { 0 };
                q_int i = 0;
                while (isalnum(*start)) {
                    var_name[i++] = *start++;
                }
                q_int var_idx = find_variable(var_name);
                if (var_idx != -1) {
                    if (variables[var_idx].type == STRING) {
                        strncat(buffer, get_string_value(var_name), strlen(get_string_value(var_name)));
                    }
                }
            }
            else if (isspace(*start)) {
                strncat(buffer, start, 1);
                start++;
            }
            else {
                start++;
            }
        }
    }
    if (initial_phase) {
        strcat(output_buffer, buffer);
        strcat(output_buffer, "\n");
    }
    else {
        printf("%s\n", buffer);
    }
}

/* Calculate expression */
q_void calculate(q_str expression) {
    q_char var_name[32] = { 0 };
    q_char keyword[32] = { 0 };
    if (strchr(expression, EQUALS)) {
        sscanf(expression, "%31s %31s =", keyword, var_name);
        q_str expr = strchr(expression, EQUALS) + 1;

        while (isspace(*expr)) expr++;

        if (*expr == QUOTES) {
            expr++;
            q_char str_value[256] = { 0 };
            q_int i = 0;
            while (*expr != QUOTES && *expr != EOS) {
                str_value[i++] = *expr++;
            }
            set_variable(var_name, STRING, str_value);
            if (!initial_phase) {
                printf("%s = \"%s\"\n", var_name, str_value);
            }
        } else {
            q_char eval_expr[256] = { 0 };
            q_int ei = 0;

            while (*expr != EOS && ei < 255) {
                if (*expr == AMP) {
                    expr++;
                    q_char var_ref[32] = { 0 };
                    q_int vi = 0;
                    while (isalnum(*expr) || *expr == '_') {
                        var_ref[vi++] = *expr++;
                    }
                    var_ref[vi] = EOS;

                    q_int idx = find_variable(var_ref);
                    if (idx != -1 && variables[idx].type == NUMERIC) {
                        ei += snprintf(&eval_expr[ei], sizeof(eval_expr) - ei, "%.6f", variables[idx].value.num_value);
                    } else {
                        fprintf(stderr, "Unknown or non-numeric variable: &%s\n", var_ref);
                        return;
                    }
                } else {
                    eval_expr[ei++] = *expr++;
                }
            }
            eval_expr[ei] = EOS;

            // Evaluate the full substituted string expression
            double result = evaluate_simple_expression(eval_expr); 
            set_variable(var_name, NUMERIC, &result);
            if (!initial_phase) {
                printf("%s = %.2lf\n", var_name, result);
            }
        }

    }
    
    else if (strstr(expression, WRITE)) {
        handle_write(expression);
    }
}


/* Process input file */
q_void process_file(const q_str filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    q_char line[MAX_EXPR_LEN];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == NEWLINE || line[0] == RETURN) {
            continue; // Skip empty lines
        }
        line[strcspn(line, "\n")] = 0; // Remove newline character
        calculate(line);
    }
    fclose(file);
    initial_phase = 0; // End of initial phase
    printf("%s", output_buffer); // Print the buffered write output
    printf("\nVariable values:\n");
	q_int i = 0;
    for (i = 0; i < var_count; i++) {
        if (variables[i].type == STRING) {
            printf("%s = \"%s\"\n", variables[i].name, variables[i].value.str_value);
        }
        else if (variables[i].type == NUMERIC) {
            printf("%s = %.2lf\n", variables[i].name, variables[i].value.num_value);
        }
        else if (variables[i].type == BOOLEAN) {
            printf("%s = %s\n", variables[i].name, variables[i].value.bool_value ? "true" : "false");
        }
        else if (variables[i].type == CHAR) {
            printf("%s = '%c'\n", variables[i].name, variables[i].value.char_value);
        }
    }
}

/* Split lines into components */
q_str* splitIntoLines(const q_str content, q_int* lineCount) {
    q_str* lines = malloc(MAX_LINES * sizeof(char*));
    if (!lines) {
        perror("Error allocating memory");
        return NULL;
    }
    *lineCount = 0;
    const q_char* start = content;
    const q_char* end;
    while ((end = strchr(start, NEWLINE)) != NULL) {
        if (*lineCount >= MAX_LINES) {
            fprintf(stderr, "Exceeded maximum number of lines\n");
            break;
        }
        q_int lineLength = (q_int) (end - start);
        lines[*lineCount] = malloc(lineLength + 1);
        if (!lines[*lineCount]) {
            perror("Error allocating memory for line");
            break;
        }
        // Use strncpy_s for safer copying
        strncpy(lines[*lineCount], start, lineLength);
        lines[*lineCount][lineLength] = '\0';
        
        lines[*lineCount][lineLength] = EOS; // Null-terminate the string
        (*lineCount)++;
        start = end + 1;
    }
    // Handle the last line if it doesn't end with a newline
    if (*start != EOS && *lineCount < MAX_LINES) {
        size_t lineLength = strlen(start);
        lines[*lineCount] = malloc(lineLength + 1);
        if (!lines[*lineCount]) {
            perror("Error allocating memory for line");
        }
        else {
            // Use _strdup to copy the remaining content
            lines[*lineCount] = strdup(start);
            if (!lines[*lineCount]) {
                perror("Error copying string using _strdup");
            }
            else {
                (*lineCount)++;
            }
        }
    }
    return lines;
}

/* Free lines */
q_void freeLines(q_str* lines, q_int lineCount) {
    q_int i = 0;
    for (i = 0; i < lineCount; i++) {
        free(lines[i]);
    }
    free(lines);
}

/* Process content */
q_void process_content(q_str fileContent) {
    q_int lineCount = 0;
    q_str* lines = splitIntoLines(fileContent, &lineCount);
    q_str line = malloc(MAX_EXPR_LEN);
    if (!lines || !line) {
        return;
    }
    printf("Lines from content:\n");
    q_int i = 0;
	for (i = 0; i < lineCount; i++) {
		line = lines[i];
		calculate(line);
	}
    initial_phase = 0; // End of initial phase
    printf("%s", output_buffer); // Print the buffered write output
    printf("\nVariable values:\n");
    for (i = 0; i < var_count; i++) {
        if (variables[i].type == STRING) {
            printf("%s = \"%s\"\n", variables[i].name, variables[i].value.str_value);
        }
        else if (variables[i].type == NUMERIC) {
            printf("%s = %.2lf\n", variables[i].name, variables[i].value.num_value);
        }
        else if (variables[i].type == BOOLEAN) {
            printf("%s = %s\n", variables[i].name, variables[i].value.bool_value ? "true" : "false");
        }
        else if (variables[i].type == CHAR) {
            printf("%s = '%c'\n", variables[i].name, variables[i].value.char_value);
        }
    }
}

q_void set_variable(const q_str name, VarType type, q_void* value) {
    q_int idx = find_variable(name);
    if (idx == -1) {
        idx = var_count++;
        strncpy(variables[idx].name, name, sizeof(variables[idx].name) - 1);
    }
    variables[idx].type = type;
    switch (type) {
        case STRING:
            strncpy(variables[idx].value.str_value, (q_char*)value, sizeof(variables[idx].value.str_value) - 1);
            break;
        case NUMERIC:
            variables[idx].value.num_value = *((q_doub*)value);
            break;
        default:
            break;
    }
}
double evaluate_simple_expression(const char* expr) {
    double result = 0.0;
    char op = '+';
    const char* p = expr;

    while (*p != EOS) {
        while (isspace(*p)) p++;

        double value = strtod(p, (char**)&p);

        switch (op) {
            case '+': result += value; break;
            case '-': result -= value; break;
            case '*': result *= value; break;
            case '/': result /= value; break;
        }

        while (isspace(*p)) p++;
        op = *p;
        if (*p != EOS) p++;
    }

    return result;
}
