#ifndef ENCODING_TYPE_H
#define ENCODING_TYPE_H

typedef enum{
    external_type = 1, /*E*/
    relocatable_type = 2, /*R*/
    absolute_type = 4 /*A*/
}encoding_type;

#endif