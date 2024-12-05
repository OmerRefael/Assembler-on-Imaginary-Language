/**
 * @author Omer Refael
 * @version 16/8/2024 
 *
 * This file contains the main function for the assembler program, which oversees the entire assembly process. 
 * The assembler converts source code written in assembly language into executable machine code.
 * the program manages command-line arguments, processes files, and coordinates the various stages of the assembly process. 
 * This includes the initial and subsequent phases of translation (first and second passes), errors and warnings management, and the creation of output files.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "pre_assembler/pre_assembler.h"
#include "pre_assembler/macros_structure/table_of_macros.h"
#include "first_pass/first_pass.h"
#include "labels_structure/table_of_labels.h"
#include "second_pass/second_pass.h"
#include "enums/errors.h"
#include "enums/boolean.h"
#include "errors_handling/print_messages.h"
#include "util/create_output_files.h"
#include "util/help_methods.h"

#define MINIMUM_NUMBER_OF_ARGUMENTS  2 /* minimum number of arguments that the user should enter (less then it, means that he didnt enter any file as argument) */
#define NOT_IN_SPECIFIC_LINE 0 /* the error that happened, is not in a specific line, so we will not print the line number */
#define NOT_IN_SPECIFIC_FILE NULL /* the error that happened, is not in a specific file, so we will not print the file name */
#define NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY 2 /* we will use 2 bytes for representing each encoded line (15 bits of data + 1 parity bit) */
#define MAX_LENGTH_OF_DATE_LINES 4096 /* we assume that the maximum length of the data lines is 4096 */
#define MAX_LENGTH_OF_INSTRUCTION_LINES 4096 /* we assume that the maximum length of the instruction lines is 4096 */
#define START_VALUE_OF_IC 0 /* the initial value of the IC */
#define START_VALUE_OF_DC 0 /* the initial value of the DC */
#define EXIT_FAILURE 1 /* the exit code for the program if there is an allocation error */


/**
 * this function frees the table of macros and the table of labels
 * 
 * parameters:
 *    macros_table - the table of macros that we want to free
 *    labels_table - the table of labels that we want to free
 * 
 */
void finish_the_assembler_program(table_of_macros *, table_of_labels *);


/** 
 * this function is the main function of the assembler program, and it is responsible for the flow of the program and the call of the other functions that process the files that the user entered. 
 * 
 * note: We are told in the forum that the size of the data set and the instruction set is constant.
 * So I defined the amount of cells (row length) of the data and the instructions arrays to be 4096 (when it start from 100 which means that the maximum number of lines in the file is 3996).
 * It is known that the total memory size is 4096 cells, and since we were not told how the memory is divided, then I defined the amount of cells of each array individually to be 4096 (again as mentioned, because I do not know how the division is carried out).
 * But I check after each encoding line that there is no deviation from the memory size, and that the sum of the used cells of the data and the instruction arrays does not exceed from 3996 together (i defined the size of array to be higher cause to pervent overflow in every case).
 * 
 * another note: we need the macros table also for the first pass, for checking if the label name is a macro name or not, so we will send it to the first pass function,
 * and there we will check it (according to the instructions that we got in the forum (it said that it is okay to do it in the first pass))
 */
int main(int argc, char *argv[]) {
    int i; /* index for the loop */
    int IC, DC; /* the instruction counter and the data counter */
    char instruction_array[MAX_LENGTH_OF_INSTRUCTION_LINES][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], data_array[MAX_LENGTH_OF_DATE_LINES][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY]; /* the arrays that will hold the encoded lines */
    char *file_name; /* the name of the file that the user entered */
    table_of_macros *macros_table; /* the table of macros */
    table_of_labels *labels_table; /* the table of labels */
    boolean allocation_faild_flag = False; /* for indicate if there is an allocation error */
    if (argc < MINIMUM_NUMBER_OF_ARGUMENTS) { /* if the user didnt enter any file as argument */
        print_errors(NO_ARGUMENTS_FOR_THE_PROGRAM, NOT_IN_SPECIFIC_LINE, NOT_IN_SPECIFIC_FILE); /* print an error message */
    }
    for (i = 1; i < argc; i++) { /* loop over all the files that the user entered */
        IC = START_VALUE_OF_IC;  /* initialize the IC */
        DC = START_VALUE_OF_DC; /* initialize the DC */
        macros_table = NULL; /* the table of macros */
        labels_table = NULL; /* the table of labels */
        file_name = argv[i]; /* get the name of the file */
        initialize_data_and_instruction_arrays(instruction_array, data_array); /* initialize the arrays that will hold the encoded lines */
        if (pre_proccess(file_name, &macros_table, &allocation_faild_flag) == THERE_IS_NO_ERROR) { /* if the file is pre-processed successfully */
            if (first_pass(instruction_array, data_array, file_name, &IC, &DC, &labels_table, macros_table, &allocation_faild_flag) == THERE_IS_NO_ERROR && allocation_faild_flag == False) { /* if the first pass is done successfully, and there is no allocation error */
                if (second_pass(instruction_array, file_name, labels_table, &allocation_faild_flag) == THERE_IS_NO_ERROR && allocation_faild_flag == False) { /* if the second pass is done successfully, and there is no allocation error */
                    if (create_output_files_after_assembler_processing(instruction_array, data_array,file_name, IC, DC, labels_table, &allocation_faild_flag) == THERE_IS_NO_ERROR && allocation_faild_flag == False) { /* if the output files are created successfully, and there is no allocation error */
                        print_errors(THERE_IS_NO_ERROR, NOT_IN_SPECIFIC_LINE, file_name); /* print the messages that the program finished successfully */
                    }
                }
            }
        }
        finish_the_assembler_program(macros_table, labels_table); /* free all the memory that we allocated in the program for the current file */
        if (allocation_faild_flag == True) { /* if there is an allocation error */
            exit(EXIT_FAILURE); /* exit the program , because we can not continue the program */
        }
    }
    return 0; 
}


/**
 * this function frees the table of macros and the table of labels
 * 
 * parameters:
 *    macros_table - the table of macros that we want to free
 *    labels_table - the table of labels that we want to free
 * 
 */
void finish_the_assembler_program(table_of_macros *macros_table, table_of_labels *labels_table){
    free_table_of_macros(macros_table); /* free the table of macros */
    free_table_of_labels(labels_table); /* free the table of labels */
}