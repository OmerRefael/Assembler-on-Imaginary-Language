#ifndef HELP_METHODS_H
#define HELP_METHODS_H

#define NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY 2 /* The number of columns in the instruction and data arrays */

#include <stdio.h>
#include "../enums/errors.h"
#include "../enums/boolean.h"

/**
 * this function checks if the allocation successed - if the pointer that we got is NULL, if it is NULL, the function will print an error message and return an error.
 * 
 * parameters:
 *     p - the pointer that we want to check if it is NULL
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if the pointer is not NULL
 *    THERE_IS_AN_ERROR - if the pointer is NULL
 * 
 */
errors check_allocation(void *);
/**
 * this function checks if the file that we can open the file , if we can not open it , the function will print an error message and return an error.
 * 
 * parameters:
 *     file - the file that we want to check if it can be opened
 *     filename - the name of the file that we want to check if it exists
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if the file open successfully
 *    THERE_IS_AN_ERROR - if the file doesn't open successfully
 * 
 */
errors check_file(FILE *, char *);
/**
 * this function sets the full name of the file (the name of the file + the extension of the file)
 * 
 * parameters:
 *     final_name - the pointer to the final name of the file
 *     file_name - the name of the file that we want to get the full name of the file
 *     file_extension - the extension of the file that we want to add to the name of the file
 * 
 * returns:
 *   THERE_IS_NO_ERROR - if there is no error in the function(especialy in the allocation)
 *   THERE_IS_AN_ERROR - if there is an error in the function(especialy in the allocation)
 */
errors get_the_full_name_of_file(char **, char *, char *);
/**
 * this function checks if the line is too long
 * 
 * parameters:
 *     line - the line that we want to check if it is too long or not
 * 
 * returns:
 *    True - if the line is too long then the maximum length of the line
 *    False - if the line is not too long then the maximum length of the line
 * 
 */
boolean is_line_too_long(char *);
/**
 * this function checks if the line is a comment or an empty line
 * 
 * parameters:
 *     line - the line that we want to check if it is a comment or an empty line
 * 
 * returns:
 *    True - if the line is a comment or an empty line
 *    False - if the line is not a comment or an empty line
 * 
 */
boolean is_not_relevent_line(char *);
/**
 * this function checks if the word is a save word
 * 
 * parameters:
 *     word - the word that we want to check if it is a save word
 * 
 * returns:
 *    True - if the word is a save word
 *    False - if the word is not a save word
 * 
 */
boolean is_a_save_word(char *);
/**
 * this function checks if the first character of the word is a letter
 * 
 * parameters:
 *     word - the word that we want to check if the first character is a letter
 * 
 * returns:
 *    True - if the first character of the word is a letter
 *    False - if the first character of the word is not a letter
 * 
 */
boolean check_if_first_char_is_letter(char *);
/**
 * this function checks if the word(probably the name of a label or macro) has illegal characters  
 * note: the name of the macro can contain the character '_', in contrast to the name of the label
 * note: the first character of the word must be a letter (and we check it in other function)
 * 
 * parameters:
 *    word - the word that we want to check if it has illegal characters
 *    is_macro - the flag that indicates if the word is a macro name
 * 
 * returns:
 *    True - if the word has illegal characters
 *    False - if the word does not have illegal characters
 * 
 */
boolean check_if_word_contains_illegal_chars(char *, boolean);
/**
 * this function returns the index of where the first word ends in the line (one after the last char of the first word)
 * 
 * parameters:
 *     line - the line that we want to get the index of the end of the first word
 * 
 * returns:
 *    the index of where the first word ends in the line (one after the last char of the first word)
 * 
 */
int get_index_after_first_word(char []);
/**
 * this function returns the index of the beginning of the second word in the line
 * 
 * parameters:
 *     line - the line that we want to get the index of the beginning of the second word
 * 
 * returns:
 *    the index of the beginning of the second word in the line
 * 
 */
int get_index_of_second_word(char []);
/**
 * this function initializes the data and instruction arrays to be empty
 * 
 * parameters:
 *     instruction_array - the array that will store the instructions encoded lines
 *     data_array - the array that will store the data encoded lines
 * 
 */
void initialize_data_and_instruction_arrays(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char data_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY]);
/**
 * this function converts the machine code to octal base
 * 
 * parameters:
 *     machine_code - the machine code that we want to convert to octal base
 *     octal_word - the octal word that we want to store the octal base
 *     address - the address of the machine code that we want to convert to octal base
 * 
 */
void convert_machine_code_to_octal_base(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char *, int );

#endif