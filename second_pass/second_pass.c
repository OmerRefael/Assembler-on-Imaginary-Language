#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "second_pass.h"
#include "encode_second_pass.h"
#include "../labels_structure/table_of_labels.h"
#include "../errors_handling/check_errors_in_second_pass.h"
#include "../enums/boolean.h"
#include "../enums/errors.h"
#include "../enums/guidance.h"
#include "../enums/labels.h"
#include "../enums/operand_type.h"
#include "../util/help_methods.h"
#include "../first_pass/help_methods_for_first_pass.h"

#define FIRST_LINE 1 /* the first line in the file (we start to count from 1 the lines) */
#define NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY 2 /* The number of columns in the instruction and data arrays */
#define MAX_CHARS_IN_LINE 82 /* the maximum length of the line is 81 and +1 for the null terminator, when we define the array */
#define MAX_CHARS_IN_LABEL 32 /* the maximum length of the label is 31 and +1 for the null terminator, when we define the array */ 
#define FIRST_PARAMETER 1 /* the first parameter in the  line */
#define SECOND_PARAMETER 2 /* the second parameter in the  line */
#define START_VALUE_OF_IC 0 /* the start value of the instruction counter */
#define NOT_FOUND -1 /* the value that we return if we didn't find the label */

/* ---------------PROTOTYPES--------------- */

errors open_files_second_pass(FILE **, char **, char *, boolean *);
void handle_entry_guidance_at_second_pass(char [], char *, int, errors *, table_of_labels *);
void handle_instruction_at_second_pass(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char [], char *, int, int *, errors *, table_of_labels *);
void finish_second_pass(FILE *, char *);

/* ---------------END OF PROTOTYPES--------------- */





/**
 * this function performs the second pass of the assembler on the given file. it checks for errors in the file (that only can be detected in the second pass),
 * completes the encoding and stores the machine code of the instructions and data in the provided arrays.
 * 
 * Parameters:
 *    instruction_array - array to store the machine code of the instructions line encoded
 *    file_name - the name of the file that we are going to read from
 *    table - the table of labels 
 *    allocation_faild_flag - the flag that indicates if there is an allocation error 
 * 
 * Returns:
 *    THERE_IS_NO_ERROR - if there are no errors in the second pass.
 *    THERE_IS_AN_ERROR - i  f there are errors in the second pass.
 */
errors second_pass(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char *file_name, table_of_labels *table, boolean *allocation_faild_flag){
    FILE *file; /* the file that we are going to read from */
    char line[MAX_CHARS_IN_LINE]; /* the current line that we are reading from the file */
    char *final_name; /* the final name of the file (after we add the correct extension to the name of the file) */
    int number_line = FIRST_LINE; /* the number of the current line that we are reading from the file */
    int IC = START_VALUE_OF_IC; /* the instruction counter */
    errors is_error = THERE_IS_NO_ERROR; /* indicate if there is an error in the second pass of the file */
    guidance guidance_type; /* the type of the guidance in the line */
    if (open_files_second_pass(&file, &final_name, file_name, allocation_faild_flag) == THERE_IS_AN_ERROR){ /* open the file and check if there is an error to open the file (if so, the function wiil free all and print the error) */
        return THERE_IS_AN_ERROR;
    }

    while (fgets(line, MAX_CHARS_IN_LINE, file) && *allocation_faild_flag == False){ /* read line by line until the end of the file, or until there is an allocation error */
        guidance_type = get_guidance_type(line);
        if (is_label_definition(line) == True){ /* if there is a label definition in the line */
            change_the_line_to_be_without_label_definition(line); /* change the line to be without the label definition */
        }
        if (guidance_type == data_guidance || guidance_type == string_guidance || guidance_type == extern_guidance){ number_line++;continue;} /* if the line is a data, string or extern guidance, we don't need to handle it at the second pass */
        if (guidance_type == entry_guidance){ /* if the line is an entry guidance, we need to handle it at the second pass */
            handle_entry_guidance_at_second_pass(line, final_name, number_line, &is_error, table);
        }
        else{
            handle_instruction_at_second_pass(instruction_array, line, final_name, number_line, &IC, &is_error, table); /* handle the instruction at the second pass */
        }
        number_line++; /* increase the number of the line */
    }
    finish_second_pass(file, final_name); /* close the file and free the final name of the file */
    return is_error; /* return if there is an error in the second pass of the file */
}


/**
 * this function opens the file that we are checking, and check if there is an error to open the file,
 * also it gets the full name of the file with the extension after we add the correct extension to the name of the file
 * 
 * parameters:
 *    input_file - the pointer to the file that we are checking
 *    final_name - the final name of the file that we are setting in the function
 *    file_name - the name of the file that we are getting
 *    allocation_faild_flag - the flag that indicates if there is an allocation error 
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error to open the file
 *    THERE_IS_AN_ERROR - if there is an error to open the file
 */
errors open_files_second_pass(FILE **input_file, char **final_name, char *file_name, boolean *allocation_faild_flag){
    get_the_full_name_of_file(final_name, file_name, "am"); /* get the full name of the input file with the extension after we add the correct extension to the name of the file */
    *input_file = fopen(*final_name, "r"); /* open the input file just for reading */
    if (check_file(*input_file,*final_name) == THERE_IS_AN_ERROR) { /* check if there is an error to open the file*/
        finish_second_pass(*input_file, *final_name); /* close the file and free the final name of the file */
        *allocation_faild_flag = True; /* set the allocation flag */
        return THERE_IS_AN_ERROR; /* return that there is an error */
    } 
    return THERE_IS_NO_ERROR; /* return that there is no error */
}


