#include <stdlib.h>
#include <string.h>
#include "../enums/labels.h"
#include "../enums/boolean.h"
#include "../enums/errors.h"
#include "../enums/guidance.h"
#include "../util/help_methods.h"

#define EQUAL 0
#define NOT_FOUND -1
#define MAX_LENGTH_OF_LABEL_NAME 31
#define LENGTH_OF_NULL_TERMINATOR 1

typedef struct table_of_labels
{
    char name_of_label[MAX_LENGTH_OF_LABEL_NAME + LENGTH_OF_NULL_TERMINATOR]; /* the name of the label +1 for the '\0' */
    label_type type_of_label; /* the type of the label */
    int address; /* the address of the label */
    struct table_of_labels *next; /* the next label in the table */
}table_of_labels;

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
boolean is_exist_label(char label_name[], table_of_labels *table){
    table_of_labels *temp = table;
    while (temp != NULL) { /* while we didn't reach the end of the table */
        if (strcmp(temp->name_of_label, label_name) == EQUAL) { /* if we found the requested label */
            return True; 
        }
        temp = (table_of_labels *)temp->next; /* move to the next label */
    }
    return False; /* if we didn't find the requested label, return False */
}


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
boolean is_there_at_least_one_label_with_the_specfic_type(table_of_labels *table, label_type type_of_the_label){
    table_of_labels *current_label = table;
    while (current_label != NULL){
        if (current_label->type_of_label == type_of_the_label){ /* check if the label is with the type that we want */
            return True; /* return true if we found at least one label with the specific type */
        }
        current_label = current_label->next; /* go to the next label */
    }
    return False; /* return false if we didn't find any label with the specific type */
}


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
label_type get_type_of_label(char label_name[], table_of_labels *table){
    table_of_labels *current_label = table; /* the current label that we are checking */
    while (current_label != NULL){
        if (strcmp(current_label->name_of_label, label_name) == EQUAL){ /* check if the label name is equal to the label name that we want to get the type of */
            return current_label->type_of_label; /* return the type of the label */
        }
        current_label = current_label->next; /* go to the next label */
    }
    return illegal_type; /* just for the protocol (we use this function after we check if the label is defined) */
    
}


/**
 * this function gets the address of the label according to the label name
 * 
 * parameters:
 *    label_name - the name of the label that we want to get the address of
 *    table - the table of labels
 * 
 * returns:
 *    the address of the label if the label is not defined, return -1
 */
int get_adress_of_label(char label_name[], table_of_labels *table){
    table_of_labels *current_label = table; /* the current label that we are checking */
    while (current_label != NULL){
        if (strcmp(current_label->name_of_label, label_name) == EQUAL){ /* check if the label name is equal to the label name that we want to get the type of */
            return current_label->address; /* get the type of the label */
        }
        current_label = current_label->next; /* go to the next label */
    }
    return NOT_FOUND; /* if there is no label with the requested name for the address */
}


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
void add_label_to_table(char label_name[], int address, errors *error, label_type type_of_label, table_of_labels **table, boolean *allocation_faild_flag){
    table_of_labels *new_label = (table_of_labels *)malloc(sizeof(table_of_labels)); /* allocate memory for the new label */
    if (check_allocation(new_label) == THERE_IS_AN_ERROR){ /* check if there is an error in the allocation */
        *error = THERE_IS_AN_ERROR;
        *allocation_faild_flag = True;
        return;
    }

    strcpy(new_label->name_of_label, label_name); /* copy the name of the label */
    new_label->type_of_label = type_of_label; /* set the type of the label */
    new_label->address = address; /* set the address of the label */
    new_label->next = NULL; /* set the next label to NULL */

    if (*table == NULL) { /* if the table is empty */
        *table = new_label; 
    } 
    else { /* if the table is not empty */
        table_of_labels *temp = *table;
        while (temp->next != NULL) { /* while we didn't reach the end of the table */
            temp = temp->next; /* move to the next label */
        }
        temp->next = new_label; /* add the new label to the end of the table */
    }
}


/**
 * this function updates all the adresses of data labels, so the address of the data labels will be the address of the data labels + ic (after we add the start adress - 100)
 * note: we use this function after we finish the first pass, we do it just in the end of the first pass, because we not assume that the data labels are in the end of the file
 * 
 * parameters:
 *    IC - the ic that we want to add to the address of the data labels
 *    table - the table of labels
 */
void update_all_the_adresses_of_data_labels(int IC, table_of_labels *table){
    table_of_labels *current_label = table;
    while (current_label != NULL){
        if (current_label->type_of_label == data){ /* check if the label is with the type that we want to update the address */
            current_label->address += IC; /* update the address of the label */
        }
        current_label = current_label->next; /* go to the next label */
    }
}


/**
 * this function sets the type of the label of label to the requested type
 * 
 * parameters:
 *    label_name - the name of the label that we want to set the type of
 *    type_of_label - the type of the label that we want to set
 *    table - the table of labels
 * 
 */
void set_type_of_label(char label_name[], label_type type_of_label, table_of_labels *table){
    table_of_labels *current_label = table; /* the current label that we are checking */
    while (current_label != NULL){
        if (strcmp(current_label->name_of_label, label_name) == EQUAL){ /* check if the label name is equal to the label name that we want to get the type of */
            current_label->type_of_label = type_of_label; /* set the type of the label */
        }
        current_label = current_label->next; /* go to the next label */
    }
}


/**
 * this function writes all the labels with the specific type and their adress to the file
 * 
 * parameters:
 *    file - the file that we want to write the labels to
 *    table - the table of the labels
 *    type_of_the_label - the type of the label that we want to write to the file
 */
void write_all_the_labels_that_with_the_specific_type(FILE *file, label_type type_of_the_label, table_of_labels *table){
    table_of_labels *current_label = table;
    boolean is_first_label = True; /* the flag that indicates if this is the first label that we write to the file */
    while (current_label != NULL){
        if (current_label->type_of_label == type_of_the_label){ /* check if the label is with the type that we want to write to the file */
            if (is_first_label == True){ /* if this is the first label that we write to the file */
                fprintf(file, "%-35s%04d", current_label->name_of_label, current_label->address); /* print the label name and address in a format where the name is left-aligned and the address is right-aligned(the maximum length of label is 31 ). */
                is_first_label = False; /* set the flag to False */
            }
            else{
            fprintf(file, "\n%-35s%04d", current_label->name_of_label, current_label->address); /* print the label name and address in a format where the name is left-aligned and the address is right-aligned(the maximum length of label is 31 ). */
            }
        }
        current_label = current_label->next; /* go to the next label */
    }
}


/**
 * this function frees the memory that allocated for the table of labels
 * parameters:
 *    table - the table of labels
 * 
 */
void free_table_of_labels(table_of_labels *table){
    table_of_labels *temp; 
    while (table != NULL){ /* while we didn't reach the end of the table */
        temp = table; /* save the current label */
        table = (table_of_labels *)table->next; /* move to the next label */
        free(temp); /* free the current label */
    }
}