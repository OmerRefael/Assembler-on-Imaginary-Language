#ifndef HANDLE_MACROS
#define HANDLE_MACROS

#include <stdio.h>
#include "macros_structure/table_of_macros.h"
#include "../enums/boolean.h"
#include "../enums/errors.h"

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
boolean is_exist_macro(char *, table_of_macros *);
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
boolean  is_macro_definition(char *, char *,  char *, int , errors *, table_of_macros *);
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
boolean is_end_of_macro_definition(char *, char *, int , errors *);
/** 
 * this function gets the content of the macro from the table of macros 
 * 
 * parameters:
 *     line - the line that contains the macro
 *     content_of_macro - the content of the macro
 *     table - the table of macros
 * 
 * returns:
 *     the content of the macro
 */
void get_macro_content(char *, char **, table_of_macros *);
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
void handle_macro_defintion(FILE *,char [], char *, int *, errors * ,table_of_macros **, boolean *);

#endif