/**
 * this function handles the entry guidance at the second pass, and check if there is an error in the entry guidance like:
 * 1. the label is defined but not declared
 * 2. the label is defined as entry and also as external in other line
 * or if there is no error, the function will set the type of the label to be entry
 * 
 * parameters:
 *    line - the current line that we are reading from the file
 *    file_name - the name of the file that we are reading from
 *    number_line - the number of the current line that we are reading from the file
 *    error - the pointer to the error flag
 *    table - the table of labels
 * 
 */
void handle_entry_guidance_at_second_pass(char line[], char *file_name, int number_line, errors *error, table_of_labels *table){
    char label_name[MAX_CHARS_IN_LABEL];
    strtok(line, " \t\n\r\v\f"); /* skip the '.entry' */
    strcpy(label_name, strtok(NULL, " \t\n\r\v\f")); /* get the label name */
    if (check_errors_of_entry_guidance_second_pass(label_name, file_name, number_line, table) == THERE_IS_AN_ERROR){ /* check if there is an error in the entry guidance */
        *error = THERE_IS_AN_ERROR; /* set the error flag */
    }
    else{
        set_type_of_label(label_name, entry, table); /* set the type of the label to be entry */
    }
}


/**
 * this function handles the instruction at the second pass, and check if there is using a label, and if so it checks if there is an error in the label like:
 * 1. the label is not defined 
 * and if there is no error, the function will encode the label according to the address of the label that the label defined
 * 
 * parameters:
 *    instruction_array - the array that we are storing the machine code of the instructions line encoded
 *    line - the current line that we are reading from the file
 *    file_name - the name of the file that we are reading from
 *    number_line - the number of the current line that we are reading from the file
 *    IC - the instruction counter
 *    error - the pointer to the error flag
 *    table - the table of labels
 * 
 */
void handle_instruction_at_second_pass(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char line[], char *file_name, int number_line, int *IC, errors *error, table_of_labels *table){
    char *first_operand_name, *second_operand_name;
    int adress_of_first_operand, adress_of_second_operand;
    operand_type type_of_first_operand_in_the_instruction_line, type_of_second_operand_in_the_instruction_line;
    (*IC)++; /* increase the IC by 1 because if we need to update the encoding or not, the first word that is encoded is not relevant */
    type_of_first_operand_in_the_instruction_line = get_parameter_type(line, FIRST_PARAMETER); /* get the type of the first operand in the instruction line (if there is) */
    type_of_second_operand_in_the_instruction_line = get_parameter_type(line, SECOND_PARAMETER); /* get the type of the second operand in the instruction line (if there is)*/
    strtok(line, " ,\t\n\r\f\v"); /* skip the first word */
    first_operand_name = strtok(NULL, " ,\t\n\r\f\v"); /* get the first operand */
    second_operand_name = strtok(NULL, " ,\t\n\r\f\v"); /* get the second operand */
    if (type_of_first_operand_in_the_instruction_line == label){ /* if the first operand is a related as label */
            if (check_errors_of_label_operand_of_instruction_line(first_operand_name, file_name, number_line, table) == THERE_IS_NO_ERROR){ /* check if there is an error in the label operand */
                adress_of_first_operand = get_adress_of_label(first_operand_name, table); /* get the address of the label that the label defined */
                
                encode_label(instruction_array, adress_of_first_operand, *IC, get_type_of_label(first_operand_name, table)); /* encode the label */
            }
            else{
                 *error = THERE_IS_AN_ERROR; /* set the error flag */}
    }
    if (first_operand_name != NULL){(*IC)++;} /* if the first operand is not NULL, we need to increase the IC by 1 */

    if (type_of_second_operand_in_the_instruction_line == label){
           if (check_errors_of_label_operand_of_instruction_line(second_operand_name, file_name, number_line, table) == THERE_IS_NO_ERROR){ /* check if there is an error in the label operand */
                adress_of_second_operand = get_adress_of_label(second_operand_name, table);
                encode_label(instruction_array, adress_of_second_operand, *IC, get_type_of_label(second_operand_name, table)); /* encode the label */
            }
            else{
                 *error = THERE_IS_AN_ERROR; /* set the error flag */}
    }

    if (second_operand_name != NULL){(*IC)++;} /* if the second operand is not NULL, we need to increase the IC by 1 */
    if (type_of_first_operand_in_the_instruction_line == reg && type_of_second_operand_in_the_instruction_line == reg){ /* if the two operands are registers */
        (*IC)--; /* if the instruction has two registers, we need to decrease the IC by 1 */
    }
}


/**
 * this function frees all the memory and close all the files
 * 
 * parameters:
 *    file - the file that we are closing
 *    final_name - the final name of the file that we are freeing
 * 
 */
void finish_second_pass( FILE *file, char *final_name){
    if (file != NULL){ /* if the file is not NULL, we need to close it */
        fclose(file); 
    }
    if (final_name != NULL){ /* if the final name is not NULL, we need to free it */
        free(final_name);
    }
}