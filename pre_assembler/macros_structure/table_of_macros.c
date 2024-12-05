#include <stdlib.h>
#include <string.h>
#include "../../enums/boolean.h"
#include "../../enums/errors.h"
#include "../../util/help_methods.h"

#define MAX_LENGTH_OF_MACRO_NAME 31 /* the maximum length of the macro name */
#define LENGTH_OF_NULL_TERMINATOR 1 /* the length of the null terminator */
#define EQUAL 0 /* for indicating that two values are equal */

typedef struct table_of_macros
{
    char name_of_macro[MAX_LENGTH_OF_MACRO_NAME + LENGTH_OF_NULL_TERMINATOR]; /* the name of the macro +1 for the '\0' */
    char *content_of_macro; /* the content of the macro */
    struct table_of_macros *next; /* the next macro in the table */

}table_of_macros;

/**
 * this function checks if the macro is defined in the table of macros
 * 
 * parameters:
 *     table - the table of macros
 *     name - the name of the macro
 * 
 * returns:
 *     True - if the macro is defined
 *     False - if the macro is not defined
 */
boolean is_defined_macro(table_of_macros *table, char *name) {
    table_of_macros *temp = table;
    while (temp != NULL) { /* while we didn't reach the end of the table */
        if (strcmp(temp->name_of_macro, name) == EQUAL) { /* if we found the requested macro */
            return True; /* if we found the requested macro */
        }
        temp = (table_of_macros *)temp->next; /* move to the next macro */
    }
    return False; /* if we didn't find the requested macro */
}


/**
 * this function sets the content of the macro from the table of macros to the paramter (conntent_of_the_macro)
 * 
 * parameters:
 *     table - the table of macros
 *     name_of_macro - the name of the macro thhat we want to get the content of
 *     content_of_the_macro - the content of the macro
 * 
 */
void get_the_content_of_the_macro(table_of_macros *table, char *name_of_the_macro, char **content_of_the_macro) {
    table_of_macros *temp = table;
    while (temp != NULL) { /* while we didn't reach the end of the table */
        if (strcmp(temp->name_of_macro, name_of_the_macro) == EQUAL) { /* if we found the requested macro */
            *content_of_the_macro = (char *)calloc(strlen(temp->content_of_macro) + 1, sizeof(char)); /* allocate memory for the content of the macro */
            strcpy(*content_of_the_macro, temp->content_of_macro); /* copy the content of the macro */
        }
        temp = (table_of_macros *)temp->next;
    }
}


/**
 * this function adds a macro to the table of macros
 * 
 * parameters:
 *    table - the table of macros
 *    name_of_macro - the name of the macro
 *    content_of_macro - the content of the macro
 *    error - the flag that indicates if there is an error
 *    allocation_faild_flag - the flag that indicates if there is an allocation error 
 * 
 */
void add_macro_to_table(table_of_macros **table, char name_of_macro[], char *content_of_macro, errors *error, boolean *allocation_faild_flag) {
    table_of_macros *new_macro = (table_of_macros *)malloc(sizeof(table_of_macros)); /* allocate memory for the new macro */
    new_macro->content_of_macro = (char *)calloc(strlen(content_of_macro) + 1, sizeof(char));
    if (check_allocation(new_macro) == THERE_IS_AN_ERROR || check_allocation(new_macro->content_of_macro) == THERE_IS_AN_ERROR){ /* if there is an error to allocate memory */
        *error = THERE_IS_AN_ERROR;
        *allocation_faild_flag = True;
        return;
    }

    strcpy(new_macro->name_of_macro, name_of_macro); /* copy the name of the macro */
    strcpy(new_macro->content_of_macro, content_of_macro); /* copy the content of the macro */
    new_macro->next = NULL; /* set the next of the new macro to NULL */

    if (*table == NULL) { /* if the table is empty, what means that this is the first macro */
        *table = new_macro; 
    } 
    else {
        table_of_macros *temp = *table;
        while (temp->next != NULL) { /* find the last macro in the table */
            temp = (table_of_macros *)temp->next;
        }
        temp->next = new_macro; /* add the new macro to the end of the table */
    }
}


/**
 * this function free all the memory that we allocated in the table of macros
 * 
 * parameters:
 *     table - the table of macros
 * 
 */
void free_table_of_macros(table_of_macros *table) {
    table_of_macros *temp; /* we will use this pointer to free the memory of the macro table */
    while (table != NULL) { /* while we didn't reach the end of the table */
        temp = table;
        table = (table_of_macros *)table->next; /* we move to the next macro */
        free(temp->content_of_macro);
        free(temp); 
    }
}