#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "check_errors_in_first_pass.h"
#include "print_messages.h"
#include "../enums/errors.h"
#include "../enums/boolean.h"
#include "../enums/guidance.h"
#include "../enums/commands.h"
#include "../enums/registers.h"
#include "../enums/address_method.h"
#include "../first_pass/help_methods_for_first_pass.h"
#include "../util/help_methods.h"
#include "../labels_structure/table_of_labels.h"
#include "../pre_assembler/macros_structure/table_of_macros.h"

#define MAX_CHARS_IN_LINE 82 /* the maximum number of chars in line + 1 (for the '\0') when defining the array */
#define MAX_MEMORY_LIMIT 3996 /* the maximum number of cells that the memory has in the program (it is from  start from 100 to 4096) */
#define MIN_AMOUNT_OF_PARAMETERS_IN_STRING_GUIDANCE 1 /* the min amount of parameters in the string guidance */
#define MIN_AMOUNT_OF_PARAMETERS_IN_DATA_GUIDANCE 1 /* the min amount of parameters in the data guidance */
#define MIN_VALUE_NUMBER_OF_DATA_GUIDANCE -16384 /* -2^14 - the minimum value of number that can be represented in 15 bits of data guidance parameter */
#define MAX_VALUE_NUMBER_OF_DATA_GUIDANCE 16383 /* 2^14-1 - the maximum value of number that can be represented in 15 bits of data guidance parameter */
#define AMOUNT_OF_PARAMETERS_IN_ENTRY_OR_EXTERN_GUIDANCE 1 /* the amount of parameters in the entry or extern guidance */
#define FIRST_CHAR 0 /* the index of the first char in the line */
#define ONE_OPERAND 1 /* the amount of operands that need to be in the line */
#define TWO_OPERANDS 2 /* the amount of operands that need to be in the line */
#define FIRST_PARAMETER 1 /* the index of the first parameter in the line */
#define SECOND_PARAMETER 2 /* the index of the second parameter in the line */
#define MAX_LENGTH_OF_LABEL_NAME 31 /* the maximum length of the label name */
#define MIN_VALUE_OF_IMMEDIATE_NUMBER_IN_INSTRUCTION_LINE -2048 /* -2^11 - the minimum value of number that can be represented in 12 bits of immediate number in instruction line */
#define MAX_VALUE_OF_IMMEDIATE_NUMBER_IN_INSTRUCTION_LINE 2047 /* 2^11-1 - the maximum value of number that can be represented in 12 bits of immediate number in instruction line */
#define NOT_IN_SPECIFIC_LINE 0 /* The error that happened, is not in a specific line, so we will not print the line number (we start to count from 1 the lines) */


/* --------------- PROTOTYPES --------------- */

/* string guidance */
errors check_if_missing_open_quote_in_the_string_parameter(char [], char *, int);
errors check_if_missing_close_quote_in_the_string_parameter(char [], char *, int);
errors check_if_string_parameter_is_empty(char [], char *, int);
errors check_if_there_it_is_extra_text_before_string_parameter(char [], char *, int);
errors check_if_there_it_is_extra_text_after_string_parameter(char [], char *, int);

/* data guidance */
errors check_commas_placement_for_data_guidance_line(char [], char *, int);
errors check_valid_values_of_data_guidance_line(char [], char *, int);
boolean check_valid_type_of_member_in_data_guidance(char *);
boolean check_valid_value_of_member_in_data_guidance(char *);

/* entry and extern guidance */
errors check_amount_of_operands_of_entry_or_or_extern_guidance(char [], char *, int);

/* instruction line */
errors check_commas_placement_for_instruction_line(char [], char *, int);
errors is_defined_commend(char [], char *, int);
errors check_amount_of_operands_in_instruction_line(char [], char *, int, int);
errors check_values_of_operands_of_instruction_line(char [], char *, int , int);
errors check_valid_value_of_operand(char *, char *, int);
errors check_immediate_addressing_operand(char *, char *, int);
errors is_correct_adrrsing_method_for_source_operand_of_the_command(char [], char *, int , int);
errors is_correct_adrrsing_method_for_destination_operand_of_the_command(char [], char *, int , int);
errors check_addressing_method_for_source_operand_of_two_operands(char [], char *, int ,commands);
errors check_addressing_method_for_destination_operand_of_two_operands(char [], char *, int , commands);
errors check_addressing_method_for_destination_operand_of_one_operand(char [], char *, int , commands);

/* label */
boolean is_empty_label_name(char []);
boolean has_space_after_label_definition(char []);
boolean is_name_of_label_contains_illegal_chars(char []);

/* general */
errors check_missing_parameters(char [], char *, int, int, guidance);
errors check_commas_placement(char [], char *, int , int);
errors check_commas_placement_in_first_part_of_sentence(char [], int);
errors check_commas_placement_in_last_part_of_sentence(char [], int);

/* --------------- END OF PROTOTYPES --------------- */





/* --------------- CHECKING STRING GUIDANCE ERRORS --------------- */

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
errors check_errors_in_string_guidance(char line[], char *file_name, int number_line){
    if (check_missing_parameters(line, file_name, MIN_AMOUNT_OF_PARAMETERS_IN_STRING_GUIDANCE, number_line, string_guidance) == THERE_IS_AN_ERROR){ /*if there is missing parameters in the string guidance*/
        return THERE_IS_AN_ERROR; /*return that there is an error*/
    }
    if (check_if_missing_open_quote_in_the_string_parameter(line, file_name, number_line) == THERE_IS_AN_ERROR){ /*if there is open quote in the line is missing*/
        return THERE_IS_AN_ERROR; /*return that there is an error*/
    }
    if (check_if_missing_close_quote_in_the_string_parameter(line, file_name, number_line) == THERE_IS_AN_ERROR){ /*if the there is missing close quote in the line*/
        return THERE_IS_AN_ERROR; /*return that there is an error*/
    }
    if (check_if_string_parameter_is_empty(line, file_name, number_line) == THERE_IS_AN_ERROR){ /*if string parameter is empty*/
        return THERE_IS_AN_ERROR; /*return that there is an error*/
    }
    if (check_if_there_it_is_extra_text_before_string_parameter(line, file_name, number_line) == THERE_IS_AN_ERROR){ /*if there is a text before the string parameter*/
        return THERE_IS_AN_ERROR; /*return that there is an error*/
    }
    if (check_if_there_it_is_extra_text_after_string_parameter(line, file_name, number_line) == THERE_IS_AN_ERROR){ /*if there is a text after the string parameter*/
        return THERE_IS_AN_ERROR; /*return that there is an error*/
    }
    return THERE_IS_NO_ERROR; /*return that there is no error*/
}


