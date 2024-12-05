#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "help_methods.h"
#include "../enums/errors.h"
#include "../enums/boolean.h"
#include "../errors_handling/print_messages.h"

#define MAX_LENGTH_OF_LINE 81 /* The maximum length of the line is 80 chars (not including the char '\n') so it is 81 with the '\n' */
#define FIRST_CHAR_IN_LINE 0 /* The index of the first char in the line */
#define EQUAL 0 /* The result of the comparison between two strings */
#define AMOUNT_OF_COMMANDS 16 /* The amount of the commands */
#define AMOUNT_OF_REGISTERS 8 /* The amount of the registers */
#define AMOUNT_OF_GUIDANCE_TYPES 4 /* The amount of the guidance types */
#define NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY 2 /* The number of columns in the instruction and data arrays */
#define LENGTH_OF_INSTRUCTION_ARRAY 3996 /* The length of the instruction array */
#define LENGTH_OF_DATA_ARRAY 3996 /* The length of the data array */
#define FIRST_COLUMN 0 /* The index of the first column in the array */
#define SECOND_COLUMN 1 /* The index of the second column in the array */
#define LENGTH_OF_DOT 1 /* The length of the dot */
#define LENGTH_OF_EMPTY_LINE 0 /* The length of the empty line (if there it is not white chars in the line) */
#define NOT_IN_SPECIFIC_LINE 0 /* The error that happened, is not in a specific line, so we will not print the line number (we start to count from 1 the lines) */
#define NOT_IN_SPECIFIC_FILE NULL /* The error that happened, is not in a specific file, so we will not print the file name */
#define INDEX_OF_LAST_DIGIT_OF_OCTAL_WORD 4 /* The index of the last char in the octal word (we display all the encoding in 5 octal digits) */
#define AMOUNT_OF_BITS_OF_ENCODING_WORD 15 /* The amount of bits in the encoding word in our project */
#define AMOUNT_OF_BITS_FOR_DISPLAY_ONE_OCTAL_DIGIT 3 /* The amount of bits that we need to display one octal digit */
#define START_VALUE_OF_DIGIT 0 /* The start value of the octal digit (before we update the value) */
#define AMOUNT_OF_BITS_IN_BYTE 8 /* The amount of bits in a byte */
#define MAX_INDEX_OF_BIT_OF_BYTE 7 /* The maximum index of the bit in a byte */
#define BIT_ON 1 /* The value of on bit */

/* ---------------PROTOTYPES--------------- */

boolean is_empty_line(char *);
int get_length_of_file(char* ,int );
void set_name_file(char **, char *, char *);

boolean is_custom_whitespace(char ch) ;
/* ---------------END OF PROTOTYPES--------------- */


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
errors check_allocation(void* p){
    if (p == NULL){ /* if the allocation failed */ 
        print_errors(ALLOCATION_FAILED, NOT_IN_SPECIFIC_LINE, NOT_IN_SPECIFIC_FILE); /* print the error */
        return THERE_IS_AN_ERROR; /* return that there is an error */
    }
    return THERE_IS_NO_ERROR; /* return that there is no error */
}


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
errors check_file(FILE *file, char* filename){
    if (file == NULL) { /* if the file can't be opened */
        print_errors(CANT_OPEN_FILE, NOT_IN_SPECIFIC_LINE, filename); /* print the error */
        return THERE_IS_AN_ERROR;  /* return that there is an error */
    }
    return THERE_IS_NO_ERROR; /* return that there is no error */
}


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
errors get_the_full_name_of_file(char **final_name,char *file_name, char *file_extension){
    int length_of_file = get_length_of_file(file_name, strlen(file_extension)); /* get the length of the file (the length of the name + the length of the extension + 1 for the dot) */
    *final_name = (char *)malloc(length_of_file * sizeof(char)); /* allocate memory for the final name of the file */
    if (check_allocation(*final_name) == THERE_IS_AN_ERROR) {  /* if the allocation failed */
        print_errors(ALLOCATION_FAILED, NOT_IN_SPECIFIC_LINE, NOT_IN_SPECIFIC_FILE); /* print the error */
        return THERE_IS_AN_ERROR;}
    set_name_file(final_name, file_name, file_extension); /* set the final name of the file (the name + the extension) to the file name that we got(without the extension) */
    return THERE_IS_NO_ERROR; /* if there is no error */
}


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
boolean is_line_too_long(char *line) {
    if (strlen(line) == MAX_LENGTH_OF_LINE && line[MAX_LENGTH_OF_LINE-1 ] != '\n') { /* if the line is too long */
        return True;  /* return that the line is too long */
    }
    return False;  /* return that the line is not too long */
}


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
boolean is_not_relevent_line(char *line){
    if (line[FIRST_CHAR_IN_LINE] == ';') { /* if the line is a comment */
        return True; /* return that need to skip the line */
    } 
    if (is_empty_line(line) == True) { /* if the line is empty */
        return True; /* return that need to skip the line */
    }
    return False; /* return that need to skip the line */

}


