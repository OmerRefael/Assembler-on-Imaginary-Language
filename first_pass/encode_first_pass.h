#ifndef ENCODE_FIRST_PASS_H
#define ENCODE_FIRST_PASS_H

#define NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY 2

/**
 * this function encodes the string guidance line
 * 
 * parameters:
 *    data_array - the array that will hold the encoded data lines
 *    line - the string guidance line that we want to encode
 *    DC - the data counter
 * 
 */
void encode_string_guidance(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char [], int *);
/**
 * this function encodes the data guidance line
 * 
 * parameters:
 *    data_array - the array that will hold the encoded data lines
 *    line - the data guidance line that we want to encode
 *    DC - the data counter
 * 
 */
void encode_data_guidance(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY],char [], int *);
/**
 * this function encodes the instruction line
 * 
 * parameters:
 *    instruction_array - the array that will hold and sotre the encoded instruction lines
 *    line - the instruction line that we want to encode
 *    IC - the instruction counter
 * 
 */
void encode_instruction(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char[], int *);


#endif