/**
 * this function checks if the open quote is missing in the string parameter
 * 
 * parameters:
 *    line - the string guidance line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and the open quote is not missing.
 *    THERE_IS_AN_ERROR - if there is an error and the open quote is missing.
 */
errors check_if_missing_open_quote_in_the_string_parameter(char line[], char *file_name, int number_line){
    int i;
    for (i = 0; i < strlen(line); i++){
        if (line[i] == '"'){
            return THERE_IS_NO_ERROR; /*return that there is no error because the open quote is not missing*/
        }
    }
    print_errors(MISS_OPEN_QUOTE_IN_STRING_PARAMETER, number_line, file_name); /*print the error message that there is missing quote in the start of the string parameter*/
    return THERE_IS_AN_ERROR; /*return that there is an error because the open quote is missing*/
}


/**
 * this function checks if the close quote is missing in the string parameter
 * 
 * parameters:
 *    line - the string guidance line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and the close quote is not missing.
 *    THERE_IS_AN_ERROR - if there is an error and the close quote is missing.
 */
errors check_if_missing_close_quote_in_the_string_parameter(char line[], char *file_name, int number_line){
    int index_of_the_first_quote = get_the_index_of_the_first_quote(line); /*get the index of the first quote in the line*/
    int index_of_the_last_quote = get_the_index_of_the_last_quote(line); /*get the index of the last quote in the line*/
    if (index_of_the_first_quote == index_of_the_last_quote){ /*if the first quote and the last quote are the same - there is no close quote in the line*/
        if (isspace(line[index_of_the_last_quote -1]) == False){ /*mean that there it a line like: .string hello" - so means the open quote is missing*/
            print_errors(MISS_OPEN_QUOTE_IN_STRING_PARAMETER, number_line, file_name); /*print the error message that there is missing quote in the end of the string parameter*/
        }
        else{
            print_errors(MISS_CLOSE_QUOTE_IN_STRING_PARAMETER, number_line, file_name); /*print the error message that there is missing quote in the end of the string parameter*/
        }
        return THERE_IS_AN_ERROR; /*return that there is an error because the close quote is missing*/
    }
    return THERE_IS_NO_ERROR; /*return that there is no error because the close quote is not missing*/
}


/**
 * this function checks if the string parameter is empty
 * 
 * parameters:
 *    line - the string guidance line that we want to check.
 * 
 * returns:
 *    True - if the string parameter is empty.
 *    False - if the string parameter is not empty.
 */
errors check_if_string_parameter_is_empty(char line[], char *file_name, int number_line){
    int index_of_the_first_quote = get_the_index_of_the_first_quote(line); /*get the index of the first quote in the line*/
    int index_of_the_last_quote = get_the_index_of_the_last_quote(line); /*get the index of the last quote in the line*/
    if (index_of_the_first_quote == index_of_the_last_quote - 1){ /*if there is no text between the quotes*/
        print_errors(EMPTY_STRING_PARAMETER, number_line, file_name); /*print the error message that the string parameter is empty*/
        return THERE_IS_AN_ERROR; /*return that there is an error because the string parameter is empty*/
    }
    return THERE_IS_NO_ERROR; /*return that there is no error because the string parameter is not empty*/
}


/**
 * this function checks if there is extra text before the string parameter
 * 
 * parameters:
 *    line - the string guidance line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and there is no text before the string parameter.
 *    THERE_IS_AN_ERROR - if there is an error and there is text before the string parameter.
 */
errors check_if_there_it_is_extra_text_before_string_parameter(char line[], char *file_name, int number_line){
    int i = get_index_after_first_word(line); /*get the index after the guidance name*/
    for (; i < get_the_index_of_the_first_quote(line); i++){ /*go through the line until the first quote*/
        if (isspace(line[i]) == False){ /*if there is a text before the string parameter*/
            print_errors(EXTRA_TEXT_BEFORE_STRING_PARAMETER, number_line, file_name); /*print the error message that there is extra text before the string parameter*/
            return THERE_IS_AN_ERROR; /*return that there is an error because there is extra text before the string parameter*/
        }
    }
    return THERE_IS_NO_ERROR; /*return that there is no error because there is no text before the string parameter*/
}


/**
 * this function checks if there is extra text after the string parameter
 * 
 * parameters:
 *    line - the string guidance line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and there is no text after the string parameter.
 *    THERE_IS_AN_ERROR - if there is an error and there is text after the string parameter.
 */
errors check_if_there_it_is_extra_text_after_string_parameter(char line[], char *file_name, int number_line){
    int i = get_the_index_of_the_last_quote(line); /*get the index of the last quote in the line*/
    i++; /*go to the next char after the last quote*/
    for (; i < strlen(line); i++){ /*go through the line from the last quote*/
        if (isspace(line[i]) == False){ /*if there is a text after the string parameter*/
            print_errors(EXTRA_TEXT_AFTER_STRING_PARAMETER, number_line, file_name); /*print the error message that there is extra text after the string parameter*/
            return THERE_IS_AN_ERROR; /*return that there is an error because there is extra text after the string parameter*/
        }
    }
    return THERE_IS_NO_ERROR; /*return that there is no error because there is no text after the string parameter*/
}

/* --------------- END OF CHECKING STRING GUIDANCE ERRORS --------------- */






