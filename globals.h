#ifndef MAMAN_14_GLOBALS_H
#define MAMAN_14_GLOBALS_H


#define COMMANDS_TABLE_SIZE = 1000
#define MAX_LABEL_SIZE 31
#define MAX_LABEL_COUNT 31
#define MAX_LINE_LENGTH 80
#define IC_START 0
#define DC_START 0
#define CODE_IMG_LENGTH 100


enum e_instructions {
    MOV = 0,
    CMP = 1,
    ADD = 2,
    SUB = 3,
    NOT = 4,
    CLR = 5,
    LEA = 6,
    INC = 7,
    DEC = 8,
    JMP = 9,
    BNE = 10,
    RED = 11,
    PRN = 12,
    JSR = 13,
    RTS = 14,
    STOP = 15,
    INSTRUCTION_NOT_FOUND = -1
};


enum e_are {
    ZERO = 0,
    ZERO_ONE = 1,
    ONE_ZERO = 2
};

enum e_address {
    IMMEDIATE = 1,
    DIRECT = 3,
    REGISTER_DIRECT = 5,
    NOT_EXISTS = 0
};

enum e_registers {
    R0 = 0,
    R1 = 1,
    R2 = 2,
    R3 = 3,
    R4 = 4,
    R5 = 5,
    R6 = 6,
    R7 = 7,
    REGISTER_NOT_FOUND = -1
};

enum e_code_word_type {
    INSTRUCTION_WORD = 0,
    DATA_NUMBER_WORD = 1,
    DATA_LABEL_WORD = 2,
    REGISTER_WORD = 3,
    CODE_WORD_NOT_FOUND = -1
};

typedef struct CodeWord {
    union {
        struct {
            enum e_are are: 2;
            enum e_address source: 3;
            unsigned int opcode: 4;
            enum e_address dest: 3;
        } instruction;


        struct {
            union {
                signed int value: 10;
                char *label;
            };
            enum e_are are: 2;
        } data;

        struct {
            enum e_registers source: 5;
            enum e_registers dest: 5;
            enum e_are are: 2;
        } registerWord;
    };

    enum e_code_word_type codeWordType;
    struct CodeWord *next;
} CodeWord;

enum e_directive {
    DATA = 0,
    STRING = 1,
    ENTRY = 2,
    EXTERN = 3,
    DIRECTIVE_NOT_FOUND = -1
};

enum e_label_type {
    DATA_LABEL = 0,
    CODE_LABEL = 1,
    ENTRY_LABEL = 2,
    EXTERN_LABEL = 3,
    UNDEFINED_LABEL = -1,
};

enum e_operand_type {
    NUMBER_O = 0,
    LABEL_O = 1,
    REGISTER_O = 2,
    OPERAND_NOT_FOUND = -1
};

typedef struct Label {
    char *name;
    long address;
    struct Label *next;
    enum e_label_type type;
} Label;

typedef struct {
    union {
        int number;
        char string[2];
    };
    enum e_directive datatype;
} DataWord;

typedef struct {
    char *name;
    enum e_instructions opcode;
    enum e_address *allowedSourceAddressingMethods;
    enum e_address *allowedDestAddressingMethods;
    int allowedSourceAddressingMethodsCount;
    int allowedDestAddressingMethodsCount;
    int numberOfOperands;
} Instruction;


typedef struct {
    char *name;
    enum e_registers registerNumber;
} Register;


typedef struct {
    char *name;
    enum e_directive directive;
} Directive;


typedef struct {
    union {
        int number;
        char label[MAX_LABEL_SIZE];
        Register register_operand;
    };
    enum e_operand_type operandType;
} Operand;

typedef struct FuncResult {
    int result;
    char *message;
} FuncResult;


#endif

