#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "create_output_files.h"
#include "help_methods.h"
#include "../enums/boolean.h"
#include "../enums/errors.h"
#include "../enums/guidance.h"
#include "../enums/operand_type.h"
#include "../enums/labels.h"
#include "../errors_handling/print_messages.h"
#include "../first_pass/help_methods_for_first_pass.h"
#include "../labels_structure/table_of_labels.h"

#define MAX_CHARS_IN_LINE 82 /* the maximum length of the line + 1 for the '\0' for the fgets function */
#define NOT_IN_SPECIFIC_LINE 0 /* the error that happened, is not in a specific line, so we will not print the line number */           
#define START_VALUE_OF_IC 0 /* the start value of IC */
#define FIRST_PARAMETER 1 /* the first parameter in the line */
#define SECOND_PARAMETER 2 /* the second parameter in the line */
#define START_ADRESS 100 /* the address where we start from */
#define LENGTH_OF_OCTAL_WORD 6 /* the length of the octal word is 5 and +1 for the null terminator */
#define START_ADRESS 100 /* the start address */

/* ---------------PROTOTYPES--------------- */

errors create_object_file(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char *, int , int);
errors create_entry_file(char *, table_of_labels *);
errors create_extern_file(char *, table_of_labels *);
boolean will_extern_file_be_empty(FILE *, table_of_labels *);
void write_all_the_uses_of_external_labels(FILE *, FILE *, table_of_labels *);
void handle_instruction_line_for_extern_file(FILE *, char *, int *, boolean *, table_of_labels *);

/* ---------------END OF PROTOTYPES--------------- */





/**
 * this function creates the object file after we finish to proccess the file, and sometimes create the entry file and the extern file if they are needed (if these files wont be empty)
 *   
 * parameters:
 *    instruction_array - the array of the instruction lines
 *    data_array - the array of the data lines
 *    file_name - the name of the file
 *    IC - the instruction counter
 *    DC - the data counter
 *    table_of_labels - the table of the labels
 *    allocation_faild_flag - the flag that indicates if there is an allocation error 
 * 
 * return:
 *   THERE_IS_AN_ERROR - if there is an error while creating the files
 *   THERE_IS_NO_ERROR - if there is no error while creating the files
 */
errors create_output_files_after_assembler_processing(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char data_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char *file_name, int IC, int DC, table_of_labels *table_of_labels, boolean *allocation_faild_flag){
    if (create_object_file(instruction_array, data_array, file_name, IC, DC) == THERE_IS_AN_ERROR){ /* try to create the object file */
        print_errors(CANT_CREATE_OBJECT_FILE, NOT_IN_SPECIFIC_LINE, file_name);
        *allocation_faild_flag = True;
        return THERE_IS_AN_ERROR;
    }
    if (create_entry_file(file_name, table_of_labels) == THERE_IS_AN_ERROR){ /* try to create the entry file */
        print_errors(CANT_CREATE_ENTRY_FILE, NOT_IN_SPECIFIC_LINE, file_name);
        *allocation_faild_flag = True;
        return THERE_IS_AN_ERROR;
    }
    if (create_extern_file(file_name, table_of_labels) == THERE_IS_AN_ERROR){ /* try to create the extern file */
        print_errors(CANT_CREATE_EXTERN_FILE, NOT_IN_SPECIFIC_LINE, file_name);
        *allocation_faild_flag = True;
        return THERE_IS_AN_ERROR;
    }
    return THERE_IS_NO_ERROR; /* return that there is no error while creating the files */
}


/**
 * this function creates the object file after we finish to proccess the file, this file contains the encoding of the file argument
 * 
 * parameters:
 *    instruction_array - the array of the instruction lines
 *    data_array - the array of the data lines
 *    IC - the instruction counter
 *    DC - the data counter
 *    file_name - the name of the file
 * 
 * return:
 *   THERE_IS_AN_ERROR - if there is an error while creating the object file
 *   THERE_IS_NO_ERROR - if there is no error while creating the object file
 */