/**
 * this function checks if the line is empty
 * 
 * parameters:
 *     line - the line that we want to check if it is empty or not
 * 
 * returns:
 *    True - if the line is empty
 *    False - if the line is not empty
 * 
 */
boolean is_empty_line(char *line) {
    int i;
    for (i = 0; i <strlen(line); i++) { /* go over all the chars in the line */
        if (isspace(line[i]) == False) { /* if there is a char that it is not a 'white char' */
            return False; /* return false because the line is not empty */
        }
    }
    return True; 
}


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
boolean is_a_save_word(char *word){
    const char *commands[] = { "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop" }; /* the saved commands */
    const char *registers_names[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7" }; /* the saved registers */
    const char *guidance_types[] = { ".data", ".string", ".entry", ".extern" }; /* the saved guidance types */
    int i;
    for (i = 0; i < AMOUNT_OF_COMMANDS; i++){ /* go over all the commands */
        if (strcmp(word, commands[i]) == EQUAL){
            return True;
        }
    }
    for (i = 0; i < AMOUNT_OF_REGISTERS; i++){ /* go over all the registers */
        if (strcmp(word, registers_names[i]) == EQUAL){
            return True;
        }
    }
    for (i = 0; i < AMOUNT_OF_GUIDANCE_TYPES; i++){ /* go over all the guidance types */
        if (strcmp(word, guidance_types[i]) == EQUAL){
            return True;
        }
    }
    if (strcmp(word,"macr") == EQUAL || strcmp(word, "endmacr") == EQUAL){ 
        return True;
    }
    return False; /* the word is not a save word , so return false */
}


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
boolean check_if_first_char_is_letter(char *word){
    return isalpha(word[FIRST_CHAR_IN_LINE]); /* return if the first character of the word is  a letter */ 
}


/**
 * this function checks if the word(probably the name of label or macro) has illegal characters
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
boolean check_if_word_contains_illegal_chars(char *word, boolean is_macro) {
    int i;
    for (i = 1; i < strlen(word); i++) { /* go over all the characters in the word */
        if (!isalpha(word[i]) && !isdigit(word[i]) ) { /* if the word has illegal characters */
            if (is_macro == True && word[i] == '_'){ /* if the it is macro name, it is okay that he has the character '_' */
                continue; /* continue because the name of the macro has legal characters */
            }
            return True; /* return True because the word has illegal characters */
        }
    }
    return False; /* return False */
}
 

/**
 * this function gets the required length of the file (the length of the name + the length of the extension + 1 for the dot)
 * 
 * parameters:
 *     file_name - the name of the file that we want to get the length of the file
 *     length_of_extension - the length of the extension of the file
 * 
 * returns:
 *    the final length of the file (the length of the name + the length of the extension + 1 for the dot)
 * 
 */
int get_length_of_file(char* file_name,int length_of_extension){
    return strlen(file_name) + length_of_extension + LENGTH_OF_DOT +1; /* return the length of the file (the length of the name + the length of the extension + 1 for the dot) + 1 for the null terminator */
}


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
int get_index_after_first_word(char line[]){ 
    int i = 0;
    while (line[i] && isspace(line[i]) != False){i++;} /* skip the spaces */
    while (line[i] && isspace(line[i]) == False){i++;} /* skip the first word */
    return i;
}


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
int get_index_of_second_word(char line[]){
    int i = 0;
    while (line[i] && isspace(line[i]) != False){i++;} /* skip the spaces */
    while (line[i] && isspace(line[i]) == False){i++;} /* skip the first word */
    while (line[i] && isspace(line[i]) != False){i++;} /* skip the spaces between the first and the second word */
    return i;
}


