#ifndef HELP_METHODS_FOR_FIRST_PASS_H
#define HELP_METHODS_FOR_FIRST_PASS_H

#include "../enums/boolean.h"
#include "../enums/commands.h"
#include "../enums/operand_type.h"
#include "../enums/guidance.h"
#include "../enums/registers.h"
#include "../enums/address_method.h"


/**
 * this function checks if the word can be a label (according to the syntax rules of labels)
 * 
 * parameters:
 *     word - the word that we want to check if it can be a label
 * 
 * returns:
 *    True - if the word passed the syntax rules of the label, and it can be a label
 *    False - if the word did not pass the syntax rules of the label, and it can't be a label
 * 
 */
boolean is_can_be_label(char *);
/**
 * this function checks if the in the line there is a label definition
 * 
 * parameters:
 *     line - the line that we want to check if it is a label definition
 * 
 * returns:
 *    True - if the line has a label definition
 *    False - if the line is not has a label definition
 * 
 */
boolean is_label_definition(char []);
/**
 * this function checks if the line has a second word
 * 
 * parameters:
 *     line - the line that we want to check if it has a second word
 * 
 * returns:
 *    True - if the line has a second word
 *    False - if the line does not have a second word
 * 
 */
boolean has_second_word(char []);
/**
 * this function returns the type of the command according passed parameter
 * 
 * parameters:
 *    command - the string that we want to check its type of command
 * 
 * returns:
 *   the type of the command according (and if it is not a command, it returns not_command)
 * 
 */
commands get_command_type(char *);
/**
 * this function returns the type of the operand according to the line and the operand number
 * 
 * parameters:
 *    line - the line that we want to check its operand type
 *    operand_number - the number of the operand that we want to check its type
 * 
 * returns:
 *   the type of the operand according (and if it is not an operand, it returns not_operand)
 * 
 */
operand_type get_parameter_type(char [], int );
/**
 * this function returns the type of the guidance according to the line
 * 
 * parameters:
 *    line - the line that we want to check its type of guidance
 * 
 * returns:
 *   the type of the guidance according (and if it is not a guidance, it returns no_guidance)
 * 
 */
guidance get_guidance_type(char []);
/**
 * this function returns the type of the register according to the passed parameter
 * 
 * parameters:
 *    reg - the string that we want to check its type of register
 * 
 * returns:
 *   the type of the register according (and if it is not a register, it returns not_register)
 * 
 */
registers get_register(char *);
/**
 * this function returns the type of the address method according to the passed operand
 * 
 * parameters:
 *    operand - the operand that we want to check its type of address method
 * 
 * returns:
 *   the type of the address method according 
 * 
 */
address_method get_address_method(char [], int );
/**
 * this function returns the index of the first quote in the line
 * 
 * parameters:
 *     line - the line that we want to get the index of the first quote
 * 
 * returns:
 *    the index of the first quote in the line (if there is no quote in the line, it returns -1)
 * 
 */
int get_the_index_of_the_first_quote(char []);
/**
 * this function returns the index of the last quote in the line
 * 
 * parameters:
 *     line - the line that we want to get the index of the last quote
 * 
 * returns:
 *    the index of the last quote in the line (if there is no quote in the line, it returns -1)
 * 
 */
int get_the_index_of_the_last_quote(char []);
/**
 * this function returns the amount of operands that need to be according to the command name
 * note: the function assumes that the line the first word is indeed a command (we checked it before we called the function)
 * 
 * parameters:
 *     line - the line that we want to check the amount of operands that need to be according to the command 
 * 
 * returns:
 *    the amount of operands that need to be according to the command
 * 
 */
int get_amount_of_operands_that_need_to_be(char []);
/**
 * converts a positive number to its negative counterpart using the two's complement representation in a 15 bit .
 * 
 * parameters:
 *    number - the number that we are converting to its negative counterpart
 * 
 * returns:
 *   the negative version of the number according to the two's complement representation in 15 bits
 */
int handle_negative_number(int );
/**
 * this function change the line to be without the label definition
 * 
 * parameters:
 *     line - the line that we want to change to be without the label definition
 * 
 */
void change_the_line_to_be_without_label_definition(char *);

#endif