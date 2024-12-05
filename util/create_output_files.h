#ifndef CREATE_OUTPUT_FILES_H
#define CREATE_OUTPUT_FILES_H

#include "../enums/errors.h"
#include "../enums/boolean.h"
#include "../labels_structure/table_of_labels.h"

#define NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY 2

/**
 * this function creates the object file after we finish to proccess the file, and sometimes create the entry file and the extern file if they are needed (if these files wont be empty)
 * 
 * parameters:
 *    instruction_array - the array of the instruction lines
 *    data_array - the array of the data lines
 *    file_name - the name of the file
 *    IC - the instruction counter
 *    DC - the data counter
 *    table_of_labels - the table of the labels
 *    allocation_faild_flag - the flag that indicates if there is an allocation error 
 * 
 * return:
 *   THERE_IS_AN_ERROR - if there is an error while creating the files
 *   THERE_IS_NO_ERROR - if there is no error while creating the files
 */
errors create_output_files_after_assembler_processing(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY],char *, int , int, table_of_labels *, boolean *);

#endif