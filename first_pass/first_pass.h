#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "../enums/errors.h"
#include "../enums/boolean.h"
#include "../labels_structure/table_of_labels.h"
#include "../pre_assembler/macros_structure/table_of_macros.h"


#define NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY 2

/**
 * this function performs the first pass of the assembler on the given file. it checks for errors in the file,
 * encodes the file into machine code, and stores the machine code of the instructions and data in the provided arrays.
 * additionally, it handles the labels.
 * 
 * parameters:
 *    instruction_array - the array that we store the machine code of the instructions
 *    data_array - the array that we store the machine code of the data
 *    file_name - the name of the file that we are checking
 *    IC - the instruction counter
 *    DC - the data counter
 *    labels_table - the table of labels that we will store the labels in it
 *    macros_table - the table of macros that we will store the macros in it
 *    allocation_faild_flag - the flag that indicates if there is an allocation error  
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error in the first pass
 *    THERE_IS_AN_ERROR - if there is an error in the first pass
 */
errors first_pass(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY],char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY],char *, int *, int *, table_of_labels **, table_of_macros *, boolean *);

#endif