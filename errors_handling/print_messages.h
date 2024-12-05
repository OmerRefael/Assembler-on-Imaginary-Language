#ifndef PRINT_MESSAGES_H
#define PRINT_MESSAGES_H

#include "../enums/errors.h"
#include "../enums/warnings.h"

/**
 * this function prints the error message according to the error that happened and the line number and the file name (if the error is in a specific line or file)
 * 
 * parameters:
 *     error - the error that happened
 *     line_number - the number of the line that the error happened (if the error is not in a specific line, the value will be 0 - we start to count from 1 the lines)
 *     file_name - the name of the file that the error happened (if the error is not in a specific file, the value will be NULL)
 * 
 */
void print_errors(errors , int ,char *);
/**
 * this function prints the warning message according to the warning that happened and the line number and the file name (if the warning is in a specific line or file)
 * 
 * parameters:
 *     warning - the warning that happened
 *     line_number - the number of the line that the warning happened (if the warning is not in a specific line, the value will be 0 - we start to count from 1 the lines)
 *     file_name - the name of the file that the warning happened (if the warning is not in a specific file, the value will be NULL)
 * 
 */
void print_warnings(warnings warning, int , char *);

#endif