/**
 * this function sets the final name of the file (the name + the extension) to the file name that we got(without the extension)
 * 
 * parameters:
 *     ptr_to_name_of_file - the pointer to the new name of the file
 *     file_name - the name of the file that we want to set to the final  name
 *     file_extension - the required extension of the file
 * 
 */
void set_name_file(char **ptr_to_name_of_file, char *file_name, char *file_extension){
    strcpy(*ptr_to_name_of_file, file_name); /* copy the name of the file to the new name */
    strcat(*ptr_to_name_of_file, "."); /* add a dot to the new name */
    strcat(*ptr_to_name_of_file, file_extension); /* add the extension to the new name */
}


/**
 * this function initializes the data and instruction arrays to be empty 
 * 
 * parameters:
 *     instruction_array - the array that will store the instructions encoded lines
 *     data_array - the array that will store the data encoded lines
 * 
 */
void initialize_data_and_instruction_arrays(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char data_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY]){
    int i;
    for (i = 0; i < LENGTH_OF_INSTRUCTION_ARRAY; i++){ /* go over all the columns in the arrays */
        instruction_array[i][FIRST_COLUMN] = 0; /* initialize the first column in the instruction array */
        instruction_array[i][SECOND_COLUMN] = 0; /* initialize the second column in the instruction array */
    }
    for (i = 0; i < LENGTH_OF_DATA_ARRAY; i++){ /* go over all the columns in the arrays */
        data_array[i][FIRST_COLUMN] = 0; /* initialize the first column in the data array */
        data_array[i][SECOND_COLUMN] = 0; /* initialize the second column in the data array */
    }
}


/**
 * this function converts the machine code to octal base
 * 
 * parameters:
 *     machine_code - the machine code that we want to convert to octal base
 *     octal_word - the octal word that we want to store the octal base
 *     address - the address of the machine code that we want to convert to octal base
 * 
 */
void convert_machine_code_to_octal_base(char machine_code[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char *octal_word, int address) {
    int octal_digit_index = INDEX_OF_LAST_DIGIT_OF_OCTAL_WORD; /* the index of the last digit in the octal word (we display all the encoding in 5 octal digits) */
    int group;  /* the group of the bits that we want to convert to octal digit, we devide the 15 bits to 5 groups of 3 bits (beacuse 3 digits of binary number is equal to 1 digit of octal number) */
    int start_bit_of_octal_digit, end_bits_of_octal_digit; /* the start and the end bits of the octal digit that we want to convert */
    int octal_digit; /* the octal digit that we want to calculate */
    int bit, byte_index, bit_index; 
    for (group = 0; group < AMOUNT_OF_BITS_OF_ENCODING_WORD/AMOUNT_OF_BITS_FOR_DISPLAY_ONE_OCTAL_DIGIT; group++) { /* go over all the groups (all the 15 bits) */
        start_bit_of_octal_digit = AMOUNT_OF_BITS_OF_ENCODING_WORD - (group + 1) * AMOUNT_OF_BITS_FOR_DISPLAY_ONE_OCTAL_DIGIT; /* the start bit of the octal digit that we want to convert */
        end_bits_of_octal_digit = start_bit_of_octal_digit + AMOUNT_OF_BITS_FOR_DISPLAY_ONE_OCTAL_DIGIT; /* the end bit of the octal digit that we want to convert */
        octal_digit = START_VALUE_OF_DIGIT;
        for (bit = start_bit_of_octal_digit; bit < end_bits_of_octal_digit; bit++) {
            byte_index = bit / AMOUNT_OF_BITS_IN_BYTE; /* the index of the byte in the machine code */
            bit_index = bit % AMOUNT_OF_BITS_IN_BYTE; /* the index of the bit in the byte */
            if (machine_code[address][byte_index] & (BIT_ON << (MAX_INDEX_OF_BIT_OF_BYTE - bit_index))) {
                octal_digit |= (BIT_ON << (end_bits_of_octal_digit - bit - 1));
            }
        }
        octal_word[octal_digit_index--] = '0' + octal_digit; /* convert the octal digit to a character to store in the octal word */
    }
    octal_word[INDEX_OF_LAST_DIGIT_OF_OCTAL_WORD + 1] = '\0'; /* add the null terminator to the end of the octal word */
}