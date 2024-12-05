#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encode_first_pass.h"
#include "help_methods_for_first_pass.h"
#include "../enums/address_method.h"
#include "../enums/operand_type.h"
#include "../enums/registers.h"
#include "../enums/encoding_type.h"
#include "../enums/commands.h"
#include "../util/help_methods.h"

#define NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY 2 /* The number of columns in the instruction and data arrays */
#define EQUAL 0 /* the value that represent that two values are equal */
#define MAX_CHARS_IN_LINE 82 /* the maximum amount of chars in a line */
#define FIRST_PARAMETER 1 /* the first parameter in the line */
#define SECOND_PARAMETER 2 /* the second parameter in the line */
#define ONE_PARAMETER 1  /* the number that represent that there is one parameter in the line */
#define TWO_PARAMETERS 2 /* the number that represent that there are two parameters in the line */
#define FIRST_COLUMN 0 /* The first column in the array of the instruction and data array(representing the first 8 bits of the encoded line) */    
#define SECOND_COLUMN 1 /* The second column in the array of the instruction and data array(representing the others  bits of the encoded line) */
#define FIRST_CHAR_IN_WORD 0 /* The first char in the word */
#define VALUE_TO_SHIFT_FOR_OPCODE_BITS 4 /* The value that we need to shift for change the bits that represent the opcode */
#define VALUE_TO_SHIFT_FOR_ENCODING_TYPE_BITS 1 /* The value that we need to shift for change the bits that represent the encoding type */
#define VALUE_TO_SHIFT_FOR_SECOND_ADRESS_METHOD_BITS 4 /* The value that we need to shift for change the bits that represent the second adress method */
#define VALUE_TO_SHIFT_FOR_DESTINATION_REGISTER_BITS 4 /* The value that we need to shift for change the bits that represent the destination register */
#define VALUE_TO_SHIFT_FOR_SOURCE_REGISTER_BITS_THAT_IN_SECOND_COLUMN 7 /* The value that we need to shift for change the bits that represent the source register that in the second column */
#define VALUE_TO_SHIFT_FOR_SOURCE_REGISTER_BITS_THAT_IN_FIRST_COLUMN 1 /* The value that we need to shift for change the bits that represent the source register that in the first column */
#define VALUE_TO_SHIFT_FOR_NUMBER_BITS_OF_INSTRUCTION_LINE_THAT_IN_SECOND_COLUMN 4 /* The value that we need to shift for change the bits that represent the number of instruction line that in the second column */
#define VALUE_TO_SHIFT_FOR_NUMBER_BITS_OF_INSTRUCTION_LINE_THAT_IN_FIRST_COLUMN 4 /* The value that we need to shift for change the bits that represent the numbe rof instruction line that in the first column */
#define VALUE_TO_SHIFT_FOR_NUMBER_BITS_OF_DATA_GUIDANCE_THAT_IN_SECOND_COLUMN 1 /* The value that we need to shift for change the bits that represent the number that in the second column */
#define VALUE_TO_SHIFT_FOR_NUMBER_BITS_OF_DATA_GUIDANCE_THAT_IN_FIRST_COLUMN 7 /* The value that we need to shift for change the bits that represent the number that in the first column */
#define VALUE_TO_SHIFT_FOR_CHAR_BIRS_OF_STRING_GUIDANCE_THAT_IN_SECOND_COLUMN 1 /* The value that we need to shift for change the bits that represent the char that in the second column */
#define VALUE_TO_SHIFT_FOR_CHAR_BIRS_OF_STRING_GUIDANCE_THAT_IN_FIRST_COLUMN 7 /* The value that we need to shift for change the bits that represent the char that in the first column */
#define VALUE_OF_MASK_OF_15_BITS 0x7FFF /* The value of the mask of 15 bits */



/* ---------------PROTOTYPES--------------- */

