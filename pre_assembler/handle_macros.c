#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "handle_macros.h"
#include "macros_structure/table_of_macros.h"
#include "../enums/boolean.h"
#include "../enums/errors.h"
#include "../errors_handling/print_messages.h"
#include "../util/help_methods.h"

#define EQUAL 0 /* for indicating that two values are equal */
#define MAX_CHARS_IN_LINE 82 /* the maximum length of the line is 81 and +1 for the null terminator, when we define the array */
#define MAX_LENGTH_OF_MACRO_NAME 31 /* the maximum length of the macro name */
#define START_VALUE_OF_WORD_COUNTER 0 /* the start value of the word counter */
#define JUST_ONE_WORD 1  /* the value of the counter of the words in the line for checking if there is an extra text in the end of the macro definition */


/* ---------------PROTOTYPES--------------- */

boolean is_valid_of_macro_name(char [], char *, int , table_of_macros *);

/* ---------------END OF PROTOTYPES--------------- */

/** 
 * this function checks if the macro is defined in the table of macros for laying out the macros
 * 
 * parameters:
 *     line - the line that contains the macro
 *     table - the table of macros  
 * 
 * returns: 
 *    True - if the macro is defined (and the line contains only the macro name, so we will lay out the macro)
 *    False - if the macro is not defined (or the line contains more than the macro name, so we will no lay out the macro)
 * 
 */
boolean is_exist_macro(char *line, table_of_macros *table) {
    char copy_line[MAX_CHARS_IN_LINE];
    char *first_word;
    boolean is_exist_macro = False;
    strcpy(copy_line, line); /* copy the line to a new string */
    first_word = strtok(copy_line, " \t\n\r\f\v");
    if(strtok(NULL, " \t\n\r\f\v") == NULL) { /* if the line contains only the macro name , and no other words */
        is_exist_macro = is_defined_macro(table, first_word); /* check if it is an existing macro */
    }
    return is_exist_macro; /* return the result */
}


/** 
 * this function checks if the line is a start of macro definition, and also if so, if there it is a extra text in the end of the macro definition
 * 
 * parameters:
 *    line - the line that contains the we want to check
 *    name_of_macro - the name of the macro
 *    file_name - the name of the file
 *    number_line - the number of the number_line
 *    error - the flag that indicates if there is an error
 *    table - the table of macros
 * 
 * returns: 
 *    True - if the line is a start of macro definition
 *    False - if the line is not is a start of macro definition
 * 
 */
boolean  is_macro_definition(char line[], char *name_of_macro, char *file_name, int number_line, errors *error, table_of_macros *table) {
    char copy_line[MAX_CHARS_IN_LINE];
    char *first_word, *token;
    strcpy(copy_line, line);
    first_word = strtok(copy_line, " \t\n\r\f\v");
    if (strcmp(first_word, "macr") == EQUAL) { /* check if the first word is "macr", that means that it is a macro definition */
        token = strtok(NULL, " \t\n\r\f\v"); /* get the name of the macro */
        if (strtok(NULL, " \t\n\r\f\v") != NULL) { /* check if there is are other words in the line */
            print_errors(EXTRA_TEXT_TO_MACRO_DEFINITION, number_line, file_name); /* print an error message */
            *error = THERE_IS_AN_ERROR; /* set the error flag to True */
        }
        if (is_valid_of_macro_name(token, file_name, number_line, table) == False) { /* check if the name of the macro is valid */
            *error = THERE_IS_AN_ERROR; /* set the error flag to True */
            name_of_macro = NULL; /* set the name of the macro to NULL */
        }
        else{
            strcpy(name_of_macro,token); /* copy the name of the macro to the parameter */
        }
        return True; /* return True */
    }  
    return False;  /* return False */
}


/** 
 * this function checks if the line is the end of macro definition and if so, it is also check if there is an extra text in the end of the macro definition
 * 
 * parameters:
 *     line - the line that contains the we want to check
 *     file_name - the name of the file
 *     number_line - the number of the number_line
 *     error - the flag that indicates if there is an error
 * 
 * returns: 
 *    True - if the line is the end of macro definition
 *    False - if the line is not the end of macro definition
 * 
 */
boolean is_end_of_macro_definition(char *line, char *file_name, int number_line, errors *error) {
    char copy_line[MAX_CHARS_IN_LINE];
    char *word;
    boolean found_endmacr_flag = False; /* flag that indicates if we found the end of the macro definition */
    int counter_of_words = START_VALUE_OF_WORD_COUNTER; /* counter of the words in the line for checking if there is an extra text in the end of the macro definition */
    strcpy(copy_line, line);
    word = strtok(copy_line, " \t\n\r\f\v");
    while (word != NULL) {
        if (strcmp(word, "endmacr") == EQUAL) { /* check if the word is "endmacr" */
            found_endmacr_flag = True; /* set the flag to True */
        }
        word = strtok(NULL, " \t\n\r\f\v"); /* get the next word */
        counter_of_words++; /* increase the counter of the words */
    }
    if (counter_of_words > JUST_ONE_WORD && found_endmacr_flag == True) { /* check if there is an extra text in the end of the macro definition */
        print_errors(EXTRA_TEXT_TO_END_MACRO_DEFINITION, number_line, file_name); /* print an error message */
        *error = THERE_IS_AN_ERROR; /* set the error flag to True */
    }
    return found_endmacr_flag; /* return the result */
}


