#ifndef ENCODE_SECOND_PASS_H
#define ENCODE_SECOND_PASS_H

#include "../enums/labels.h"

#define NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY 2

/**
 * this function encode the label in the instruction array(we can to encode the label just in the second pass) 
 * 
 * parameters:
 *    instruction_array - the array that contains the instructions
 *    adress_of_label - the adress of the label
 *    IC - the instruction counter
 *    type_of_the_label - the type of the label (for encoding the label - external or relocatable type)
 */
void encode_label(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], int , int , label_type);

#endif