#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "first_pass.h"
#include "encode_first_pass.h"
#include "help_methods_for_first_pass.h"
#include "../labels_structure/table_of_labels.h"
#include "../enums/errors.h" 
#include "../enums/warnings.h"
#include "../enums/boolean.h"
#include "../enums/labels.h"
#include "../enums/guidance.h"
#include "../errors_handling/check_errors_in_first_pass.h"
#include "../errors_handling/print_messages.h"
#include "../util/help_methods.h"
#include "../pre_assembler/macros_structure/table_of_macros.h"

#define NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY 2 /* The number of columns in the instruction and data arrays */
#define FILE_EXTENSION_LENGTH_AFTER_PRE_ASSEMBLER 2 /* the length of the file extension after the pre assembler - "am" */
#define MAX_CHARS_IN_LINE 82 /* the maximum length of the line + 1 for the '\0' for the fgets function */
#define MAX_CHARS_OF_LABEL 31 /* the maximum length of the label */
#define START_ADDRESS 100 /* the start address */
#define ADRESS_OF_EXTERN_LABEL 0 /* the address of the extern label */
#define FIRST_LINE 1 /* the first line in the file (we start counting from 1) */
#define EQUAL 0 /* for indicating that two strings are equal */

/* ---------------PROTOTYPES--------------- */

errors open_files_first_pass(FILE **, char **, char *, boolean *);
void handle_label(char [], char *, int, int , int , guidance, errors *, table_of_labels **, table_of_macros *, boolean *);
void handle_data_and_string_guidance(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char [], char *, int, int *, errors *, guidance);
void handle_string_guidance(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char [], char *, int, int *, errors *);
void handle_data_guidance(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char [], char *, int, int *, errors *);
void handle_entry_and_extern_guidance(char [], char *, int, guidance, errors *, table_of_labels **, boolean *);
void handle_instruction(char [][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char [], char *, int, int *, errors *, boolean);
void finish_first_pass(FILE *, char *);

/* ---------------END OF PROTOTYPES--------------- */





/**
 * this function performs the first pass of the assembler on the given file. it checks for errors in the file,
 * encodes the file into machine code, and stores the machine code of the instructions and data in the provided arrays.
 * additionally, it handles the labels.
 * 
 * note: when i check errors in the entry and the extern guidance, i dont check if there are errors in the name of the label(the content of the label), because
 *  according to the project's instructions, for extern guidance we dont need to check if there are errors in the name of the label, and for the entry guidance, we will check the content where it 
 *  is defined (and not where it is declared) (and if it is just decleared, but not defined, we will check it in the second pass).
 * 
 * parameters:
 *    instruction_array - the array that we store the machine code of the instructions
 *    data_array - the array that we store the machine code of the data
 *    file_name - the name of the file that we are checking
 *    IC - the instruction counter
 *    DC - the data counter
 *    labels_table - the table of labels that we will store the labels in it
 *    macros_table - the table of macros that we will store the macros in it
 *    allocation_faild_flag - the flag that indicates if there is an allocation error 
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error in the first pass
 *    THERE_IS_AN_ERROR - if there is an error in the first pass
 */
errors first_pass(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY],char data_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY],char *file_name, int *IC, int *DC, table_of_labels **labels_table, table_of_macros *macros_table, boolean *allocation_faild_flag){
    FILE *file; /* the file that we are checking */
    char line[MAX_CHARS_IN_LINE]; /* the line that we are reading from the file */
    char *final_name; /* the final name of the file that we are checking */
    int number_line = FIRST_LINE; /* the number of the line that we are checking */
    errors is_error = THERE_IS_NO_ERROR; /* for indicating if there is an error in the first pass */
    guidance guidance_type; /* the guidance type of the line (data, string, entry, extern, no guidance) */
    boolean has_label_definition = False; /* for indicating if there is a label definition in the line */
    boolean is_memory_exceeded = False; /* for indicating if we exceed the memory limit */
    if (open_files_first_pass(&file, &final_name, file_name, allocation_faild_flag) == THERE_IS_AN_ERROR){ /* open the file and check if there is an error (if so, the function will handle and free all the memory + print the error) */
        return THERE_IS_AN_ERROR;
    }

    while (fgets(line, MAX_CHARS_IN_LINE, file) && *allocation_faild_flag == False && is_memory_exceeded == False){ /* read the file line by line untill the end of the file or until there is an allocation error or until we exceed the memory limit */
        guidance_type = get_guidance_type(line); /* get the guidance type of the line */
        if (is_label_definition(line) == True){ /* if there is a label definition in the line */
            has_label_definition = True; /* indicate that there is a label definition in the line */
            handle_label(line, final_name, number_line, *IC, *DC, guidance_type, &is_error, labels_table, macros_table, allocation_faild_flag); /* handle the label definition */
            change_the_line_to_be_without_label_definition(line); /* change the line to be without the label definition */
        }
        if (guidance_type == data_guidance || guidance_type == string_guidance){ /* if it is a data or string guidance */
            handle_data_and_string_guidance(data_array, line, final_name, number_line, DC, &is_error, guidance_type );
        }
        else if (guidance_type == entry_guidance || guidance_type == extern_guidance){ /* if it is an entry or extern guidance */
            handle_entry_and_extern_guidance(line, final_name, number_line, guidance_type, &is_error, labels_table, allocation_faild_flag); 
        }
        else{ /*assume that is a instruction line, and if it is not, the function will handle it */
            handle_instruction(instruction_array, line, final_name, number_line, IC, &is_error, has_label_definition);
        }
        has_label_definition = False; /* after we handle the label definition, we set the flag to be False */
        number_line++; /* increase the number of the line */
        check_exceeding_memory_limit(final_name, *IC, *DC, &is_memory_exceeded, &is_error); /* check if we exceed the memory limit - 4096 cells */
    }
    update_all_the_adresses_of_data_labels(*IC, *labels_table); /* update all the adresses of data labels, so the address of the data labels will be the address of the data labels + IC (after we add the start adress - 100) */
    finish_first_pass(file, final_name); /* free all the memory and close all the files */
    return is_error;  
}