errors create_object_file(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char data_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char *file_name, int IC, int DC){
    FILE *object_file;
    char *final_name;
    char octal_word[LENGTH_OF_OCTAL_WORD]; /* the octal word that we want to print to the file */
    int i;
    get_the_full_name_of_file(&final_name, file_name, "ob"); /* get the full name of the file */
    object_file = fopen(final_name, "w"); /* open the file */
    if (check_file(object_file, final_name) == THERE_IS_AN_ERROR) { /* check if there is an error while opening the file */
        free(final_name);
        fclose(object_file);
        return THERE_IS_AN_ERROR;
    }
    fprintf(object_file, "  %d  %d", IC, DC); /* print the info anout the encoding - the title of the object file */
    for (i = 0; i < IC; i++){ /* go through all the instruction lines */
        fprintf(object_file,"\n%04d ", i + START_ADRESS); /* print the address + 1 space to keep the format */
        convert_machine_code_to_octal_base(instruction_array, octal_word, i);  /* convert the encoded instruction line to octal base */
        fprintf(object_file, "%s", octal_word); /* print the instruction line encoded in octal base */
    }
    for (i = 0; i < DC; i++){ /* go through all the data lines */
        fprintf(object_file,"\n%04d ", i + IC + START_ADRESS); /* print the address + 1 space to keep the format */
        convert_machine_code_to_octal_base(data_array, octal_word, i);  /* convert the encoded guidance line to octal base */
        fprintf(object_file, "%s", octal_word); /* print the data line encoded in octal base */

    }
    free(final_name); /* free the final name */
    fclose(object_file); /* close the file */
    return THERE_IS_NO_ERROR; /* return that there is no error */ 
}


/**
 * this function creates the entry file after we finish to proccess the file, this file contains the entry labels and their addresses (if there are any)
 * 
 * note: we know that the maximum length of the label is 31 chars, so if beacuse it, we will will align the text according to this case - the maximum length of the label that can be and we will add a few more spaces to keep the format
 * 
 * parameters:
 *    file_name - the name of the file
 *    table - the table of the labels
 * 
 * return:
 *   THERE_IS_AN_ERROR - if there is an error while creating the entry file
 *   THERE_IS_NO_ERROR - if there is no error while creating the entry file
 */
errors create_entry_file(char *file_name, table_of_labels *table){
    char *final_name;
    FILE *entry_file;
    if(is_there_at_least_one_label_with_the_specfic_type(table, entry) == True){ /* check if there is at least one label with the type of entry which means the file wont be empty */
        get_the_full_name_of_file(&final_name, file_name, "ent"); /* get the full name of the file */
        entry_file = fopen(final_name, "w"); /* open the file for writing */
        if (check_file(entry_file,final_name) == THERE_IS_AN_ERROR) { /* check if there is an error while opening the file */
            free(final_name);
            fclose(entry_file);
            return THERE_IS_AN_ERROR;
        }
        write_all_the_labels_that_with_the_specific_type(entry_file, entry, table); /* write all the labels with the type of entry to the file */
        free(final_name); /* free the allocated memory of the final name of the file */
        fclose(entry_file); /* close the file */
    }
    return THERE_IS_NO_ERROR; /* return that there is no error */
}


/**
 * this function creates the extern file after we finish to proccess the file, this file contains the extern labels and their address of the encoding of them (if there are any)
 * 
 * parameters:
 *    file_name - the name of the file
 *    table - the table of the labels
 * 
 * return:
 *   THERE_IS_AN_ERROR - if there is an error while creating the extern file
 *   THERE_IS_NO_ERROR - if there is no error while creating the extern file
 */