/* --------------- CHECKING DATA GUIDANCE ERRORS --------------- */

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
errors check_errors_in_data_guidance(char line[], char *file_name, int number_line){
    if(check_missing_parameters(line, file_name, MIN_AMOUNT_OF_PARAMETERS_IN_DATA_GUIDANCE, number_line, data_guidance) == THERE_IS_AN_ERROR){ /*if there is no parameters in the data guidance*/
        return THERE_IS_AN_ERROR; /*return that there is an error*/
    }
    if (check_commas_placement_for_data_guidance_line(line, file_name, number_line) == THERE_IS_AN_ERROR){ /* if the commas placement in the line is incorrect*/
        return THERE_IS_AN_ERROR; /*return that there is an error*/
    }
    if (check_valid_values_of_data_guidance_line(line, file_name, number_line) == THERE_IS_AN_ERROR){ /*if the values or the types of the data members are invalid*/
        return THERE_IS_AN_ERROR;  /*return that there is an error*/
    }
    return THERE_IS_NO_ERROR; /*return that there is no error*/
}


/**
 * this function checks if the commas placement in the data guidance line is correct
 * 
 * parameters:
 *    line - the data guidance line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and the commas placement is correct.
 *    THERE_IS_AN_ERROR - if there is an error and the commas placement is incorrect.
 */
errors check_commas_placement_for_data_guidance_line(char line[], char *file_name, int number_line){
    int start_index_of_second_word = get_index_of_second_word(line); /*get the index of the second word in the line*/
    return check_commas_placement(line, file_name, start_index_of_second_word, number_line); /*check the commas placement in the line*/
}


/**
 * this function checks if the values and the types of the data members are valid
 * 
 * parameters:
 *    line - the data guidance line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and the values and the types of the data members are valid.
 *    THERE_IS_AN_ERROR - if there is an error and the values and the types of the data members are invalid.
 */
errors check_valid_values_of_data_guidance_line(char line[], char *file_name, int number_line){
    errors error = THERE_IS_NO_ERROR;
    char copy_line[MAX_CHARS_IN_LINE];
    char *token;
    strcpy(copy_line, line);
    strtok(copy_line, " ,\t\n\r\f\v"); /*skip the firs word, in that case - ".data" */
    while ((token = strtok(NULL, " ,\t\n\r\f\v")) != NULL){ /*get the next token in the line*/
        if (check_valid_type_of_member_in_data_guidance(token) == False){  /*check if the type of the member is valid*/
            error = INVALID_TYPE_OF_MEMBER_IN_DATA_GUIDANCE; /*set the error to be that the type of the member is invalid*/
            break;
        } 
        if (check_valid_value_of_member_in_data_guidance(token) == False){ /*check if the value of the member is range*/
            error = INVALID_VALUE_OF_MEMBER_IN_DATA_GUIDANCE; /*set the error to be that the value of the member is invalid*/
            break;
        } 
    } 

    if (error != THERE_IS_NO_ERROR){ /*if there is an error*/
        print_errors(error, number_line, file_name); /*print the error message*/
        return THERE_IS_AN_ERROR; /*return THERE_IS_AN_ERROR*/
    }
    return THERE_IS_NO_ERROR; /*return THERE_IS_NO_ERROR*/
}


/**
 * this function checks if the type of the member in the data guidance is valid
 * 
 * parameters:
 *    member - the member that we want to check if its type is valid
 * 
 * returns:
 *    True - if the type of the member is valid
 *    False - if the type of the member is invalid
 */
boolean check_valid_type_of_member_in_data_guidance(char *member){
    int i = 0;
    if (member[i] == '-' || member[i] == '+'){ /*if the first char is '-' or '+'*/
        i++; /*skip the first char*/
    }
    for (; i < strlen(member); i++){
        if (isdigit(member[i]) == False ){ /*check if the member is a number*/
            return False; 
        }
    }
    return True;
}


/**
 * this function checks if the value of the member in the data guidance is valid
 * 
 * parameters:
 *    member - the member that we want to check if its value is valid
 * 
 * returns:
 *    True - if the value of the member is valid
 *    False - if the value of the member is invalid
 */
boolean check_valid_value_of_member_in_data_guidance(char *member){
    if (atoi(member) < MIN_VALUE_NUMBER_OF_DATA_GUIDANCE || atoi(member) > MAX_VALUE_NUMBER_OF_DATA_GUIDANCE){ /*check if the value of the member is not in the range*/
        return False;
    }
    return True;
}

/* --------------- END OF CHECKING DATA GUIDANCE ERRORS --------------- */





/* --------------- CHECKING ENTRY AND EXTERN GUIDANCE ERRORS --------------- */


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
errors check_errors_in_entry_and_extern_guidance(char line[], char *file_name, int number_line){
    if (check_amount_of_operands_of_entry_or_or_extern_guidance(line, file_name, number_line) == THERE_IS_AN_ERROR){ /*if there is incorrect amount of operands in the entry or extern guidance - more or less than one parameter*/
        return THERE_IS_AN_ERROR; /*return that there is an error*/
    }
    return THERE_IS_NO_ERROR; /*return that there is no error*/
}


/**
 * this function checks if there is incorrect amount of operands in the entry or extern guidance
 * 
 * parameters:
 *    line - the entry or extern guidance line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and the amount of operands is correct (just one operand).
 *    THERE_IS_AN_ERROR - if there is an error and the amount of operands is incorrect.
 */