void get_string_from_string_guidance_line(char [], char []);
void encode_first_word_of_instruction_line(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char [], int *, int);
void encode_command_of_instruction_line(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char [], int *);
void encode_first_adress_method(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char [], int *, int);
void encode_second_adress_method(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char [], int *, int);
void encode_other_words_of_instruction_line(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char [], int *, operand_type, operand_type);
void encode_absolute_type(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY],int *);
void encode_operand(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char [], int *, int, int, operand_type);
void encode_number(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY],char [], int *, int);
void encode_register(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char [], int *, int, int);

/* ---------------END PROTOTYPES --------------- */





/* ---------------ENCODE STRING GUIDANCE--------------- */

/**
 * this function encodes the string guidance line
 * 
 * parameters:
 *    data_array - the array that will hold and store the encoded data lines
 *    line - the string guidance line that we want to encode
 *    DC - the data counter
 * 
 */
void encode_string_guidance(char data_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char line[], int *DC) {
    char string[MAX_CHARS_IN_LINE];
    int asci_value;
    int i;
    get_string_from_string_guidance_line(line, string); /* the string paramter will be the value of the string guidance parameter in the line */
    for (i = 0; i < strlen(string); i++) {
        asci_value = string[i];
        data_array[*DC][SECOND_COLUMN] |= asci_value << VALUE_TO_SHIFT_FOR_CHAR_BIRS_OF_STRING_GUIDANCE_THAT_IN_SECOND_COLUMN;
        asci_value = asci_value >> VALUE_TO_SHIFT_FOR_CHAR_BIRS_OF_STRING_GUIDANCE_THAT_IN_FIRST_COLUMN;
        data_array[*DC][FIRST_COLUMN] |= asci_value;
        (*DC)++;
    }
    (*DC)++; /* for the null terminator (the data array is initialized with zeros) */
} 


/**
 * this function update the value of the string argument to be the value of the parameter in the string guidance line
 * 
 * parameters:
 *    line - the string guidance line that we want to get the string from
 *    string - varible that we want to to be the value of the string argument
 * 
 */
void get_string_from_string_guidance_line(char line[], char string[]) {
    int start_index, end_index;  /* the start and the end index of the string */
    start_index = get_the_index_of_the_first_quote(line); /* get the index of the first quote */
    end_index = get_the_index_of_the_last_quote(line); /* get the index of the last quote */
    strncpy(string, line + start_index + 1, end_index - start_index - 1); /* copy the string to a new string */
    string[end_index - start_index - 1] = '\0' ;/* add the null terminator */
}

/* ---------------END ENCODE STRING GUIDANCE--------------- */





/* ---------------ENCODE DATA GUIDANCE--------------- */

/**
 * this function encodes the data guidance line
 * 
 * parameters:
 *    data_array - the array that will hold the encoded data lines
 *    line - the data guidance line that we want to encode
 *    DC - the data counter
 * 
 */
void encode_data_guidance(char data_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY],char line[], int *DC){
    char copy_line[MAX_CHARS_IN_LINE];
    int number;
    char *number_string;
    strcpy(copy_line, line); /* copy the line to a new string */
    strtok(copy_line, " ,\t\n\r\f\v"); /*skip the word ".data"*/
    while ((number_string = strtok(NULL, " ,\t\n\r\f\v")) != NULL){
        if (number_string[FIRST_CHAR_IN_WORD] == '-') {
            number = handle_negative_number(atoi(++number_string)); /* get the value of the negative number according to the two's complement representation in 15 bits */
        }
        else{
            number = atoi(number_string); /* convert the string to a number */
        }
        data_array[*DC][SECOND_COLUMN] |= number << VALUE_TO_SHIFT_FOR_NUMBER_BITS_OF_DATA_GUIDANCE_THAT_IN_SECOND_COLUMN; /* encode the number in the second column */
        number = number >> VALUE_TO_SHIFT_FOR_NUMBER_BITS_OF_DATA_GUIDANCE_THAT_IN_FIRST_COLUMN; /* shift the number to the right */
        data_array[*DC][FIRST_COLUMN] |= number; /* encode the number in the first column */
        (*DC)++; /* update the DC */
    }
}

/* ---------------END ENCODE DATA GUIDANCE--------------- */