/**
 * this function opens the file that we are checking, and check if there is an error to open the file,
 * also it gets the full name of the file with the extension after we add the correct extension to the name of the file
 * 
 * parameters:
 *    input_file - the pointer to the file that we are checking
 *    final_name - the final name of the file that we are setting in the function
 *    file_name - the name of the file that we are getting
 *    allocation_faild_flag - the flag that indicates if there is an allocation error 
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error to open the file
 *    THERE_IS_AN_ERROR - if there is an error to open the file
 */
errors open_files_first_pass(FILE **input_file, char **final_name, char *file_name, boolean *allocation_faild_flag){
    get_the_full_name_of_file(final_name, file_name, "am"); /* get the full name of the input file with the extension after we add the correct extension to the name of the file */
    *input_file = fopen(*final_name, "r"); /* open the input file just for reading */
    if (check_file(*input_file,*final_name) == THERE_IS_AN_ERROR) { /* check if there is an error to open the file*/
        finish_first_pass(*input_file, *final_name); /* call to function that free all the memory and close all the files */
        *allocation_faild_flag = True; /* indicate that there is an allocation error */
        return THERE_IS_AN_ERROR; /* return that there is an error */
    } 
    return THERE_IS_NO_ERROR; /* return that there is no error */

}


/**
 * this function checks if the label is valid, according to the rules of the label:
 * 1. the label must not be empty.
 * 2. the label must not be too long.
 * 3. the label must not to be a saved word.
 * 4. the label must contain only letters and digits(in contrast to the macro name that also can contain '_').
 * 5. the label must to start with a letter.
 * 6. after the label definition, there must a space (after the colon).
 * 7. the label must not be a macro name.
 * 8. the label must not be a label that already exists in the table of labels.
 * 
 * parameters:
 *    label_name - the label name that we want to check.
 *    file_name - the name of the file that we are checking.
 *    number_line - the number of the line that we are checking.
 *    error - the error that we want to update if there is an error in the label.
 *    labels_table - the table of labels that we want to check if the label is already exist in it.
 *    macros_table - the table of macros that we want to check if the label is a macro.
 *    allocation_faild_flag - the flag that indicates if there is an allocation error .
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if the label is valid.
 *    THERE_IS_AN_ERROR - if the label is not valid.
 */