/** 
 * this function checks if the name of the macro is valid according to the rules of the project:
 * 1. the name of the macro must not be empty.
 * 2. the name of the macro must not be too long.
 * 3. the name of the macro must not be a saved word.
 * 4. the name of the macro must contain only letters and digits and the character '_'.
 * 5. the name of the macro must not be a macro that already exists in the table of macros.
 * 
 * parameters:
 *     name_of_macro - the name of the macro
 *     file_name - the name of the file
 *     number_line - the number of the number_line
 *     table - the table of macros
 * 
 * returns:
 *     True - if the name of the macro is valid
 *     False - if the name of the macro is not valid
 */
boolean is_valid_of_macro_name(char name_of_macro[], char *file_name, int number_line, table_of_macros *table) {
    errors error = THERE_IS_NO_ERROR; /* set the error to "THERE_IS_NO_ERROR" */
    if (name_of_macro == NULL) { /* check if the name of the macro is empty */
        error = EMPTY_MACRO_NAME; /* set the error to "EMPTY_MACRO_NAME" */
    }
    else if (strlen(name_of_macro) > MAX_LENGTH_OF_MACRO_NAME) { /* check if the length of the macro name is bigger than the maximum length */
        error = MACRO_NAME_IS_TOO_LONG; /* set the error to "MACRO_NAME_IS_TOO_LONG" */
    }
    else if (is_a_save_word(name_of_macro) ==  True) { /* check if the macro name is a saved word */
        error = MACRO_NAME_IS_SAVE_WORD; /* set the error to "MACRO_NAME_IS_SAVE_WORD" */
    }
    else if (check_if_first_char_is_letter(name_of_macro) == False) { /* check if the first character of the macro name is a letter */
        error = MACRO_NAME_NOT_START_WITH_LETTER; /* set the error to "MACRO_NAME_MUST_START_WITH_LETTER" */
    }
    else if (check_if_word_contains_illegal_chars(name_of_macro, True) == True) { /* check if the macro name has forbidden characters */
        error = MACRO_NAME_CONTAINS_ILLEGAL_CHARACTERS; /* set the error to "MACRO_NAME_CONTAINS_ILLEGAL_CHARACTERS" */
    }
    else if  (is_defined_macro(table, name_of_macro) == True) { /* check if the macro is already defined */
        error = OTHER_MACROS_WITH_THE_SAME_NAME; /* set the error to "OTHER_MACROS_WITH_THE_SAME_NAME" */
    }

    if (error != THERE_IS_NO_ERROR) { /* check if there is an error */
        print_errors(error, number_line, file_name); /* print an error message */
        return False; /* return False */
    }
    return True; /* return True */
}


/** 
 * this function gets the content of the macro from the table of macros 
 * 
 * parameters:
 *     line - the line that contains the macro
 *     content_of_macro - the content of the macro
 *     table - the table of macros
 * 
 */
void get_macro_content(char *line, char **content_of_macro, table_of_macros *table) {
    char *name_of_macro = strtok(line, " \t\n\r\f\v"); /* get the name of the macro */
    get_the_content_of_the_macro(table, name_of_macro, content_of_macro); /* get the content of the macro */
} 


/** 
 * this function handles the macro definition
 * 
 * parameters:
 *     input_file - the file that we are reading from
 *     name_of_macro - the name of the macro
 *     file_name - the name of the file
 *     number_line - the number of the number_line
 *     error - the flag that indicates if there is an error
 *     macros_table - the table of macros
 *     allocation_faild_flag - the flag that indicates if there is an allocation error
 * 
 */
void handle_macro_defintion(FILE *input_file,char name_of_macro[], char *file_name, int *number_line, errors *error ,table_of_macros **macros_table, boolean *allocation_faild_flag) {
    char line[MAX_CHARS_IN_LINE];
    char *macro_content;
    macro_content = (char*)calloc(MAX_CHARS_IN_LINE + 1, sizeof(char)); /* allocate memory for the macro content */
    if (check_allocation(macro_content) == THERE_IS_AN_ERROR) {
        *error = THERE_IS_AN_ERROR;
        *allocation_faild_flag = True;
        return;
    }
    
    while (fgets(line, MAX_CHARS_IN_LINE, input_file) != NULL) { /* read the next line, and check if we reached the end of the file */
        (*number_line)++;
        if (is_end_of_macro_definition(line, file_name, *number_line, error) == True) { /* check if the line marks the end of the macro definition */
            break;
        }
        if (is_not_relevent_line(line) == True) {continue;} /* if the line is a comment or an empty line */
        if (is_line_too_long(line) == True) { /* if the line is too long */
            print_errors(LINE_TOO_LONG, *number_line, file_name); /* print the error */
            *error = THERE_IS_AN_ERROR; /* indicate that there is an error in the file */
        }

        macro_content = strcat(macro_content, line); /* add the line to the macro content */
        macro_content = (char *)realloc(macro_content, strlen(macro_content) + MAX_CHARS_IN_LINE); /* reallocate memory for the macro content */
        if (check_allocation(macro_content) == THERE_IS_AN_ERROR) { /* check if there is an error to allocate memory */
            *error = THERE_IS_AN_ERROR;
            *allocation_faild_flag = True;
            return;
        }

    }
    if (*error == THERE_IS_NO_ERROR){
    add_macro_to_table(macros_table, name_of_macro, macro_content, error, allocation_faild_flag);} /* add the macro to the table of macros */
    free(macro_content); /* free the memory that we allocated for the macro content */
}