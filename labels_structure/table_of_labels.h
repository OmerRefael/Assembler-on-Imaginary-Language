#ifndef SYMBOLS_TABLE_H
#define SYMBOLS_TABLE_H

#include <stdio.h>
#include "../enums/errors.h"
#include "../enums/boolean.h"
#include "../enums/labels.h"
#include "../enums/guidance.h"

typedef struct table_of_labels table_of_labels;


/**
 * this function checks if the label is already defined in the table of labels
 * 
 * parameters:
 *    label_name - the name of the label that we want to check if it is defined
 *    table - the table of labels
 * 
 * returns:
 *    True - if the label is defined
 *    False - if the label is not defined
 */
boolean is_exist_label(char [], table_of_labels *);
/**
 * this function checks if there is at least one label with the specific type
 * 
 * parameters:
 *    table - the table of labels
 *    type_of_the_label - the type of the label that we want to check if there is at least one label with this type
 * 
 * returns:
 *    True - if there is at least one label with the specific type
 *    False - if there is no label with the specific type
 */
boolean is_there_at_least_one_label_with_the_specfic_type(table_of_labels *, label_type );
/**
 * this function returns the type of the label according to the label name
 * 
 * parameters:
 *    label_name - the name of the label that we want to get the type of
 *    table - the table of labels
 * 
 * returns:
 *    the type of the label
 */
label_type get_type_of_label(char [], table_of_labels *);
/**
 * this function gets the address of the label according to the label name
 * 
 * parameters:
 *    label_name - the name of the label that we want to get the address of
 *    table - the table of labels
 * 
 * returns:
 *    the address of the label, if the label is not defined, return -1
 */
int get_adress_of_label(char [], table_of_labels *);
/**
 * this function add a label to the table of labels
 * 
 * parameters:
 *    label_name - the name of the label that we want to add to the table
 *    address - the address of the label that we want to add to the table
 *    error - the pointer to the error flag
 *    type_of_label - the type of the label that we want to add to the table
 *    table - the table of labels
 *    allocation_faild_flag - the flag that indicates if there is an allocation error 
 */
void add_label_to_table(char [], int , errors *, label_type , table_of_labels **, boolean *);
/**
 * this function updates all the adresses of data labels, so the address of the data labels will be the address of the data labels + ic (after we add the start adress - 100)
 * note: we use this function after we finish the first pass, we do it just in the end of the first pass, because we not assume that the data labels are in the end of the file
 * 
 * parameters:
 *    IC - the ic that we want to add to the address of the data labels
 *    table - the table of labels
 */
void update_all_the_adresses_of_data_labels(int, table_of_labels * );
/**
 * this function sets the type of the label of label to the requested type
 * 
 * parameters:
 *    label_name - the name of the label that we want to set the type of
 *    type_of_label - the type of the label that we want to set
 *    table - the table of labels
 * 
 */
void set_type_of_label(char [], label_type , table_of_labels *);
/**
 * this function writes all the labels with the specific type and their adress to the file
 * 
 * parameters:
 *    file - the file that we want to write the labels to
 *    table - the table of the labels
 *    type_of_the_label - the type of the label that we want to write to the file
 */
void write_all_the_labels_that_with_the_specific_type(FILE *, label_type , table_of_labels *);
/**
 * this function frees the memory that allocated for the table of labels
 * parameters:
 *     table - the table of labels
 * 
 */
void free_table_of_labels(table_of_labels *);

#endif