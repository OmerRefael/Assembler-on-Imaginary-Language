#ifndef CHECK_ERRORS_IN_SECOND_PASS_H
#define CHECK_ERRORS_IN_SECOND_PASS_H

#include "../enums/errors.h"
#include "../labels_structure/table_of_labels.h"

/**
 * this function checks if there is an error in the entry guidance line (error that can be checked just in the second pass)
 * 
 * parameters:
 *     label_name - the name of the label of the entry guidance
 *     file_name - the name of the file that the entry guidance is in
 *     number_line - the number of the line that the entry guidance is in
 *     table - the table of labels
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if the label of the entry guidance is defined
 *    THERE_IS_AN_ERROR - if the label of the entry guidance is not defined
 * 
 */
errors check_errors_of_entry_guidance_second_pass(char [], char *, int, table_of_labels *);
/**
 * this function checks if there is an error in the label operand of the instruction line (error that can be checked just in the second pass)
 * 
 * parameters:
 *     label_name - the name of the label of the operand
 *     file_name - the name of the file that the instruction line is in
 *     number_line - the number of the line that the instruction line is in
 *     table - the table of labels
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if the label of the operand is defined
 *    THERE_IS_AN_ERROR - if the label of the operand is not defined
 * 
 */
errors check_errors_of_label_operand_of_instruction_line(char [], char *, int , table_of_labels *);
#endif