void handle_label(char line[], char *file_name, int number_line, int IC, int DC, guidance guidance_type, errors *error, table_of_labels **labels_table, table_of_macros *macros_table, boolean *allocation_faild_flag){
    char copy_line[MAX_CHARS_IN_LINE];
    char *label_name;
    strcpy(copy_line, line); /* copy the line to a new string */
    label_name = strtok(copy_line, " :\t\n\r\v\f"); /* get the label name */
    if (check_errors_in_label(line, label_name, file_name, number_line, *labels_table, macros_table) == THERE_IS_AN_ERROR){ /* check if there are errors in the label definition */
        *error = THERE_IS_AN_ERROR; /* if there are errors in the label, set the error to be THERE_IS_AN_ERROR */
        return;
    }
    if (guidance_type == no_guidance){ /* we assume that it is an instruction */
        add_label_to_table(label_name, IC + START_ADDRESS, error, code, labels_table, allocation_faild_flag); /* add the label to the table of labels */ 
    }
    else if (guidance_type == data_guidance || guidance_type == string_guidance){ /* if it is a data or string guidance */
        add_label_to_table(label_name, START_ADDRESS + DC, error, data,labels_table, allocation_faild_flag);  /* add the label to the table of labels */
    }
    else{ /* if it is an entry or extern guidance, we ignore them and even print a warning message */
        warnings warning;
        if (guidance_type == entry_guidance){
            warning = ENTRY_GUIDANCE_WITH_LABEL_DEFINITION;
        }
        else{
            warning = EXTERN_GUIDANCE_WITH_LABEL_DEFINITION;
        }
        print_warnings(warning, number_line, file_name); /* print a warning that there is an entry guidance with a label definition */
    }
}


/**
 * this function handles the data and string guidance, and calls the function that treats sperately the string guidance and the data guidance
 * 
 * parameters:
 *    data_array - the data array that we store the machine code of 
 *    line - the string or data guidance line that we need to handle
 *    file_name - the name of the file that we are checking
 *    number_line - the number of the line that we are checking
 *    DC - the data counter
 *    error - the pointer to the error flag
 *    guidance_type - the type of the guidance of the line
 * 
 */
void handle_data_and_string_guidance(char data_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char line[], char *file_name, int number_line, int *DC , errors *error, guidance guidance_type){
    if (guidance_type == string_guidance){ /* if it is a string guidance */
        handle_string_guidance(data_array, line, file_name, number_line, DC, error);
    }
    else{ /* it is a data guidance */
        handle_data_guidance(data_array, line, file_name, number_line, DC, error);
    }   
}


/**
 * this function handles the string guidance, and check if there are errors in the string guidance, if not, the function will encode the string guidance and add it to the data array
 * 
 * parameters:
 *    data_array - the data array that we store the machine code of 
 *    line - the string guidance line that we need to handle
 *    file_name - the name of the file that we are checking
 *    number_line - the number of the line that we are checking
 *    DC - the data counter
 *    error - the pointer to the error flag
 * 
 */
void handle_string_guidance(char data_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char line[], char *file_name, int number_line, int *DC , errors *error){
    if (check_errors_in_string_guidance(line, file_name, number_line) == THERE_IS_NO_ERROR){ /* check if there are no errors in the string guidance line*/
        encode_string_guidance(data_array, line, DC); /* encode the string guidance, and add it to the data array */
    }
    else{
        *error = THERE_IS_AN_ERROR; /* there are errors in the string guidance line, set the error to be THERE_IS_AN_ERROR */
    }
}


/**
 * this function handles the data guidance, and check if there are errors in the data guidance, if not, the function will encode the data guidance and add it to the data array
 * 
 * parameters:
 *    data_array - the data array that we store the machine code of 
 *    line - the data guidance line that we need to handle
 *    file_name - the name of the file that we are checking
 *    number_line - the number of the line that we are checking
 *    DC - the data counter
 *    error - the pointer to the error flag
 * 
 */
void handle_data_guidance(char data_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char line[], char *file_name,  int number_line, int *DC , errors *error){
    if (check_errors_in_data_guidance(line, file_name, number_line) == THERE_IS_NO_ERROR){ /* check if there are no errors in the data guidance line */
        encode_data_guidance(data_array, line, DC); /* encode the data guidance, and add it to the data array */
    }
    else{
        *error = THERE_IS_AN_ERROR; /* there are errors in the data guidance line, set the error to be THERE_IS_AN_ERROR */
    }
}


