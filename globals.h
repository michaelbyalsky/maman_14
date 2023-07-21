#ifndef MAMAN_14_GLOBALS_H
#define MAMAN_14_GLOBALS_H


#define COMMANDS_TABLE_SIZE = 1000
#define MAX_LABEL_SIZE 31
#define MAX_LINE_LENGTH 80
#define IC_START 100
#define DC_START 0
#define CODE_IMG_LENGTH 1000


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
    STOP = 15
};


enum e_are {
    ABSOLUTE = 0,
    EXTERNAL = 1,
    RELOCATABLE = 2
};

enum e_address {
    IMMEDIATE = 1,
    DIRECT = 3,
    REGISTER_DIRECT = 5,
    NOT_EXISTS = -1
};

enum e_registers {
    R0 = 0,
    R1 = 1,
    R2 = 2,
    R3 = 3,
    R4 = 4,
    R5 = 5,
    R6 = 6,
    R7 = 7
};

typedef struct {
    unsigned int ARE : 2;
    unsigned int dest : 3;
    unsigned int opcode : 4;
    unsigned int source : 3;
} CodeWord;

typedef struct {
    char *name;
    int address;
} Label;

typedef struct {
    unsigned int ARE : 2;
    unsigned long data : 10;
} DataWord;

typedef struct {
    char *name;
    enum e_instructions opcode;
    enum e_address allowedSourceAddressingMethods[4];
    enum e_address allowedDestAddressingMethods[4];
} Instruction;



typedef struct {
    char *name;
    enum e_registers registerNumber;
} Register;



enum e_directive {
    DATA = 0,
    STRING = 1,
    ENTRY= 2,
    EXTERN = 3,
    DIRECTIVE_NOT_FOUND = -1
};

typedef struct {
    char *name;
    enum e_directive directive;
} Directive;





#endif

