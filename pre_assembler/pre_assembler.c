#include <stdio.h>
#include <stdlib.h>
#include "pre_assembler.h"
#include "handle_macros.h"
#include "macros_structure/table_of_macros.h"
#include "../enums/boolean.h"
#include "../enums/errors.h"
#include "../errors_handling/print_messages.h"
#include "../util/help_methods.h"

#define MAX_CHARS_IN_LINE 82 /* the maximum length of the line is 81 and +1 for the null terminator, when we define the array */
#define MAX_LENGTH_OF_MACRO_NAME 31 /* the maximum length of the name of the macro */
#define LENGTH_OF_NULL_TERMINATOR 1 /* the length of the null terminator */
#define FIRST_LINE 1 /* the number of the first line in the file (we start to count from 1) */

/* ---------------PROTOTYPES--------------- */

errors open_files_pre_assembler(FILE **, FILE **, char **, char **, char *, boolean *);
void finish_the_pre_process(FILE *, FILE *, char *, char *);

/* ---------------END OF PROTOTYPES--------------- */



/**
 * this function pre-processes the file that the user entered and lays out the macros that were defined in the file. 
 * 
 * the function gets the name of the file that the user entered and adds the correct extension to the name of the file for opening the file. 
 * after it, the function reads the file line by line, and if the line is a macro call, the function writes the content of the macro to the output file that the function creates.
 * if there is an error in the file, the function will print the error, the line, and the name of the file where the error happened, and we don't continue to the next passes.
 * 
 * note: the check if there it is a name of a label that is like a macro name that was defined, will be in the first pass. (according to the project's instructions)
 * 
 * parameters:
 *      file_name - the name of the file that the user entered (without the extension)
 *      macros_table - the table of macros that we are going to update
 *      allocation_faild_flag - the flag that indicates if there is an allocation error
 * 
 * returns:
 *     THERE_IS_NO_ERROR - if there is no error in the file
 *     THERE_IS_AN_ERROR - if there is an error in the file
 * 
 */
errors pre_proccess(char *file_name,table_of_macros **macros_table, boolean *allocation_faild_flag) {
    FILE *input_file,*output_file; /* the input file that we are going to read from(that the user entered)and the output file that is the input file after laying out the macros */
    char *final_name_of_input_file, *final_name_of_output_file; /* the name of the input file and the name of the output file that we are going to open (with the correct extension) */
    char line[MAX_CHARS_IN_LINE], name_of_macro[MAX_LENGTH_OF_MACRO_NAME + LENGTH_OF_NULL_TERMINATOR]; /* the current line that we are reading from the file and the name of the macro that we are going to define */
    char *content_of_macro; /* the content of the macro that we are going to define */
    errors is_error = THERE_IS_NO_ERROR; /* indicate if there is an error in the file */
    int number_line = FIRST_LINE; /* indicate the number of the current line that we are reading from the file */
    if (open_files_pre_assembler(&input_file, &output_file, &final_name_of_input_file, &final_name_of_output_file, file_name, allocation_faild_flag) == THERE_IS_AN_ERROR) { /* open the input file and the output file and check if there is an error to open the files */
        return THERE_IS_AN_ERROR; /* if there is an error to open the file, return that there is an error */
    }
    while (fgets(line, MAX_CHARS_IN_LINE, input_file)  && *allocation_faild_flag == False) { /* read line by line until the end of the file, or until there is an allocation error */
        if (is_not_relevent_line(line) == True) {number_line++;continue;} /* if the line is a comment or an empty line */
        if (is_line_too_long(line) == True) { /* if the line is too long */
            print_errors(LINE_TOO_LONG, number_line, final_name_of_input_file);  /* print the error */
            is_error = THERE_IS_AN_ERROR; /* indicate that there is an error in the file */
        }
        if (is_exist_macro(line, *macros_table) == True) { /* check if the line is a macro call(with out extra text), and if so we need to lay out the macro */
            get_macro_content(line, &content_of_macro, *macros_table); /* sets the content_of_macro value to be the content of the macro- lay out the macro */
            fprintf(output_file, "%s", content_of_macro); /* write the macro's content to the output file */
            free(content_of_macro); /* free the memory that we allocated for the content of the macro */
        }
        else if(is_macro_definition(line, name_of_macro, final_name_of_input_file, number_line, &is_error, *macros_table) == True) { /*check if the line is start of a macro definition, and if so, we update the name of the macro*/
           handle_macro_defintion(input_file, name_of_macro, final_name_of_input_file, &number_line, &is_error, macros_table, allocation_faild_flag); /* handle the macro definition */
        }
        else {
            fprintf(output_file, "%s", line); /* write the line to the output file */
        }
        number_line++; /* we read the next line, so we need to increment the number of the current line */
    }
    finish_the_pre_process(input_file, output_file, final_name_of_input_file, final_name_of_output_file); /* call to function that free all the memory that we allocated in the pre-process */
    return is_error; /* return if there is an error in the file */
}  