/* ---------------ENCODE INSTRUCTION LINE--------------- */

/**
 * this function encodes the instruction line
 * 
 * parameters:
 *    instruction_array - the array that will hold and sotre the encoded instruction lines
 *    line - the instruction line that we want to encode
 *    IC - the instruction counter 
 */
void encode_instruction(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char line[], int *IC) {
    operand_type type_of_first_operand;
    operand_type type_of_second_operand;
    int amount_of_operands = get_amount_of_operands_that_need_to_be(line); /* get the amount of operands that need to be according to the command name */
    encode_first_word_of_instruction_line(instruction_array, line, IC ,amount_of_operands); /* encode the first word of the instruction line */
    type_of_first_operand = get_parameter_type(line,FIRST_PARAMETER); /* get the type of the first operand (if there is) */
    type_of_second_operand = get_parameter_type(line,SECOND_PARAMETER);  /* get the type of the second operand (if there is) */
    encode_other_words_of_instruction_line(instruction_array, line, IC, type_of_first_operand, type_of_second_operand); /* encode the other words of the instruction line(if there are) , and update the IC accordingly */ 
}


/**
 * this function encodes the first word of the instruction line
 * 
 * parameters:
 *    instruction_array - the array that will hold and sotre the encoded instruction lines
 *    line - the instruction line that we want to encode
 *    IC - the instruction counter
 * 
 */
void encode_first_word_of_instruction_line(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char line[], int *IC, int amount_of_operands)
{
    encode_command_of_instruction_line(instruction_array, line, IC); /* encode the command */
    encode_first_adress_method(instruction_array, line, IC, amount_of_operands); /* encode the first adress method */
    encode_second_adress_method(instruction_array, line, IC, amount_of_operands); /* encode the second adress method */
    encode_absolute_type(instruction_array,IC); /* encode the encoding type */ 
    (*IC)++; /* update the IC */
}


/**
 * this function encodes the command of the instruction line
 * 
 * parameters:
 *    instruction_array - the array that will hold and sotre the encoded instruction lines
 *    line - the instruction line that we want to encode
 *    IC - the instruction counter
 * 
 */
void encode_command_of_instruction_line(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char line[], int *IC) {
    char copy_line[MAX_CHARS_IN_LINE];
    char *command;
    commands opcode;
    strcpy(copy_line, line);
    command = strtok(copy_line, " ,\t\n\r\f\v"); /* get the command */
    opcode = get_command_type(command); /* get the opcode of the command (we know that this is command , so we dont need to check the case it is not)*/  
    instruction_array[*IC][FIRST_COLUMN] |= opcode << VALUE_TO_SHIFT_FOR_OPCODE_BITS; /* encode the opcode as the first 4 bits of the instruction line */   
}


/**
 * this function encodes the first adress method of the instruction line
 * 
 * parameters:
 *    instruction_array - the array that will hold and sotre the encoded instruction lines
 *    line - the instruction line that we want to encode
 *    IC - the instruction counter
 * 
 */
void encode_first_adress_method(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char line[], int *IC, int amount_of_operands) {
    char copy_line[MAX_CHARS_IN_LINE];
    char *first_operand;
    address_method first_address_method;
    strcpy(copy_line, line);
    strtok(copy_line, " ,\t\n\r\f\v"); /*skip the command*/
    first_operand = strtok(NULL, " ,\t\n\r\f\v");
    /* if there is no first operand, this is a command without operands, which means that the bits representing the first address method need to be 0 (they already are) */
    if (first_operand != NULL && amount_of_operands == TWO_PARAMETERS) { /* if there is a first operand */
        first_address_method = get_address_method(line,FIRST_PARAMETER); /* get the address method of the first operand */
        instruction_array[*IC][FIRST_COLUMN] |= first_address_method; /* encode the first address method */
    }   
}


/**
 * this function encodes the second adress method of the instruction line
 * 
 * parameters:
 *    instruction_array - the array that will hold and sotre the encoded instruction lines
 *    line - the instruction line that we want to encode
 *    IC - the instruction counter
 * 
 */