errors check_amount_of_operands_of_entry_or_or_extern_guidance(char line[], char *file_name, int number_line){
    boolean letter_flag = False;
    errors error = THERE_IS_NO_ERROR;
    int i;
    int counter_of_operand = 0;
    if (has_second_word(line) == False){ /*if there is second word in line- if there is no operand in the entry or extern guidance*/
        error = INCORRECT_AMOUNT_OF_OPERANDS_OF_ENTRY_OR_EXTERN_GUIDANCE; /*set the error to be that there is incorrect amount of operands in the entry or extern guidance*/
    }
    else{
        i = get_index_of_second_word(line); /*get the index of the second word in the line*/
        for (; i < strlen(line); i++){ /*go through the line from the second word*/
            if (letter_flag == True){  /*if there is a letter in the line*/
                if (isspace(line[i]) != False){ /*if there is a space in the line, it means that the operand is finished*/
                    counter_of_operand++; /*increase the counter of the operands*/
                    letter_flag = False; /*set the letter flag to be False*/
                }
            }
            else if (isspace(line[i]) == False){ /*if there is no space in the line*/
                letter_flag = True; /*set the letter flag to be True*/
            }
        }
        if (letter_flag == True){ /*if there is a letter in the end of the line*/
            counter_of_operand++; /*increase the counter of the operands*/
        }
        if (counter_of_operand != ONE_OPERAND){ /*if there is more than one operand in the entry or extern guidance (the case when there is no operand is checked in the first if)*/
            error = INCORRECT_AMOUNT_OF_OPERANDS_OF_ENTRY_OR_EXTERN_GUIDANCE; /*set the error to be that there is incorrect amount of operands in the entry or extern guidance*/
        }
    }
   if (error != THERE_IS_NO_ERROR){ /*if there is an error*/
        print_errors(error, number_line, file_name); /*print the error message that there is incorrect amount of operands in the entry or extern guidance*/
        return THERE_IS_AN_ERROR;
    }
    return THERE_IS_NO_ERROR; /*return that there is no error because the amount of operands is correct (just one operand)*/
}

/* --------------- END OF CHECKING ENTRY AND EXTERN GUIDANCE ERRORS --------------- */





/* --------------- CHECKING INSTRUCTION LINE ERRORS --------------- */

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
errors check_errors_in_instruction_line(char line[], char *file_name, int number_line){
    int amount_of_operands_that_need_to_be; /*get the amount of operands that need to be in the line according to the command*/
    if (is_defined_commend(line, file_name, number_line) == THERE_IS_AN_ERROR){ /*if the first word is not a command*/
        return THERE_IS_AN_ERROR; /*return that there is an error*/
    }
    if (check_commas_placement_for_instruction_line(line, file_name, number_line) == THERE_IS_AN_ERROR){ /*if the commas placement in the line is incorrect*/
        return THERE_IS_AN_ERROR; /*return that there is an error*/
    }

    amount_of_operands_that_need_to_be = get_amount_of_operands_that_need_to_be(line); /*get the amount of operands that need to be in the line according to the command (after we check it indeed a command)*/

    if (check_amount_of_operands_in_instruction_line(line, file_name, number_line, amount_of_operands_that_need_to_be) == THERE_IS_AN_ERROR){ /*if the amount of operands is incorrect*/
        return THERE_IS_AN_ERROR; /*return that there is an error*/
    }
    if (check_values_of_operands_of_instruction_line(line, file_name, number_line, amount_of_operands_that_need_to_be) == THERE_IS_AN_ERROR){ /*if the values of the operands are invalid*/
        return THERE_IS_AN_ERROR; /*return that there is an error*/
    }
    if (is_correct_adrrsing_method_for_source_operand_of_the_command(line, file_name, number_line, amount_of_operands_that_need_to_be) == THERE_IS_AN_ERROR){ /*if the addressing method for the source operand (if there is, else can not be an error) of the command is incorrect (it is depand on the command - which addressing method is correct for the source operand) */
        return THERE_IS_AN_ERROR; /*return that there is an error*/
    }
    if (is_correct_adrrsing_method_for_destination_operand_of_the_command(line, file_name, number_line, amount_of_operands_that_need_to_be) == THERE_IS_AN_ERROR){ /*if the addressing method for the destination operand (if there is, else can not be an error) of the command is incorrect (it is depand on the command - which addressing method is correct for the destination operand) */
        return THERE_IS_AN_ERROR; /*return that there is an error*/
    }
    return THERE_IS_NO_ERROR; /*return that there is no error*/
}


/**
 * this function checks if the commas placement in the instruction line is correct
 * 
 * parameters:
 *    line - the instruction line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and the commas placement is correct.
 *    THERE_IS_AN_ERROR - if there is an error and the commas placement is incorrect.
 */
errors check_commas_placement_for_instruction_line(char line[], char *file_name, int number_line){
    int start_index_of_second_word = get_index_of_second_word(line); /*get the index of the second word in the line*/
    return check_commas_placement(line, file_name, start_index_of_second_word, number_line); /*check the commas placement in the line*/
}


/**
 * this function checks if the first word of the instruction line is a command
 * 
 * parameters:
 *    line - the instruction line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    amount_of_operands_that_need_to_be - the amount of operands that need to be in the line.
 *    number_line - the number of the line that we are checking.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and the amount of operands is correct.
 *    THERE_IS_AN_ERROR - if there is an error and the amount of operands is incorrect.
 */
errors is_defined_commend(char line[], char *file_name, int number_line){
    char copy_line[MAX_CHARS_IN_LINE];
    char *word;
    strcpy(copy_line, line);
    word = strtok(copy_line, " \t\n\r\f\v");
    if (get_command_type(word) == not_command){ /*if the command is not defined*/
        print_errors(UNDEFINED_COMMAND, number_line, file_name); /*print the error message*/
        return THERE_IS_AN_ERROR; /*return that there is an error because the command is not defined*/
    }
    return THERE_IS_NO_ERROR; /*return that there is no error because the command is defined*/
}


/**
 * this function gets the amount of operands that need to be in the line according to the command and checks if the amount of operands is correct
 * 
 * parameters:
 *    line - the instruction line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 *    amount_of_operands_that_need_to_be - the amount of operands that need to be in the line according to the command.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and the amount of operands is correct.
 *    THERE_IS_AN_ERROR - if there is an error and the amount of operands is incorrect.
 */
errors check_amount_of_operands_in_instruction_line(char line[], char *file_name, int number_line, int amount_of_operands_that_need_to_be){
    int counter_of_operands = 0;
    char copy_line[MAX_CHARS_IN_LINE];
    char *word;
    strcpy(copy_line, line);
    strtok(copy_line, " ,\t\n\r\f\v"); /*skip the first word, which is the command*/
    while ((word = strtok(NULL, " ,\t\n\r\f\v")) != NULL){ /*get the next word (operand) in the line*/
        counter_of_operands++; /*increase the counter of the operands*/
    }
    if (counter_of_operands != amount_of_operands_that_need_to_be){ /*if the amount of operands is incorrect*/
        print_errors(INCORRECT_AMOUNT_OF_OPERANDS_OF_COMMAND, number_line, file_name); /*print the error message*/
        return THERE_IS_AN_ERROR; /*return that there is an error because the amount of operands is incorrect*/
    }
    return THERE_IS_NO_ERROR; /*return that there is no error because the amount of operands is correct*/
}


