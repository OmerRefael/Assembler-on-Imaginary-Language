#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "../enums/errors.h"
#include "../labels_structure/table_of_labels.h"

#define NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY 2

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
errors second_pass(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char *, table_of_labels *, boolean *);

#endif