void encode_second_adress_method(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char line[], int *IC, int amount_of_operands) {
    char copy_line[MAX_CHARS_IN_LINE];
    char *second_operand;
    address_method second_address_method;
    strcpy(copy_line, line);
    strtok(copy_line, " ,\t\n\r\f\v"); /*skip the command*/
    if (amount_of_operands == TWO_PARAMETERS) { /* if there is only two parameters */
        strtok(NULL, " ,\t\n\r\f\v"); /*skip the first operand*/
    }
    second_operand = strtok(NULL, " ,\t\n\r\f\v"); /* not skip the first operand, because we have just one operand, which it is the destination operand */
    /* if there is no second operand, this means that the bits representing the second address method need to be 0 (they already are) */
    if (second_operand != NULL) {
        second_address_method = get_address_method(line,amount_of_operands); /* get the address method of the second operand */
        instruction_array[*IC][SECOND_COLUMN] |= second_address_method << VALUE_TO_SHIFT_FOR_SECOND_ADRESS_METHOD_BITS; /* encode the second address method */
    }
}


/**
 * this function handle the encodings of the other words of the instruction line - the operands
 * 
 * parameters:
 *    instruction_array - the array that will hold and sotre the encoded instruction lines
 *    line - the instruction line that we want to encode
 *    IC - the instruction counter
 *    type_of_first_operand - the type of the first operand (to know how to encode it, and if exists)
 *    type_of_second_operand - the type of the second operand (to know how to encode it, and if exists)
 * 
 */
void encode_other_words_of_instruction_line(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char line[], int *IC, operand_type type_of_first_operand, operand_type type_of_second_operand) {
    if (type_of_first_operand != not_operand) { /* if there is a first operand */
        if (type_of_second_operand != not_operand) {  /* if there is a second operand */
            encode_operand(instruction_array, line, IC, FIRST_PARAMETER, TWO_PARAMETERS, type_of_first_operand); /* encode the first operand */
            if (type_of_first_operand == reg && type_of_second_operand == reg) { /* if the first and the second operands are registers */
                (*IC)--; /* we need to encode the registers in the same word */
            }
            encode_operand(instruction_array, line, IC, SECOND_PARAMETER, TWO_PARAMETERS, type_of_second_operand); /* encode the second operand */

        }
        else { /* if there is only one operand */
        encode_operand(instruction_array, line, IC, FIRST_PARAMETER, ONE_PARAMETER, type_of_first_operand); /* encode the first operand */
        }
    }
}

/* ---------------END ENCODE INSTRUCTION LINE--------------- */

/* ---------------GENERAL ENCODE METHODS--------------- */

/**
 * this function encodes the absolute type of the instruction line
 * 
 * parameters:
 *    instruction_array - the array that will hold and sotre the encoded instruction lines
 *    IC - the instruction counter
 * 
 */
void encode_absolute_type(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY],int *IC) {
    encoding_type A = absolute_type; /* the encoding type is absolute */
    instruction_array[*IC][SECOND_COLUMN] |= A << VALUE_TO_SHIFT_FOR_ENCODING_TYPE_BITS; /* encode the encoding type */
}


/**
 * this function encodes the operand parameter
 * 
 * parameters:
 *    instruction_array - the array that will hold and sotre the encoded instruction lines
 *    line - the instruction line that we want to encode
 *    IC - the instruction counter
 *    parameter_position - the position of the operand in the line
 *    amount_of_operands - the amount of operands in the line
 *    type - the type of the operand
 * 
 */
void encode_operand(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char line[], int *IC, int parameter_position, int amount_of_operands, operand_type type) {
    /* if the operand is a label it will encoded in the second pass, and not now*/
    switch (type) { 
        case reg: /* if the operand is a register */
            encode_register(instruction_array, line, IC, parameter_position, amount_of_operands);
            break;
        case number: /* if the operand is a number */
            encode_number(instruction_array, line, IC, parameter_position);
            break;
        default: /* if the operand is a label, we will encode it in the second pass */
            (*IC)++;  /* update the IC */
    }
}