/**
 * this function checks if the values of the operands of the instruction line are valid (if there are operands)
 * 
 * parameters:
 *    line - the instruction line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 *    amount_of_operands - the amount of operands that need to be in the line according to the command.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and the values of the operands are valid.
 *    THERE_IS_AN_ERROR - if there is an error and the values of the operands are invalid.
 */
errors check_values_of_operands_of_instruction_line(char line[], char *file_name, int number_line, int amount_of_operands){
    char copy_line[MAX_CHARS_IN_LINE];
    char *operand;
    strcpy(copy_line, line);
    strtok(copy_line, " ,\t\n\r\f\v"); /*skip the first word, which always is not a operands*/
    if (amount_of_operands >= ONE_OPERAND){ /*if the command has operand */
        operand = strtok(NULL, " ,\t\n\r\f\v"); /*get the first operand because we know that the command has at least one operand*/
        if (check_valid_value_of_operand(operand, file_name, number_line) == THERE_IS_AN_ERROR){ /*check if the value of the operand is valid*/
            return THERE_IS_AN_ERROR; /*return that there is an error because the value of the first operand is invalid*/
        }
    }
    if (amount_of_operands == TWO_OPERANDS){ /*if the command has two operands */
        operand = strtok(NULL, " ,\t\n\r\f\v"); /*get the second operand because we know that the command has two operands*/
        if (check_valid_value_of_operand(operand, file_name, number_line) == THERE_IS_AN_ERROR){ /*check if the value of the operand is valid*/
            return THERE_IS_AN_ERROR; /*return that there is an error because the value of the second operand is invalid*/
        }
    }
    return THERE_IS_NO_ERROR; /*return that there is no error because the values of the operands are valid (if there are operands)*/
}


/**
 * this function checks if the value of the operand is valid according to the rules of the operand:
 * 1. if it is immediate addressing (start with "#")- the value after the hashtag need to be integer number and in the range between -2048 to 2047.
 * 2. if it is indirect register addressing (start with "*")- the value after the asterisk need to be a register (r0-r7).
 * 3. if it is direct register addressing - the value need to be a register (r0-r7).
 * 4. if it is can be a label - just chars and digits and the first char is a lettert, and all the rules of the label definition.
 * 
 * 
 * parameters:
 *    operand - the operand that we want to check if its value is valid
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and the value of the operand is valid.
 *    THERE_IS_AN_ERROR - if there is an error and the value of the operand is invalid.
 */
errors check_valid_value_of_operand(char *operand, char *file_name, int number_line){ 
    if (operand[FIRST_CHAR] == '#'){ /*if the operand is of immediate addressing*/
        return check_immediate_addressing_operand(++operand,file_name, number_line); /*check if the value of the operand is in the range*/
    }
    if (operand[FIRST_CHAR] == '*'){ /*if the operand is of indirect register addressing*/
        if  (get_register(operand+1) == not_register){ /*if the operand is not register*/
            print_errors(INVALID_VALUE_OF_REGISTER_OF_INDIRECT_ADRESSING_OPERAND, number_line, file_name); /*print the error message that the register is invalid*/
            return THERE_IS_AN_ERROR; /*return that there is an error because the register is invalid*/
        }
        return THERE_IS_NO_ERROR; /*return that there is no error because the register is valid*/
    }
    if (get_register(operand) != not_register){ /*if the operand is register*/
        return THERE_IS_NO_ERROR; /*return that there is no error because the register is valid*/
    }

    /*else, the last option is that the is if the operand is label, or it is not a valid operand, in the first pass we cant check if this defined label, but we can check if it is possible to be a label acording to the rules of the label definition*/
    if (is_can_be_label(operand) == False){ /*if the operand is cant even be a label, so it means it is indeed invalid operand*/
        print_errors(INVALID_OPERAND, number_line, file_name); /*print the error message that the operand is invalid*/
        return THERE_IS_AN_ERROR; /*return that there is an error because the operand is invalid*/
    }
    return THERE_IS_NO_ERROR; /*return that there is no error because the operand is valid, and we think that it is a label - we will check in the second pass if it is defined*/

}


/**
 * this function checks if the value of the operand of immediate addressing according to the rules of the immediate addressing:
 * 1. the value of the operand is a integer number.
 * 2. the value of the operand is in the range between -2048 to 2047.
 * 
 * parameters:
 *    operand - the operand that we want to check if its value is in the range
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and the value of the operand is in the range.
 *    THERE_IS_AN_ERROR - if there is an error and the value of the operand is not in the range.
 */
errors check_immediate_addressing_operand(char *operand, char *file_name, int number_line){
    int i = 0;
    if (operand[i] == '-' || operand[i] == '+'){ /*if the first char is '-' or '+'*/
        i++; /*skip the first char*/
    }
    for (; i < strlen(operand); i++){
        if (isdigit(operand[i]) == False){ /*check if the member is a integer number*/ 
            print_errors(INVALID_TYPE_OF_IMMEDIATE_ADDRESSING_OPERAND, number_line, file_name); /*print the error message that the value of the operand is invalid - it is need to be a integer number*/
            return THERE_IS_AN_ERROR; /*return that there is an error because the value of the operand is invalid*/
        }               
    }
    if (atoi(operand) < MIN_VALUE_OF_IMMEDIATE_NUMBER_IN_INSTRUCTION_LINE || atoi(operand) > MAX_VALUE_OF_IMMEDIATE_NUMBER_IN_INSTRUCTION_LINE){ /*check if the value of the operand is not in the range*/
        print_errors(INVALID_VALUE_OF_IMMEDIATE_ADDRESSING_OPERAND , number_line, file_name); /*print the error message that the value of the operand is invalid it is need to be in the range*/
        return THERE_IS_AN_ERROR; /*return that there is an error because the value of the operand is invalid*/
    }
    return THERE_IS_NO_ERROR; /*return that there is no error because the value of the operand is valid*/
}