errors create_extern_file(char *file_name, table_of_labels *table){
    char *final_name_of_extern_file, *final_name_of_after_pre_assembler;
    FILE *extern_file, *after_pre_assembler_file;
    errors error = THERE_IS_NO_ERROR;
    get_the_full_name_of_file(&final_name_of_after_pre_assembler, file_name, "am"); /* get the full name of the after pre assembler file */
    after_pre_assembler_file = fopen(final_name_of_after_pre_assembler, "r"); /* open the file for reading */
    if(will_extern_file_be_empty(after_pre_assembler_file, table) == False){ /* check if the extern file will be empty */
        error = THERE_IS_AN_ERROR;
        get_the_full_name_of_file(&final_name_of_extern_file, file_name, "ext"); /* get the full name of the extern file */
        extern_file = fopen(final_name_of_extern_file, "w"); /* open the file for writing */
        if (check_file(extern_file,final_name_of_extern_file) == THERE_IS_NO_ERROR && check_file(after_pre_assembler_file,final_name_of_after_pre_assembler) == THERE_IS_NO_ERROR) { /* check if there is an error while opening the files */
            error = THERE_IS_NO_ERROR; /* there is no error */
            write_all_the_uses_of_external_labels(extern_file, after_pre_assembler_file, table); /* write all the uses of the external labels to the file */
        }
        free(final_name_of_extern_file);
        fclose(extern_file);
    }
    free(final_name_of_after_pre_assembler);
    fclose(after_pre_assembler_file);
    return error; /* return if there is an error or not */
}


/**
 * this function checks if the extern file will be empty or not
 * 
 * parameters:
 *    file - the file that we want to check if it will be empty
 *    table - the table of the labels
 * 
 * returns:
 *    True - if the file will be empty
 *    False - if the file will not be empty
 */
boolean will_extern_file_be_empty(FILE *file, table_of_labels *table){
    char line[MAX_CHARS_IN_LINE], copy_line[MAX_CHARS_IN_LINE];
    operand_type type_of_first_operand_in_the_instruction_line, type_of_second_operand_in_the_instruction_line; /* the type of the first and the second operand in the instruction line */
    char *first_operand_name, *second_operand_name;
    while (fgets(line, MAX_CHARS_IN_LINE, file)){ /* read the line from the file */
        if (is_label_definition(line) == True){ /* check if the line is a label definition */
            change_the_line_to_be_without_label_definition(line); /* get the line without the label */
        } 
        strcpy(copy_line, line); /* copy the line to the copy line */
        if (get_guidance_type(line) == no_guidance){ /* means it is an instruction line */
            strtok(copy_line, " ,\t\n\r\f\v"); /* skip the first word */
            first_operand_name = strtok(NULL, " ,\t\n\r\f\v"); /* get the first operand */
            second_operand_name = strtok(NULL, " ,\t\n\r\f\v"); /* get the second operand */
            type_of_first_operand_in_the_instruction_line = get_parameter_type(line, FIRST_PARAMETER); /* get the type of the first operand in the instruction line */
            type_of_second_operand_in_the_instruction_line = get_parameter_type(line, SECOND_PARAMETER); /* get the type of the second operand in the instruction line */
            if (type_of_first_operand_in_the_instruction_line == label && get_type_of_label(first_operand_name, table) == external){ /* check if the first operand is a label and if it is external */
                return False; /* we found a usage of an external label, which means the file wont be empty */
            }
            if (type_of_second_operand_in_the_instruction_line == label && get_type_of_label(second_operand_name, table) == external){ /* check if the second operand is a label and if it is external */
                return False; /* we found a usage of an external label, which means the file wont be empty */
            }
        }
    }
    return True; /* we didn't find any usage of an external label, which means the file will be empty */
}


/**
 * this function responsible to write all the labels with the specific type to the file
 * 
 * parameters:
 *    file - the file that we want to write the labels to
 *    table - the table of the labels
 *    type_of_the_label - the type of the label that we want to write to the file
 */