/**
 * this function open the input file and the output file and check if there is an error to open the files
 * 
 * parameters:
 *     input_file - the pointer to the input file that we are going to open
 *     output_file - the pointer to the output file that we are going to open
 *     final_name_of_input_file - the pointer to the name of the input file that we are going to open
 *     final_name_of_output_file - the pointer to the name of the output file that we are going to open
 *     file_name - the name of the file that the user entered
 *     allocation_faild_flag - the flag that indicates if there is an allocation error
 * 
 * returns:
 *    THERE_IS_NO_ERROR - if there is no error to open the files
 *    THERE_IS_AN_ERROR - if there is an error to open the files
 * 
 */
errors open_files_pre_assembler(FILE **input_file, FILE **output_file, char **final_name_of_input_file, char **final_name_of_output_file, char *file_name, boolean *allocation_faild_flag) {
    get_the_full_name_of_file(final_name_of_input_file, file_name, "as"); /* get the full name of the input file with the extension after we add the correct extension to the name of the file */
    *input_file = fopen(*final_name_of_input_file, "r"); /* open the input file just for reading */
    if (check_file(*input_file,*final_name_of_input_file) == THERE_IS_AN_ERROR) { /* check if there is an error to open the file*/
        finish_the_pre_process(*input_file, NULL, *final_name_of_input_file,NULL); /* call to function that free all the memory and close all the files */
        *allocation_faild_flag = True; /* indicate that there is an allocation error */
        return THERE_IS_AN_ERROR; /* return that there is an error */
    } 
    get_the_full_name_of_file(final_name_of_output_file, file_name, "am"); /* get the full name of the output file with the extensiom after we add the correct extension to the name of the file */
    *output_file = fopen(*final_name_of_output_file, "w"); /* open the output file just for writing */
    if (check_file(*output_file,*final_name_of_output_file) == THERE_IS_AN_ERROR) { /* check if there is an error to open the file*/
        finish_the_pre_process(*input_file, *output_file, *final_name_of_input_file, *final_name_of_output_file); /* call to function that free all the memory and close all the files */
        *allocation_faild_flag = True; /* indicate that there is an allocation error */
        return THERE_IS_AN_ERROR; /* return that there is an error */
    }
    return THERE_IS_NO_ERROR; /* return that there is no error */

}


/**
 * this function free all the memory that we allocated in the pre-process
 * 
 * parameters:
 *     input_file - the input file that we are going to free(if it is not NULL)
 *     output_file - the output file that we are going to free(if it is not NULL)
 *     final_name_of_input_file - the name of the input file that we are going to free(if it is not NULL)
 *     final_name_of_output_file - the name of the output file that we are going to free(if it is not NULL)
 * 
 */
void finish_the_pre_process(FILE *input_file, FILE *output_file, char *final_name_of_input_file, char *final_name_of_output_file) {
    if (input_file){
        fclose(input_file); /* close the input file */
    }
    if (output_file){
        fclose(output_file); /* close the output file */
    }
    if (final_name_of_input_file){
        free(final_name_of_input_file); /* free the memory that we allocated for the name of the input file */
    }
    if (final_name_of_output_file){
        free(final_name_of_output_file); /* free the memory that we allocated for the name of the output file */
    }
}