/**
 * this function checks if the addressing method for the source operand of the command is correct according to the command type (if there is a source operand)
 * 
 * parameters:
 *    line - the instruction line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 *    amount_of_operands - the amount of operands that need to be in the line according to the command
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and the addressing method for the source operand is correct.
 *    THERE_IS_AN_ERROR - if there is an error and the addressing method for the source operand is incorrect.
 */
errors is_correct_adrrsing_method_for_source_operand_of_the_command(char line[], char *file_name, int number_line, int amount_of_operands){
    char copy_line[MAX_CHARS_IN_LINE];
    commands command;
    strcpy(copy_line, line);
    command = get_command_type(strtok(copy_line, " ,\t\n\r\f\v")); /*get the command type*/
    if (amount_of_operands == TWO_OPERANDS){ /*if the command has two operands*/
        return check_addressing_method_for_source_operand_of_two_operands(line, file_name, number_line, command); /*check the addressing method for the source operand of the command*/
    }
    return THERE_IS_NO_ERROR; /*return that there is no error because there is no source operand*/
}


/**
 * this function checks if the addressing method for the destination operand of the command is correct according to the command type
 * 
 * parameters:
 *    line - the instruction line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 *    amount_of_operands - the amount of operands that need to be in the line according to the command
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and the addressing method for the destination operand is correct.
 *    THERE_IS_AN_ERROR - if there is an error and the addressing method for the destination operand is incorrect.
 */
errors is_correct_adrrsing_method_for_destination_operand_of_the_command(char line[], char *file_name, int number_line, int amount_of_operands){
    char copy_line[MAX_CHARS_IN_LINE];
    commands command;
    strcpy(copy_line, line);
    command = get_command_type(strtok(copy_line, " ,\t\n\r\f\v"));
    if (amount_of_operands == TWO_OPERANDS){ /*if the command has two operands*/
        return check_addressing_method_for_destination_operand_of_two_operands(line, file_name, number_line, command); /*check the addressing method for the destination operand of the command (the command is belongs to the group of commands that have two operands)*/
    } 
    else if (amount_of_operands == ONE_OPERAND){ /*if the command has one operand*/
        return check_addressing_method_for_destination_operand_of_one_operand(line, file_name, number_line, command); /*check the addressing method for the destination operand of the command (the command is belongs to the group of commands that have one operand)*/
    }
    return THERE_IS_NO_ERROR; /*return that there is no error because there is no destination operand*/
}


/**
 * this function checks if the addressing method for the source operand of the command is correct according to the command type of the command that has two operands
 * note: just the lea command can not use any addressing method for the source operand except for direct address in contrast to the other commands that have a source operand that can use every addressing method for the source operand.
 * 
 * parameters:
 *    line - the instruction line that we want to check.
 *    command - the command type
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and the addressing method for the source operand is correct.
 *    THERE_IS_AN_ERROR - if there is an error and the addressing method for the source operand is incorrect.
 */
errors check_addressing_method_for_source_operand_of_two_operands(char line[], char *file_name, int number_line,commands command){
    address_method addressing_method_of_source_operand = get_address_method(line,FIRST_PARAMETER); /*get the addressing method for the source operand in the instruction line*/
    if (command == lea){  /*lea command is the only command (of those that have a source operand) that can not have other addressing method except for direct address for the source operand, all the other commands that have a source operand can use every addressing method for the source operand*/
        if (addressing_method_of_source_operand != direct_adress){ /*if the addressing method for the source operand is not direct address*/
            print_errors(ILLEGAL_ADDRESSING_METHOD_FOR_SOURCE_OPERAND, number_line, file_name); /*print the error message*/
            return THERE_IS_AN_ERROR; /*return that there is an error because the addressing method for the source operand is incorrect of the lea command, beacuse it can be only direct address*/
        }
    }
    return THERE_IS_NO_ERROR; /*return that there is no error because the addressing method for the source operand is correct*/
}


/**
 * this function checks if the addressing method for the destination operand of the command is correct according to the command type of the command that has two operands
 * note: of the commands that have two operands - mov, add, sub, lea commands can not use immediate addressing method for the destination operand, all the other commands can use every addressing method for the destination operand.
 * 
 * parameters:
 *    line - the instruction line that we want to check.
 *    command - the command type
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and the addressing method for the destination operand is correct.
 *    THERE_IS_AN_ERROR - if there is an error and the addressing method for the destination operand is incorrect.
 */
errors check_addressing_method_for_destination_operand_of_two_operands(char line[], char *file_name, int number_line, commands command){
    address_method addressing_method_of_destination_operand = get_address_method(line,SECOND_PARAMETER); /*get the addressing method for the destination operand in the instruction line*/
    if (command == mov  || command == add || command == sub || command == lea){ /*mov, add, sub, lea commands can not use immediateaddressing method for the destination operand*/
        if (addressing_method_of_destination_operand == immediate_adress){  /*if the addressing method for the destination operand is immediate address*/
            print_errors(ILLEGAL_ADDRESSING_METHOD_FOR_DESTINATION_OPERAND, number_line, file_name); /*print the error message*/
            return THERE_IS_AN_ERROR; /*return that there is an error because the addressing method for the destination operand is incorrect*/
        }
    }
    return THERE_IS_NO_ERROR; /*return that there is no error because the addressing method for the destination operand is correct*/
}


/**
 * this function checks if the addressing method for the destination operand of the command is correct according to the command type of the command that has one operand
 * note: clr, not, inc, dec, red commands can not use immediate addressing method for the destination operand,
 *  jmp, bne, jsr commands can not use direct address and indirect register addressing method for the destination operand, all the other commands can use every addressing method for the destination operand.
 * 
 * parameters:
 *    line - the instruction line that we want to check.
 *    command - the command type
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and the addressing method for the destination operand is correct.
 *    THERE_IS_AN_ERROR - if there is an error and the addressing method for the destination operand is incorrect.
 */
