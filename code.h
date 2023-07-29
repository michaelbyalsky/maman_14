#ifndef MAMAN_14_CODE_H
#define MAMAN_14_CODE_H

#include "globals.h"

enum Directives {
    DATA = 0,
    STRING = 1,
    ENTRY = 2,
    EXTERN = 3,
    DIRECTIVE_NOT_FOUND = -1
};


typedef struct {
    char *name;
    enum Directives directive;
} Directive;


/**
 * @brief find the directive by name, gets a string that in the beginning of it there is a directive name
 * @param line
 * @return int
 */
int find_directive_by_name(char *line);

/**
 * @brief Enum for registers
 */
enum Register {
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


/**
 * @brief Struct for registers
 */
typedef struct {
    char *name;
    enum Register registerNumber;
} Register;

/**
 * @brief function to find register by name
 * @param name
 * @return Register
 */
Register findRegisterByName(const char *name);

enum Instructions {
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

typedef struct Instruction {
    char *name;
    enum Instructions opcode;
    int *allowedSourceAddressingMethods;
    int *allowedDestAddressingMethods;
    int allowedSourceAddressingMethodsCount;
    int allowedDestAddressingMethodsCount;
    int numberOfOperands;
} Instruction;

/**
 * @brief function to determine if a line is an instruction
 * @param line
 * @return
 */
int is_instruction(char *line);

/**
 * @brief function to find instruction by name
 * @param name
 * @return
 */
Instruction findInstructionByName(const char *name);


enum Are {
    ZERO = 0,
    ZERO_ONE = 1,
    ONE_ZERO = 2
};

enum AddressMethod {
    IMMEDIATE = 1,
    DIRECT = 3,
    REGISTER_DIRECT = 5,
    NOT_EXISTS = 0
};

enum codeWordType {
    INSTRUCTION_WORD = 0,
    DATA_NUMBER_WORD = 1,
    DATA_LABEL_WORD = 2,
    REGISTER_WORD = 3,
    CODE_WORD_NOT_FOUND = -1
};

enum LabelType {
    DATA_LABEL = 0,
    CODE_LABEL = 1,
    ENTRY_LABEL = 2,
    EXTERN_LABEL = 3,
    UNDEFINED_LABEL = -1
};

enum OperandType {
    NUMBER_O = 0,
    LABEL_O = 1,
    REGISTER_O = 2,
    OPERAND_NOT_FOUND = -1
};

typedef struct CodeWord {
    union CodeWordUnion {
        struct instruction {
            enum AddressMethod source;
            unsigned int opcode;
            enum AddressMethod dest;
        } instruction;


        struct {
            union dataUnion {
                signed int value;
                char *label;
            } dataUnion;
        } data;

        struct registerWord {
            enum Register source;
            enum Register dest;
        } registerWord;
    } CodeWordUnion;

    enum Are are;
    enum codeWordType codeWordType;
    struct CodeWord *next;
} CodeWord;


typedef struct Label {
    char *name;
    long address;
    struct Label *next;
    enum LabelType type;
} Label;

typedef struct DataWord {
    union NumberStringUnion {
        int number;
        char string[2];
    } NumberStringUnion;
    enum Directives datatype;
} DataWord;


typedef struct {
    union NameLabelUnion {
        int number;
        char label[MAX_LABEL_SIZE];
        Register register_operand;
    } NameLabelUnion;
    enum OperandType operandType;
} Operand;

/**
 * @brief give the operand its value and return the addressing method
 * @param string
 * @param instruction
 * @param operand
 * @param isSourceOperand
 * @return int
 */
int get_operand_from_string(char *string, Instruction instruction, Operand *operand, int isSourceOperand);


#endif