void write_all_the_uses_of_external_labels(FILE *extern_file, FILE *after_pre_assembler_file, table_of_labels *table){
    char line[MAX_CHARS_IN_LINE];
    int IC = START_VALUE_OF_IC; /* the start value of IC */
    boolean is_first_line = True; /* the flag that indicates if it is the first line that will be in the extern file */
    rewind(after_pre_assembler_file); /* go to the start of the file */
    while (fgets(line, MAX_CHARS_IN_LINE, after_pre_assembler_file)){ /* read the line from the file */
        if (is_label_definition(line) == True){ /* check if the line is a label definition */
            change_the_line_to_be_without_label_definition(line); /* get the line without the label */
        } 
        if (get_guidance_type(line) == no_guidance){ /* means it is an instruction line */
            handle_instruction_line_for_extern_file(extern_file, line, &IC, &is_first_line, table);
        }
    }
}

/**
 * this function gets a instruction line and check if the first or the second operands are external labels, if they are, we write the external label and the adress of the encoding label to the extern file
 * 
 * note: we know that the maximum length of the label is 31 chars, so if beacuse it, we will will align the text according to this case - the maximum length of the label that can be and we will add a few more spaces to keep the format
 * parameters:
 *    file - the file that we want to write the labels to
 *    line - the line that we want to check if it has external labels
 *    IC - the instruction counter
 *    is_first_line - the flag that indicates if it is the first line that will be in the extern file
 *    table - the table of the labels
 */
void handle_instruction_line_for_extern_file(FILE *extern_file, char line[], int *IC, boolean *is_first_line ,table_of_labels *table){
    char *first_operand_name, *second_operand_name;
    operand_type type_of_first_operand_in_the_instruction_line, type_of_second_operand_in_the_instruction_line;
    type_of_first_operand_in_the_instruction_line = get_parameter_type(line, FIRST_PARAMETER); /* get the type of the first operand in the instruction line */
    type_of_second_operand_in_the_instruction_line = get_parameter_type(line, SECOND_PARAMETER); /* get the type of the second operand in the instruction line */
    (*IC)++;
    strtok(line, " ,\t\n\r\f\v"); /* skip the first word */
    first_operand_name = strtok(NULL, " ,\t\n\r\f\v"); /* get the first operand */
    second_operand_name = strtok(NULL, " ,\t\n\r\f\v"); /* get the second operand */
    if (type_of_first_operand_in_the_instruction_line == label && get_type_of_label(first_operand_name, table) == external){ /* check if the first operand is a label and if it is external */
        if (*is_first_line == True){ 
            fprintf(extern_file, "%-35s%04d", first_operand_name, *IC + START_ADRESS); /* print the name of the extern label and the adress where it is used according to a specific format */
            *is_first_line = False;
            }
        else{
            fprintf(extern_file, "\n%-35s%04d", first_operand_name, *IC + START_ADRESS); /* print the name of the extern label and the adress where it is used according to a specific format */
        }
    }
    if (first_operand_name != NULL){(*IC)++;} /* if the first operand name is not NULL, we need to increase the IC by 1 */
    if (type_of_second_operand_in_the_instruction_line == label && get_type_of_label(second_operand_name, table) == external){
        if (*is_first_line == True){
            fprintf(extern_file, "%-35s%04d", first_operand_name, *IC + START_ADRESS); /* print the name of the extern label and the adress where it is used according to a specific format */
            *is_first_line = False;
        }
        else{
            fprintf(extern_file, "\n%-35s%04d", second_operand_name, *IC + START_ADRESS); /* print the name of the extern label and the adress where it is used according to a specific format */
        }
    }
    if (second_operand_name != NULL){(*IC)++;} /* if the second operand name is not NULL, we need to increase the IC by 1 */
    if (type_of_first_operand_in_the_instruction_line == reg && type_of_second_operand_in_the_instruction_line == reg){
        (*IC)--; /* if the instruction has two registers, we need to decrease the IC by 1 */
    }
}