errors check_addressing_method_for_destination_operand_of_one_operand(char line[], char *file_name, int number_line, commands command){
    address_method addressing_method_of_destination_operand = get_address_method(line,FIRST_PARAMETER); /*get the addressing method for the destination operand in the instruction line*/
    if (command == clr || command == not || command == inc || command == dec || command == red){ /*clr, not, inc, dec, red commands can not use immediate addressing method for the destination operand*/
        if (addressing_method_of_destination_operand == immediate_adress){ /*if the addressing method for the destination operand is immediate address*/
            print_errors(ILLEGAL_ADDRESSING_METHOD_FOR_DESTINATION_OPERAND, number_line, file_name); /*print the error message*/
            return THERE_IS_AN_ERROR; /*return that there is an error because the addressing method for the destination operand is incorrect*/
        }
    }
    if (command == jmp || command == bne || command == jsr){ /*jmp, bne, jsr commands can not use direct_adress and indirect register addressing method for the destination operand*/
        if (addressing_method_of_destination_operand != direct_adress && addressing_method_of_destination_operand != indirect_register_adress){
            print_errors(ILLEGAL_ADDRESSING_METHOD_FOR_DESTINATION_OPERAND, number_line, file_name); /*print the error message*/
            return THERE_IS_AN_ERROR; /*return that there is an error because the addressing method for the destination operand is incorrect*/
        }
    }  
    return THERE_IS_NO_ERROR; /*return that there is no error because the addressing method for the destination operand is correct*/
}

/* --------------- END OF CHECKING INSTRUCTION LINE ERRORS --------------- */





/* --------------- CHECKING LABEL ERRORS --------------- */

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
errors check_errors_in_label(char line[],char label_name[], char *file_name, int number_line, table_of_labels *labels_table, table_of_macros *macros_table){
    errors error = THERE_IS_NO_ERROR;
    if (is_empty_label_name(line) == True){ /*if the label name is empty*/
        error = EMPTY_LABEL_NAME;
    }
    else if (strlen(label_name) > MAX_LENGTH_OF_LABEL_NAME){ /*if the label name is too long*/
        error = LABEL_NAME_IS_TOO_LONG;
    }
    else if (is_a_save_word(label_name) == True){ /*if the label name is a save word*/
        error = LABEL_NAME_IS_SAVE_WORD;
    }
    else if (check_if_first_char_is_letter(label_name) == False){ /*if the label name does not start with a letter*/
        error = LABEL_NAME_NOT_START_WITH_LETTER;
    }
    else if (has_space_after_label_definition(line) == False){ /*if there is no space after the label definition (if it a label defintion and is not in the entry or extern guidance line, the function will return True)*/
        error = NOT_SPACE_AFTER_LABEL_DEFINITION;
    }
    else if (is_name_of_label_contains_illegal_chars(line) == True){ /*if the label name contains illegal characters*/
        error = LABEL_NAME_CONTAINS_ILLEGAL_CHARACTERS;
    }
    else if (is_exist_label(label_name, labels_table) == True){ /*if there is other labels with the same name*/
        error = OTHER_LABELS_WITH_THE_SAME_NAME; 
    }
    else if (is_defined_macro(macros_table, label_name) == True){ /*if the label name is a name of a macro*/
        error = LABEL_WITH_A_NAME_OF_MACRO;
    }

    if (error != THERE_IS_NO_ERROR) { /* check if there is an error */
        print_errors(error, number_line, file_name); /* print an error message */
        return THERE_IS_AN_ERROR; /* return THERE_IS_AN_ERROR */
    }
    return THERE_IS_NO_ERROR;
}


/**
 * this function checks if the label name is empty
 * 
 * parameters:
 *    line - the line that we want to check which contains the label name.
 * 
 * returns:
 *    True - if the label name is empty.
 *    False - if the label name is not empty.
 */
boolean is_empty_label_name(char line[]){
    char *colon = strchr(line, ':'); /* find the colon in the line */
    int index_of_colon = colon - line; /* get the index of the colon */
    int i;
    for (i = 0; i < index_of_colon; i++){ /* go through the line until the colon */
        if (isspace(line[i]) == False){ /* if there is a char that is not a space */
            return False; /* return False */
        }
    }
    return True; /* return True */
}


/**
 * this function checks if the label name is not a name of a macro
 * 
 * parameters:
 *    label_name - the name of the label that we want to check.
 *    macros_table - the table of macros
 * 
 * returns:
 *    True - if the label name is not a name of a macro.
 *    False - if the label name is a name of a macro.
 */
boolean has_space_after_label_definition(char line[]){
    int i;
    for (i = 0; i < strlen(line); i++){
        if (line[i] == ':'){ /*if the char is ':'*/
            if (isspace(line[i+1]) == False){ /*if there is no space after the label definition*/
                return False; /*return False*/
            }
            return True; /*return True*/
        }
    }
    return True;  /*if there is no label definition in the line, it means that there is no need to have space after the label definition, so return True*/
}


/**
 * this function checks if the label name is containes just letters and digits
 * 
 * parameters:
 *    line - the line that we want to check which contains the label name.
 * 
 * returns:
 *    True - if the label name is not containes just letters and digits.
 *    False - if the label name is containes just letters and digits.
 */
boolean is_name_of_label_contains_illegal_chars(char line[]){
    char *colon = strchr(line, ':'); /* find the colon in the line */
    char word[MAX_CHARS_IN_LINE];
    int length;
    int i = 0;
    while (line[i] && isspace(line[i]) != False) {i++;} /*skip the spaces in the beginning of the line*/
    length = colon - (line + i); /* get the length of the label name */
    strncpy(word, line + i, length);
    word[length] = '\0'; /* get the chracters until the colon (not include the spaces at the beginning of the line) */
    return check_if_word_contains_illegal_chars(word, False); /* check if the word contains illegal characters */
}
/* ---------------END OF CHECKING LABEL ERRORS--------------- */





/* --------------- CHECKING GENERAL ERRORS --------------- */

/** 
 * this function checks if there is an error if there is missing parameters in the guidance line
 * 
 * parameters:
 *    line - the guidance line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    amount_of_parameters - the amount of parameters that need to be in the line according to the guidance.
 *    number_line - the number of the line that we are checking.
 *    guidance_type - the type of the guidance (string guidance or data guidance)
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and there is no missing parameters.
 *    THERE_IS_AN_ERROR - if there is an error and there is missing parameters.
 */
