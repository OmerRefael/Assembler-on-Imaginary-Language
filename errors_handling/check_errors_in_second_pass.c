#include "check_errors_in_second_pass.h"
#include "print_messages.h"
#include "../enums/errors.h"
#include "../enums/labels.h"
#include "../enums/boolean.h"
#include "../labels_structure/table_of_labels.h"


/**
 * this function checks if there is an error in the entry guidance line (error that can be checked just in the second pass)
 * 
 * parameters:
 *     label_name - the name of the label of the entry guidance
 *     file_name - the name of the file that the entry guidance is in
 *     number_line - the number of the line that the entry guidance is in
 *     table - the table of labels
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if the label of the entry guidance is defined
 *    THERE_IS_AN_ERROR - if the label of the entry guidance is not defined
 * 
 */
errors check_errors_of_entry_guidance_second_pass(char label_name[], char *file_name, int number_line, table_of_labels *table){
    label_type type_of_label = get_type_of_label(label_name, table); /* get the type of the label */
    errors error = THERE_IS_NO_ERROR;  /* the type of error */
    if (type_of_label == illegal_type){ /* this means that the label is declerd but not defined */
        error = LABEL_OF_ENTRY_GUIDANCE_THAT_DECLARED_BUT_NOT_DEFINED; /* set the error */
    }
    else if (type_of_label == external){ /* the label defined as entry and also as external in other line */
        error = LABEL_THAT_ENTRY_AND_ALSO_EXTERNAL; /* set the error */
    }

    if (error != THERE_IS_NO_ERROR){ /* if there is an error */
        print_errors(error, number_line, file_name); /* print the error message */
        return THERE_IS_AN_ERROR; /* return that there is an error */
    }
    return THERE_IS_NO_ERROR; /* return that there is no error */

}


/**
 * this function checks if there is an error in the label operand of the instruction line (error that can be checked just in the second pass)
 * 
 * parameters:
 *     label_name - the name of the label of the operand
 *     file_name - the name of the file that the instruction line is in
 *     number_line - the number of the line that the instruction line is in
 *     table - the table of labels
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if the label of the operand is defined
 *    THERE_IS_AN_ERROR - if the label of the operand is not defined
 * 
 */
errors check_errors_of_label_operand_of_instruction_line(char label_name[], char *file_name, int number_line, table_of_labels *table){
    if (is_exist_label(label_name, table) == False){ /* this means that the label is declerd but not defined */
        print_errors(LABEL_THAT_NOT_DEFINED, number_line, file_name); /* print the error message */
        return THERE_IS_AN_ERROR; /* return that there is an error */
    }
    return THERE_IS_NO_ERROR; /* return that there is no error */
}