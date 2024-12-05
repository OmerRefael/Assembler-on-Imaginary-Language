#ifndef WARNINGS_H
#define WARNINGS_H

/* note:In general, in this project we can only notify one warning, but in order to have a neat and beautiful form of code, 
and even if I might want to develop the project, I also made an enum of warnings*/
typedef enum {
    THERE_IS_AN_WARNING = -1, THERE_IS_NO_WARNING, ENTRY_GUIDANCE_WITH_LABEL_DEFINITION, EXTERN_GUIDANCE_WITH_LABEL_DEFINITION
} warnings;

#endif