/**
 * this function encodes the number parameter of the instruction line
 * 
 * parameters:
 *    instruction_array - the array that will hold and sotre the encoded instruction lines
 *    line - the instruction line that we want to encode
 *    IC - the instruction counter
 *    operand_number - the number of the operand in the line
 * 
 */
void encode_number(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY],char line[], int *IC, int operand_number) {
    char copy_line[MAX_CHARS_IN_LINE];
    char *operand;
    int number;
    int i;
    strcpy(copy_line, line); /* copy the line to a new string */
    strtok(copy_line, " ,\t\n\r\f\v"); /*skip the command*/
    for (i = 0; i < operand_number - 1; i++) {
        strtok(NULL, " ,\t\n\r\f\v");
    }
    operand = strtok(NULL, " ,\t\n\r\f\v"); /* get the operand */
    operand++; /* skip the '#' */ 
    if (operand[FIRST_CHAR_IN_WORD] == '-') { /* if the operand is a negative number */
            number = handle_negative_number(atoi(++operand)); /* get the value of the negative number according to the two's complement representation in 15 bits */
        }
        else{
            number = atoi(operand); /* convert the string to a number */
        }
    instruction_array[*IC][SECOND_COLUMN] |= number << VALUE_TO_SHIFT_FOR_NUMBER_BITS_OF_INSTRUCTION_LINE_THAT_IN_SECOND_COLUMN; 
    number = number >> VALUE_TO_SHIFT_FOR_NUMBER_BITS_OF_INSTRUCTION_LINE_THAT_IN_FIRST_COLUMN;
    instruction_array[*IC][FIRST_COLUMN] |= number;
    encode_absolute_type(instruction_array,IC); /* add to the encoding number the absolue encoding type */
    (*IC)++;  /* update the IC */
}


/**
 * this function encodes the register parameter
 * 
 * parameters:
 *    instruction_array - the array that will hold and sotre the encoded instruction lines
 *    line - the instruction line that we want to encode
 *    IC - the instruction counter
 *    parameter_position - the position of the register in the line
 *    amount_of_operands - the amount of operands in the line
 * 
 */
void encode_register(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char line[], int *IC, int parameter_position, int amount_of_operands) {
    char copy_line[MAX_CHARS_IN_LINE];
    char *operand;
    registers reg;
    int i;
    strcpy(copy_line, line);
    strtok(copy_line, " ,\t\n\r\f\v"); /*skip the command*/
    for (i = 0; i < parameter_position - 1; i++) { /* skip until the required number of the operand */
        strtok(NULL, " ,\t\n\r\f\v");
    }
    operand = strtok(NULL, " ,\t\n\r\f\v"); /* get the operand */
    if (operand[FIRST_CHAR_IN_WORD] == '*') {operand++;} /* if the operand is a register in a indirect addressing method, but we will still encode it as a register */
    reg = get_register(operand); /* get the register */
    if (parameter_position == FIRST_PARAMETER && amount_of_operands == TWO_PARAMETERS) { /* if the operand is the first operand, encode the register when it is the source register */
        instruction_array[*IC][SECOND_COLUMN] |= reg << VALUE_TO_SHIFT_FOR_SOURCE_REGISTER_BITS_THAT_IN_SECOND_COLUMN; 
        reg = reg >> VALUE_TO_SHIFT_FOR_SOURCE_REGISTER_BITS_THAT_IN_FIRST_COLUMN;
        instruction_array[*IC][FIRST_COLUMN] |= reg; 
    }
    else { /* parameter_position == SECOND_PARAMETER or paramter position is the first parameter but there is only one operand, so we will treat it as the destination register */
        instruction_array[*IC][SECOND_COLUMN] |= reg << VALUE_TO_SHIFT_FOR_DESTINATION_REGISTER_BITS; /* encode the register when it is the destination register */
    }
    encode_absolute_type(instruction_array,IC); /* encode the encoding type */
    (*IC)++; /* update the IC */
}

/* ---------------END GENERAL ENCODE METHODS--------------- */