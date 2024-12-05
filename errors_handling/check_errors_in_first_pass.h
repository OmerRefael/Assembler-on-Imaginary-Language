#ifndef CHECK_ERRORS_IN_FIRST_PASS_H
#define CHECK_ERRORS_IN_FIRST_PASS_H

#include "../enums/errors.h"
#include "../enums/boolean.h"
#include "../labels_structure/table_of_labels.h"
#include "../pre_assembler/macros_structure/table_of_macros.h"

/**
 * this function checks if the string parameter is valid, according to the rules of the string guidance:
 * 1. the parameter is not missing.
 * 2. there is a open quote in the line.
 * 3. there is a close quote in the line.
 * 4. the string parameter is not empty.
 * 5. there is no text before the string parameter.
 * 6. there is no text after the string parameter.
 * 
 * 
 * parameters:
 *    line - the string guidance line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error in the string guidance line.
 *    THERE_IS_AN_ERROR - if there is an error in the string guidance line.
 */
errors check_errors_in_string_guidance(char [], char *, int );
/**
 * this function checks if the data parameter is valid, according to the rules of the data guidance:
 * 1. there is at least one parameter.
 * 2. the commas placement is correct.
 * 3. the values and the types of the data members are valid.
 * 
 * 
 * parameters:
 *    line - the data guidance line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error in the data guidance line.
 *    THERE_IS_AN_ERROR - if there is an error in the data guidance line.
 */
errors check_errors_in_data_guidance(char [], char *, int );
/**
 * this function checks if there is an error in the entry or extern guidance line if the amount of operands is correct (just one operand)
 * 
 * note: all the rules of defined label are also applied to the entry and extern labels, but according to the project's instructions, for extern guidance we dont need to check if there are errors in the name of the label
 * and for the entry guidance, we will check the content where it is defined (and not where it is declared) (and if it is just decleared, but not defined, we will check it in the second pass).
 * 
 * parameters:
 *    line - the entry or extern guidance line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.s
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error in the entry or extern guidance.
 *    THERE_IS_AN_ERROR - if there is an error in the entry or extern guidance.
 */
errors check_errors_in_entry_and_extern_guidance(char [], char *, int);
/**
 * this function checks if there is an error in the instruction line according to the rules of the instruction line:
 * 1. the first word is a command.
 * 2. the amount of operands is correct.
 * 3. the values of the operands are valid.
 * 4. the addressing method for the source operand of the command is correct.
 * 5. the addressing method for the destination operand of the command is correct.
 * 
 * parameters:
 *    line - the instruction line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error in the instruction line.
 *    THERE_IS_AN_ERROR - if there is an error in the instruction line.
 */
errors check_errors_in_instruction_line(char [], char *, int );
/**
 * this function checks if there is an error in the label line according to the rules of the label:
 * 1. the label name is not empty.
 * 2. the label name is not too long.
 * 3. the label name is not a save word.
 * 4. the label name starts with a letter.
 * 5. there is a space after the label definition (if it a label defintion and is not in the entry or extern guidance line).
 * 6. the label name contains just letters and digits.
 * 7. there is no other labels with the same name.
 * 8. the label name is not a name of a macro.
 * 
 * parameters:
 *    line - the label line that we want to check.
 *    label_name - the name of the label that we want to check.
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 *    labels_table - the table of labels
 *    macros_table - the table of macros
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error in the label.
 *    THERE_IS_AN_ERROR - if there is an error in the label.
 */
errors check_errors_in_label(char [],char [], char *, int , table_of_labels *, table_of_macros *);
/**
 * this function checks if there is a ecxceeding memory limit in the program (the sum of the IC and DC is greater than the maximum memory limit)
 * 
 * parameters:
 *    file_name - the name of the file that we are checking.
 *    IC - the instruction counter
 *    DC - the data counter
 *    is_memory_exceeded - the memory exceeded flag
 *    error - the error flag
 */
void check_exceeding_memory_limit(char *, int , int, boolean *, errors *);


#endif