#include <string.h>
#include <ctype.h>
#include "help_methods_for_first_pass.h"
#include "../util/help_methods.h"
#include "../enums/guidance.h"
#include "../enums/commands.h"
#include "../enums/registers.h"
#include "../enums/boolean.h"
#include "../enums/operand_type.h"
#include "../enums/address_method.h"


#define FIRST_CHAR_IN_WORD 0 /* the index of the first char in the word */
#define MAX_CHARS_IN_LINE 82 /* the maximum length of the line is 81 and +1 for the null terminator, when we define the array */
#define MAX_LABEL_LENGTH 31 /* the maximum length of the label */
#define AMOUNT_OF_COMMANDS 16 /* the amount of the commands */
#define AMOUNT_OF_GUIDANCE_TYPES 4 /* the amount of the guidance types */
#define AMOUNT_OF_REGISTERS 8 /* the amount of the registers */
#define EQUAL 0 /* for indicating that two values are equal */
#define LENGTH_OF_COLON 1 /* the length of the colon */
#define QUOTE_NOT_FOUND -1 /* the value that indicates that the quote was not found */
#define NO_OPERANDS 0 /* no operands in the line */
#define ONE_OPERAND 1 /* one operand in the line */
#define TWO_OPERANDS 2 /* two operands in the line */
#define AMOUNT_OF_BITS 15 /* the amount of bits in the number in our project */
#define BIT_ON 1 /* the value for just the rightmost bit is on, and the rest are off */
#define VALUE_TO_SHIFT_FOR_THE_NEXT_BIT 1 /* the value to shift for the next bit */


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
boolean is_can_be_label(char *word){
    int i;
    if (check_if_first_char_is_letter(word) == False){ /* check if the first character of the word is a letter */
        return False; /* label must start with a letter */
    }
    if (strlen(word) > MAX_LABEL_LENGTH){ /* check if the length of the label is too long */
        return False; /* the length of the label must be less than 31 */
    }
    if (is_a_save_word(word) == True){ /* check if the word is a save word */
        return False; /* label can't be a save word */
    }
    for (i = 0; i < strlen(word); i++){
        if (isalnum(word[i]) == False){ /* check if the word contains only letters and digits */
            return False; /* label can contain only letters and digits */
        } 
    }
    return True; /* if the word passed all the checks, it can be a label */
}   


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
boolean is_label_definition(char line[]){
    int i;
    for (i = 0; i < strlen(line); i++){ /* for each character in the line */
        if (line[i] == ':'){ /* if the character is a colon */
            return True; /* if there is a label definition in the line */
        }
    }
    return False;  /* if there is no label definition in the line */
}


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
boolean has_second_word(char line[]){
    int i = 0;
    while (line[i] && isspace(line[i])){i++;} /* skip the spaces */
    while (line[i] && !isspace(line[i])){i++;} /* skip the first word */
    while (line[i] && isspace(line[i])){i++;} /* skip the spaces between the words */

    return line[i] != '\0'; /*Check if there's a second word*/
}


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
commands get_command_type(char *command) {
    const char *commands_names[] = { "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop" }; /* the names of all commands */
    int i;
    for (i = 0; i < AMOUNT_OF_COMMANDS; i++) {  /* check if the parameter is one of the commands */
        if (strcmp(command, commands_names[i]) == EQUAL) { /* if the parameter is a command */
            return (commands)i; /* return the type of the command */
        }
    }
    return not_command; /* if the parameter is not a command , return not_command */
}


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
operand_type get_parameter_type(char line[], int operand_number) {
    char copy_line[MAX_CHARS_IN_LINE];
    char *operand;
    operand_type type_of_operand;
    int i;
    strcpy(copy_line, line);
    strtok(copy_line, " ,\t\n\r\f\v"); /*skip the command*/
    for (i = 0; i < operand_number - 1; i++) { /* skips until the operand that we want to check its type */
        strtok(NULL, " ,\t\n\r\f\v");
    }
    operand = strtok(NULL, " ,\t\n\r\f\v");
    if (operand == NULL) { /* if there is no operand in the line */
        type_of_operand = not_operand; 
    }
    else if (operand[FIRST_CHAR_IN_WORD] == '#') { /* if the operand is a number */
        type_of_operand = number;
    }
    else if (operand[FIRST_CHAR_IN_WORD] == '*') { /* if the operand is a register, (even if it is indirect adress method) */
        type_of_operand = reg; 
    }
    else if (get_register(operand) != not_register) { /* if the operand is a register */
        type_of_operand = reg;
    }
    else {
        type_of_operand = label; /* if the operand is a label (we assume that the operand is a label, because we checked if it is can be a label before) */
    }
    return type_of_operand; /* return the type of the operand */
}


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
guidance get_guidance_type(char line[]){
    char copy_line[MAX_CHARS_IN_LINE];
    char *word;
    const char *guidance_types[] = { ".data", ".string", ".entry", ".extern" }; /* the names of all guidance types */
    int i;
    strcpy(copy_line, line);
    if (is_label_definition(line) == True){ /* if there is a label definition in the line */
        strtok(copy_line, " \t\n\r\v\f"); /* skip of the name of the label */
        word = strtok(NULL, " \t\n\r\v\f"); /* get the first word after the label */
        if (word == NULL) { /* if there is no word after the label - we will handle this case in other function (print error) */
            return no_guidance;
        }
    }
    else{
        word = strtok(copy_line, " \t\n\r\v\f"); /* get the first word in the line */
    }
    for (i = 0; i < AMOUNT_OF_GUIDANCE_TYPES; i++) { /* check if the parameter is one of the guidance types */
        if (strcmp(word, guidance_types[i]) == EQUAL) { /* if the parameter is a guidance type */
            return (guidance)i; /* return the type of the guidance */
        }
    }
    return no_guidance; /* if the parameter is not a guidance type, return no_guidance */
}


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
registers get_register(char *reg){
    const char *registers_names[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7" }; /* the names of all registers */
    int i;
    for (i = 0; i < AMOUNT_OF_REGISTERS; i++) { /* check if the parameter is one of the registers */
        if (strcmp(reg, registers_names[i]) == EQUAL) { /* if the parameter is a register */
            return (registers)i; /* return the type of the register */
        }
    }
    return not_register; /* if the parameter is not a register, return not_register */
}


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
address_method get_address_method(char line[], int operand_number) {
    char copy_line[MAX_CHARS_IN_LINE];
    char *operand;
    int i;
    strcpy(copy_line, line);
    strtok(copy_line, " ,\t\n\r\f\v"); /*skip the command*/
    for (i = 0; i < operand_number - 1; i++) { /* skips until the operand that we want to check its type */
        strtok(NULL, " ,\t\n\r\f\v");
    }
    operand = strtok(NULL, " ,\t\n\r\f\v");
    if (operand[FIRST_CHAR_IN_WORD] == '#') { /* if the address method is immediate adress method */
        return immediate_adress;
    }
    else if(operand[FIRST_CHAR_IN_WORD] == '*') { /* if the address method is indirect register adress method */
        return indirect_register_adress;
    }
    else if (get_register(operand) != not_register) { /* if the address method is direct register adress method */
        return direct_register_adress;
    }
    return direct_adress; /* we assume that if the addressing method is not one of the ones listed above, it is a direct addressing method.*/
}


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
int get_the_index_of_the_first_quote(char line[]){
    int i;
    for (i = 0; i < strlen(line); i++){ /* for each character in the line */
        if (line[i] == '"'){ /* if the character is a quote */
            return i; /* return the index of the quote */
        }
    }
    return QUOTE_NOT_FOUND; /* if there is no quote in the line */
}


/**
 * this function returns the index of the last quote in the line
 * 
 * parameters:
 *    line - the line that we want to get the index of the last quote
 * 
 * returns:
 *    the index of the last quote in the line (if there is no quote in the line, it returns -1)
 * 
 */
int get_the_index_of_the_last_quote(char line[]){
    int i;
    for (i = strlen(line)-1; i >= 0; i--){ /* for each character in the line */
        if (line[i] == '"'){ /* if the character is a quote */
            return i; /* return the index of the quote */
        }
    }
    return QUOTE_NOT_FOUND; /* if there is no quote in the line */
}


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
int get_amount_of_operands_that_need_to_be(char line[]){
    char copy_line[MAX_CHARS_IN_LINE];
    char *command_word;
    commands command;
    strcpy(copy_line, line);
    command_word = strtok(copy_line, " \t\n\r\v\f"); /* get the command */
    command = get_command_type(command_word); /* get the type of the command */
    switch (command){ /* check the command */
        case mov:
        case cmp:
        case add:
        case sub:
        case lea:
            return TWO_OPERANDS; /* these commands need two operands */
        case not:
        case clr:
        case inc:
        case dec:
        case jmp:
        case bne:
        case red:
        case prn:
        case jsr:
            return ONE_OPERAND; /* these commands need one operand */
        case rts:
        case stop:
            return NO_OPERANDS; /* these commands need no operands */
        default:
            break; /* not relevant for this function (we check before we call to this function )*/
    }
    return -1; /* garbage value */
}


/**
 * converts a positive number to its negative counterpart using the two's complement representation in a 15 bit .
 * 
 * parameters:
 *    number - the number that we are converting to its negative counterpart
 * 
 * returns:
 *   the negative version of the number according to the two's complement representation in 15 bits
 */
int handle_negative_number(int number)
{
    unsigned int mask = BIT_ON; /* the mask */
    int i = 0;
    boolean is_bit_on = False; /* the flag that indicates if the current bit is on */
    while(i < AMOUNT_OF_BITS)
    {
        if(is_bit_on == True)
            number = (int)(number ^ mask); /* Reverses the bits(0 will be 1 and 1 will be 0). */
            /* Checks if the current bit is 1, that is mean the current bit is on bit. */
        else if(number & mask)
            is_bit_on = True;
        mask <<= VALUE_TO_SHIFT_FOR_THE_NEXT_BIT;
        i++;
    }
    return number; /* return the negative number we got */
}


/**
 * this function change the line to be without the label definition
 * 
 * parameters:
 *    line - the line that we want to change to be without the label definition
 * 
 */
void change_the_line_to_be_without_label_definition(char *line){
    char copy_line[MAX_CHARS_IN_LINE];
    char *colon_pos;
    int label_length;
    int i, j;
    strcpy(copy_line, line); /* copy the line to preserve the original line */
    colon_pos = strchr(copy_line, ':'); /* find the position of the colon */
    if (colon_pos == NULL) {return;} /* if there is no colon in the line, return */
    label_length = colon_pos - copy_line; /* calculate the length of the label */
    for (i = label_length + LENGTH_OF_COLON, j = 0; i < strlen(line); i++, j++) { /* copy the line without the label definition */
        line[j] = line[i];
    } 
    line[j] = '\0'; /* add the null terminator to the end of the line */
}