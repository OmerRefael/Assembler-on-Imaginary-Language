#ifndef ERRORS_H
#define ERRORS_H

typedef enum {
    /* Abstract Errors */
    THERE_IS_AN_ERROR = -1,
    THERE_IS_NO_ERROR,

    /* General Errors */
    ALLOCATION_FAILED,
    CANT_OPEN_FILE,
    LINE_TOO_LONG,
    NO_ARGUMENTS_FOR_THE_PROGRAM,
    EXCEEDING_MEMORY_LIMIT,

    /* Syntax Errors */ 
    MULTIPLE_COMMAS,
    MISSING_COMMAS,
    ILLEGAL_COMMAS,
    COMMA_AT_THE_END,


    /* Macro Errors - Pre Assembler */
    EMPTY_MACRO_NAME,
    MACRO_NAME_IS_TOO_LONG,
    MACRO_NAME_IS_SAVE_WORD,
    MACRO_NAME_CONTAINS_ILLEGAL_CHARACTERS,
    OTHER_MACROS_WITH_THE_SAME_NAME,
    EXTRA_TEXT_TO_MACRO_DEFINITION,
    EXTRA_TEXT_TO_END_MACRO_DEFINITION,
    MACRO_NAME_NOT_START_WITH_LETTER,

    /* Label Errors - First Pass */ 
    EMPTY_LABEL_NAME,
    LABEL_NAME_IS_TOO_LONG,
    LABEL_NAME_IS_SAVE_WORD,
    LABEL_NAME_NOT_START_WITH_LETTER,
    NOT_SPACE_AFTER_LABEL_DEFINITION,
    LABEL_NAME_CONTAINS_ILLEGAL_CHARACTERS,
    OTHER_LABELS_WITH_THE_SAME_NAME,
    LABEL_WITH_A_NAME_OF_MACRO,
    LABEL_WITHOUT_CONTENT,

    /* String Guidance Errors - First Pass */
    MISSING_PARAMETERS_STRING_GUIDANCE,
    MISS_OPEN_QUOTE_IN_STRING_PARAMETER,
    MISS_CLOSE_QUOTE_IN_STRING_PARAMETER,
    EMPTY_STRING_PARAMETER,
    EXTRA_TEXT_BEFORE_STRING_PARAMETER,
    EXTRA_TEXT_AFTER_STRING_PARAMETER,

    /* Data Guidance Errors - First Pass */
    MISSING_PARAMETERS_DATA_GUIDANCE,
    INVALID_TYPE_OF_MEMBER_IN_DATA_GUIDANCE,
    INVALID_VALUE_OF_MEMBER_IN_DATA_GUIDANCE,

    /* Extry and Extern Errors - First Pass */
    MISSING_PARAMETERS_ENTRY_EXTERN_GUIDANCE,
    INCORRECT_AMOUNT_OF_OPERANDS_OF_ENTRY_OR_EXTERN_GUIDANCE,

    /* Instruction Errors - First Pass */
    UNDEFINED_COMMAND,
    INCORRECT_AMOUNT_OF_OPERANDS_OF_COMMAND,
    INVALID_TYPE_OF_IMMEDIATE_ADDRESSING_OPERAND,
    INVALID_VALUE_OF_IMMEDIATE_ADDRESSING_OPERAND,
    INVALID_VALUE_OF_REGISTER_OF_INDIRECT_ADRESSING_OPERAND,
    INVALID_OPERAND,
    ILLEGAL_ADDRESSING_METHOD_FOR_SOURCE_OPERAND,
    ILLEGAL_ADDRESSING_METHOD_FOR_DESTINATION_OPERAND,

    /* Entry/Extern Guidance Errors - Second Pass */
    LABEL_OF_ENTRY_GUIDANCE_THAT_DECLARED_BUT_NOT_DEFINED,
    LABEL_THAT_ENTRY_AND_ALSO_EXTERNAL, 

    /* Label Errors - Second Pass */
    LABEL_THAT_NOT_DEFINED,

    /* File Creation Errors */
    CANT_CREATE_OBJECT_FILE,
    CANT_CREATE_ENTRY_FILE,
    CANT_CREATE_EXTERN_FILE
} errors; 

#endif
