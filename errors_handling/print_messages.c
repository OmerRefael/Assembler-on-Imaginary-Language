#include <stdio.h>
#include "print_messages.h"
#include "../enums/errors.h"
#include "../enums/warnings.h"

#define NOT_IN_SPECIFIC_LINE 0 /* The error that happened, is not in a specific line, so we will not print the line number (we start to count from 1 the lines) */
#define NOT_IN_SPECIFIC_FILE NULL /* The error that happened, is not in a specific file, so we will not print the file name */


/**
 * this function prints the error message according to the error that happened and the line number and the file name (if the error is in a specific line or file)
 * 
 * parameters:
 *     error - the error that happened
 *     line_number - the number of the line that the error happened (if the error is not in a specific line, the value will be 0 - we start to count from 1 the lines)
 *     file_name - the name of the file that the error happened (if the error is not in a specific file, the value will be NULL)
 * 
 */
void print_errors(errors error, int line_number,char *file_name){
    char *messages[] = {
        /* Abstract Errors */
        "Everything went successfully.",

        /* General Errors */
        "ERROR: Allocation failed.",
        "ERROR: The file can't be opened.",
        "ERROR: The line is too long.",
        "ERROR: No files were passed as arguments to run the program.",
        "ERROR: Exceeded memory limit.",

        /* Syntax Errors */
        "ERROR: Multiple commas - missing value between commas.",
        "ERROR: Missing comma between the operands.",
        "ERROR: Illegal comma.",
        "ERROR: There is an comma at the end of the line.",

        /* Macro Errors - Pre Assembler */
        "ERROR: There is not name (empty name) for the macro that defined.",
        "ERROR: The macro name is too long.",
        "ERROR: You can not use this name for macro - this is a saved word.",
        "ERROR: The macro name contains illegal characters.",
        "ERROR: Cannot define a macro that has already been defined.",
        "ERROR: In the macro definition, there is extra text.",
        "ERROR: At the end of the macro definition, there is extra text.",
        "ERROR: The macro name does not start with a letter.",

        /* Label Errors - First Pass */
        "ERROR: There is not name (empty name) for the label that defined.",
        "ERROR: The label name is too long.",
        "ERROR: You can not use this name for label - this is a saved word.",
        "ERROR: The label name does not start with a letter.",
        "ERROR: There is no space after the label definition.",
        "ERROR: The label name contains illegal characters - only letters and numbers are allowed.",
        "ERROR: Another label with the same name already exists.",
        "ERROR: Label can not have the same name as a macro that defined in the file.",
        "ERROR: Label without content - after the label definition, there is no command or guidance.",

        /* String Guidance Errors - First Pass */
        "ERROR: Missing parameter for the string guidance.",
        "ERROR: There is no opening quotation mark in the string parameter.",
        "ERROR: There is no closing quotation mark in the string parameter.",
        "ERROR: The string parameter is empty.",
        "ERROR: There is extra text before the string parameter.",
        "ERROR: There is extra text after the string parameter.",

        /* Data Guidance Errors - First Pass */
        "ERROR: Missing parameters for the data guidance.",
        "ERROR: Invalid type of member in data guidance - the member must be an integer number.",
        "ERROR: Invalid value of member in data guidance - the value of the number is not in range.",

        /* Entry and Extern Errors - First Pass */
        "ERROR: Missing parameters for entry/extern guidance.",
        "ERROR: Incorrect number of operands for entry/extern guidance.",

        /* Instruction Errors - First Pass */
        "ERROR: Undefined command.",
        "ERROR: Incorrect number of operands for command.",
        "ERROR: Invalid type of the operand in instruction line - the operand must be an integer number (when using immediate adressing methods).",
        "ERROR: Invalid value of the number in instruction line - the value of the number is not in range.",
        "ERROR: Invalid value of register - in indirect adressing the register must to be register between r0 to r7.",
        "ERROR: Invalid operand - the operand must be a number, register (in indirect or direct adressing) or label (Ensure the label follows the correct definition rules, else it wont consider as label).",
        "ERROR: Illegal source operand - you can not use this addressing method on the source operand in this command.",
        "ERROR: Illegal destination operand - you can not use this addressing method on the destination operand in this command.",

        /* Entry/Extern Guidance Errors - Second Pass */
        "ERROR: This label of the entry guidance that declared but was not defined.",
        "ERROR: Label is defined as both entry and external.",

        /* Label Errors - Second Pass */
        "ERROR: The label not defined.",

        /* File Creation Errors */
        "ERROR: Cannot create object file.",
        "ERROR: Cannot create entry file.",
        "ERROR: Cannot create extern file."
    };
    printf("\n-----------------------------------------------------------------------------------------\n");
    if (error == THERE_IS_NO_ERROR){ /* If there is a message that we finish successfully the process of the file, we will treat it differently */
        printf("In file \"%s.as\" - %s\n", file_name, messages[error]);
    }
    else{
        if (line_number != NOT_IN_SPECIFIC_LINE){ /* If the error is in a specific line */
        printf("In file -  \"%s\", at line - %d:\n", file_name,line_number);
        }
        else if(file_name != NOT_IN_SPECIFIC_FILE){ /* If the error is in a specific file */
            printf("In file - \"%s\":\n", file_name);
        }
        printf("%s\n", messages[error]); /* Print the error message */
    }
    printf("-----------------------------------------------------------------------------------------\n");
}

 
/**
 * this function prints the warning message according to the warning that happened and the line number and the file name (if the warning is in a specific line or file)
 * 
 * parameters:
 *     warning - the warning that happened
 *     line_number - the number of the line that the warning happened (if the warning is not in a specific line, the value will be 0 - we start to count from 1 the lines)
 *     file_name - the name of the file that the warning happened (if the warning is not in a specific file, the value will be NULL)
 * 
 */
void print_warnings(warnings warning, int line_number, char *file_name){
    char *messages[] = {
        "There is no warning.",
        "WARNING: There is an entry guidance with label definition in the same line.",
        "WARNING: There is an extern guidance with label definition in the same line."  
    };
    printf("\n-----------------------------------------------------------------------------------------\n");
    if (line_number != NOT_IN_SPECIFIC_LINE){ /* If the warning is in a specific line */
        printf("In file - %s, at line - %d:\n", file_name, line_number);
    }
    else if(file_name != NOT_IN_SPECIFIC_FILE){ /* If the warning is in a specific file */
        printf("In file - %s:\n", file_name);
    }
    printf("%s\n", messages[warning]); /* Print the warning message */
    printf("-----------------------------------------------------------------------------------------\n");
}