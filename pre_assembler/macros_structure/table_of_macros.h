#ifndef TABLE_OF_MACROS_H
#define TABLE_OF_MACROS_H

#include "../../enums/boolean.h"
#include "../../enums/errors.h"

typedef struct table_of_macros table_of_macros;

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
boolean is_defined_macro(table_of_macros *, char *);
/**
 * this function sets the content of the macro from the table of macros to the paramter (conntent_of_the_macro)
 * 
 * parameters:
 *     table - the table of macros
 *     name_of_macro - the name of the macro thhat we want to get the content of
 *     content_of_the_macro - the content of the macro
 * 
 */
void get_the_content_of_the_macro(table_of_macros *, char *, char **);
/**
 * this function adds a macro to the table of macros
 * 
 * parameters:
 *     table - the table of macros
 *     name_of_macro - the name of the macro
 *     content_of_macro - the content of the macro
 *     error - the flag that indicates if there is an error
 *     allocation_faild_flag - the flag that indicates if there is an allocation error  
 * 
 */
void add_macro_to_table(table_of_macros **, char *, char *, errors *, boolean *);
/**
 * this function free all the memory that we allocated in the table of macros
 * 
 * parameters:
 *     table - the table of macros
 * 
 */
void free_table_of_macros(table_of_macros *);

#endif