errors check_missing_parameters(char line[], char *file_name, int amount_of_parameters, int number_line, guidance guidance_type ){
    errors error = THERE_IS_NO_ERROR;
    char copy_line[MAX_CHARS_IN_LINE];
    int i;
    strcpy(copy_line, line);
    strtok(copy_line, " \t\n\r\f\v"); /*skip the first word, which always is not a parameter*/ 
    for (i = 1; i <=amount_of_parameters; i++){
        if (strtok(NULL, " \t\n\r\f\v") == NULL){ /*if there is no parameter*/
            switch (guidance_type){ /*check which type of guidance it is*/
                case string_guidance: 
                    error = MISSING_PARAMETERS_STRING_GUIDANCE; /*set the error to be that there is missing parameters in the string guidance*/
                    break;
                case data_guidance:
                    error = MISSING_PARAMETERS_DATA_GUIDANCE; /*set the error to be that there is missing parameters in the data guidance*/
                    break;
                default: /* not relevant for this function */
                    break; 
            }
            print_errors(error, number_line, file_name); /*print the error message*/
            return THERE_IS_AN_ERROR; /*return that there is an error because there is missing parameters*/
        }
    }
    return THERE_IS_NO_ERROR; /*return that there is no error because there is no missing parameters*/
}


/**
 * this function checks if there is an error in the commas placement in the line
 * 
 * parameters:
 *    line - the line that we want to check.
 *    file_name - the name of the file that we are checking.
 *    start_index - the index of the first char in the line that we want to check.
 *    number_line - the number of the line that we are checking.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error and the commas placement is correct.
 *    THERE_IS_AN_ERROR - if there is an error and the commas placement is incorrect.
 */
errors check_commas_placement(char line[], char *file_name, int start_index, int number_line){
    errors is_error = THERE_IS_NO_ERROR;
    is_error = check_commas_placement_in_first_part_of_sentence(line, start_index); /* check the commas placement in the first part of the sentence */
    if (is_error != THERE_IS_NO_ERROR) {
        print_errors(is_error, number_line, file_name);
        return THERE_IS_AN_ERROR;
    }
    is_error = check_commas_placement_in_last_part_of_sentence(line, start_index); /* check the commas placement in the last part of the sentence */
    if (is_error != THERE_IS_NO_ERROR) {
        print_errors(is_error, number_line, file_name);
        return THERE_IS_AN_ERROR;
    }
    return THERE_IS_NO_ERROR;
}


/**
 * this function checks if there is an error in the commas placement in the first part of the sentence
 * 
 * parameters:
 *    line - the line that we want to check.
 *    start_index - the index of the first char in the line that we want to check.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error because there is no commas in the first part of the sentence.
 *    ILLEGAL_COMMAS - if there is an error because there is a comma in the first part of the sentence.
 */
errors check_commas_placement_in_first_part_of_sentence(char line[], int start_index){
    int i;
    for (i = 0; i < start_index; i++){
        if (line[i] == ','){
            return ILLEGAL_COMMAS;
        }
    }
    return THERE_IS_NO_ERROR;
}


/**
 * this function checks if there is an error in the commas placement in the last part of the sentence
 * 
 * parameters:
 *    line - the line that we want to check.
 *    start_index - the index of the first char in the line that we want to check.
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error because there is no commas in the last part of the sentence.
 *    MISSING_COMMAS - if there is an error because there is a missing comma in the last part of the sentence.
 *    ILLEGAL_COMMAS - if there is an error because there is an illegal comma in the last part of the sentence.
 *    MULTIPLE_COMMAS - if there is an error because there is multiple commas in the last part of the sentence.
 *    COMMAS_AT_END - if there is an error because there is commas at the end of the sentence.
 */
errors check_commas_placement_in_last_part_of_sentence(char line[], int start_index){
    errors error = THERE_IS_NO_ERROR;
    boolean letter_flag = False;
    boolean word_flag = False;
    boolean comma_flag = False;
    int current_index = start_index;
    while (line[current_index] != '\0' && error == THERE_IS_NO_ERROR) { /* check the commas placement in the last part of the sentence */
        if (isspace(line[current_index]) == False && line[current_index] != ',') { /* if the char is not a space and not a comma */
            if (word_flag == True && comma_flag == False) {
                error = MISSING_COMMAS; /* there is error in the comma placement - missing comma */
            }
            else if (word_flag == True && comma_flag == True) { 
                word_flag = False; 
                comma_flag = False; 
            }  
            else {
                letter_flag = True;
            }
        } 
        else if (line[current_index] == ',') { /* if the char is a comma */
            if (word_flag == False && letter_flag == True) {
                word_flag = True;
                comma_flag = True;
            }
            else if (word_flag == False && letter_flag == False ) {
                error = ILLEGAL_COMMAS; /* there is error in the comma placement - illegal comma */
            }  
            else if (comma_flag == True) {
                error = MULTIPLE_COMMAS; /* there is error in the comma placement - multiple commas */
            } 
            else {
                comma_flag = True;
            }
        } 
        else if (letter_flag == True) { /* if the char is a space */
            word_flag = True;
        }
        current_index++; /* move to the next char */
    }

    if (comma_flag == True && error == THERE_IS_NO_ERROR) { /* if there is commas at the end of the sentence */
        error = COMMA_AT_THE_END;
    }
    return error;
}


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
void check_exceeding_memory_limit(char *file_name, int IC, int  DC, boolean *is_memory_exceeded, errors *error){
    if (IC + DC > MAX_MEMORY_LIMIT){ /*if the sum of the IC and DC is greater than the maximum memory limit*/
        *is_memory_exceeded = True; /*set the memory exceeded flag to be True*/
        print_errors(EXCEEDING_MEMORY_LIMIT, NOT_IN_SPECIFIC_LINE, file_name); /*print the error message*/
        *error = True; /*set the error to be True*/
    }
}

/* ---------------END OF CHECKING GENERAL ERRORS--------------- */