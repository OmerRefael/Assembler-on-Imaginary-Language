#include <stdio.h>

#include "encode_second_pass.h"
#include "../enums/encoding_type.h"
#include "../enums/labels.h"

#define NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY 2 /* The number of columns in the instruction and data array */
#define FIRST_COLUMN 0 /* The first column in the  array */
#define SECOND_COLUMN 1 /* The second column in the  array */
#define VALUE_TO_SHIFT_FOR_ENCODING_TYPE_BITS 1 /* The value that we need to shift for change the bits that represent the encoding type */
#define VALUE_TO_SHIFT_FOR_LABEL_BITS_OF_THAT_IN_SECOND_COLUMN 4 /* The value that we need to shift for change the bits that represent the label */
#define VALUE_TO_SHIFT_FOR_LABEL_BITS_OF_THAT_IN_FIRST_COLUMN 4 /* The value that we need to shift for change the bits that represent the label */

/* ---------------PROTOTYPES--------------- */

void encode_relocatable_type(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], int );
void encode_external_type(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], int );

/* ---------------END OF PROTOTYPES--------------- */

/**
 * this function encode the label in the instruction array(we can to encode the label just in the second pass)
 * 
 * parameters:
 *    instruction_array - the array that contains the instructions
 *    adress_of_label - the adress of the label
 *    IC - the instruction counter
 *    type_of_the_label - the type of the label (for encoding the label - external or relocatable type)
 */
void encode_label(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], int adress_of_label, int IC, label_type type_of_the_label){
    instruction_array[IC][SECOND_COLUMN] |= adress_of_label << VALUE_TO_SHIFT_FOR_LABEL_BITS_OF_THAT_IN_SECOND_COLUMN; /* set the adress of the label in the second column */
    adress_of_label = adress_of_label >> VALUE_TO_SHIFT_FOR_LABEL_BITS_OF_THAT_IN_FIRST_COLUMN; /* shift the adress of the label to the right */
    instruction_array[IC][FIRST_COLUMN] |= adress_of_label; /* set the adress of the label in the first column */
    if (type_of_the_label == external){
        encode_external_type(instruction_array, IC); /* encode the external type */
    }
    else{
        encode_relocatable_type(instruction_array, IC); /* encode the relocatable type */
    }
} 


/**
 * this function encodes the relocatable type 
 *  
 * parameters:
 *    instruction_array - the array that will hold and sotre the encoded instruction lines
 *    IC - the instruction counter
 * 
 */
void encode_relocatable_type(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], int IC){
    encoding_type R = relocatable_type;  /* the encoding type is relocatable */
    instruction_array[IC][SECOND_COLUMN] |= R << VALUE_TO_SHIFT_FOR_ENCODING_TYPE_BITS; /* encode the encoding type */
}

/**
 * this function encodes the external type 
 *  
 * parameters:
 *    instruction_array - the array that will hold and sotre the encoded instruction lines
 *    IC - the instruction counter
 * 
 */
void encode_external_type(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], int IC){
    encoding_type E = external_type;  /* the encoding type is external */
    instruction_array[IC][SECOND_COLUMN] |= E << VALUE_TO_SHIFT_FOR_ENCODING_TYPE_BITS; /* encode the encoding type */
}