/**
 * this function handles the entry and extern guidance, and check if there are errors in the entry and extern guidance, if not, the function will add the extern label to the table of labels
 * note: all the rules of defined label are also applied to the entry and extern labels, but according to the project's instructions, for extern guidance we dont need to check if there are errors in the name of the label
 * and  for the entry guidance, we will check the content where it is defined (and not where it is declared) (and if it is just decleared, but not defined, we will check it in the second pass).
 *  
 * parameters:
 *    line - the entry or extern guidance line that we need to handle
 *    file_name - the name of the file that we are checking
 *    number_line - the number of the line that we are checking
 *    guidance_type - the type of the guidance of the line
 *    error - the pointer to the error flag
 *    labels_table - the table of labels that we will store the labels in it
 *    allocation_faild_flag - the flag that indicates if there is an allocation error 
 * 
 */
void handle_entry_and_extern_guidance(char line[], char *file_name, int number_line, guidance guidance_type, errors *error, table_of_labels **labels_table, boolean *allocation_faild_flag){
    if (check_errors_in_entry_and_extern_guidance(line, file_name, number_line) == THERE_IS_NO_ERROR) { /* check if there are no errors in the entry or extern guidance line */
        if (guidance_type == extern_guidance){ /* if it is an extern guidance */
            char copy_line[MAX_CHARS_IN_LINE];
            char label_of_entry_or_extern_word[MAX_CHARS_OF_LABEL];
            strcpy(copy_line, line); /* copy the line to a new string */
            strtok(copy_line, " :\t\n\r\v\f"); /* skip the entry or extern word - ".entry" or ".extern" */
            strcpy(label_of_entry_or_extern_word, strtok(NULL, " :\t\n\r\v\f")); /* get the label of the entry or extern word */
            add_label_to_table(label_of_entry_or_extern_word, ADRESS_OF_EXTERN_LABEL, error, external, labels_table,allocation_faild_flag); /* add the extern label to the table of labels */
        }
    }
    else{
        *error = THERE_IS_AN_ERROR; /* there are errors in the entry or extern guidance line, set the error to be THERE_IS_AN_ERROR */
    }
}


/**
 * this function handles the instruction line, and check if there are errors in the instruction line, if not, the function will encode the instruction and add it to the instruction array
 * 
 * parameters:
 *    instruction_array - the instruction array that we store the machine code of 
 *    line - the instruction line that we need to handle
 *    IC - the instruction counter
 *    error - the pointer to the error flag
 *    has_label_definition - the flag that indicates if there is a label definition in the line
 * 
 */
void handle_instruction(char instruction_array[][NUMBER_OF_COLUMNS_FOR_INSTRUCTION_AND_DATA_ARRAY], char line[], char *file_name, int number_line, int *IC, errors *error, boolean has_label_definition){
    char copy_line[MAX_CHARS_IN_LINE];
    strcpy(copy_line, line); /* copy the line to a new string */
    if (has_label_definition == True && strtok(copy_line,  " \t\n\r\v\f") == NULL){ /* if there is a label definition with no content after the label definition (remember that we removed the label definition before) */
        print_errors(LABEL_WITHOUT_CONTENT, number_line, file_name); /* print an error message that there is a label definition with no content after the label definition */
        *error = THERE_IS_AN_ERROR; /* set the error to be THERE_IS_AN_ERROR */

    }
    else{
        if (check_errors_in_instruction_line(line, file_name, number_line) == THERE_IS_NO_ERROR){ /* check if there are no errors in the instruction line */
            encode_instruction(instruction_array, line, IC); /* encode the instruction, and add it to the instruction array */
        }
        else{
            *error = THERE_IS_AN_ERROR; /* there are errors in the instruction line, set the error to be THERE_IS_AN_ERROR */
        }
    }
}


/**
 * this function frees all the memory and close all the files
 * 
 * parameters:
 *    file - the file that we are closing
 *    final_name - the final name of the file that we are freeing
 * 
 */
void finish_first_pass(FILE *file, char *final_name){
    if (file){ /* if the file is not NULL */
        fclose(file); /* close the file */
    }
    if (final_name){ /* if the final name is not NULL */
        free(final_name); /* free the final name */
    }
}