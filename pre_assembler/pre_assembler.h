#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include "macros_structure/table_of_macros.h"
#include "../enums/errors.h"
#include "../enums/boolean.h"

/**
 * this function pre-processes the file that the user entered and lays out the macros that were defined in the file. 
 * 
 * the function gets the name of the file that the user entered and adds the correct extension to the name of the file for opening the file. 
 * after it, the function reads the file line by line, and if the line is a macro call, the function writes the content of the macro to the output file that the function creates.
 * if there is an error in the file, the function will print the error, the line, and the name of the file where the error happened, and we don't continue to the next passes.
 * 
 * parameters:
 *    file_name - the name of the file that the user entered (without the extension)
 *    macros_table - the table of macros that we are going to update
 *    allocation_faild_flag - the flag that indicates if there is an allocation error
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error in the file
 *    THERE_IS_AN_ERROR - if there is an error in the file
 * 
 */
errors pre_proccess(char *,table_of_macros **, boolean *);

#endif