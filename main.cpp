#include <iostream>

const uint8_t REGISTER = 0b001;
const uint8_t STACK = 0b010;
const uint8_t REGISTER_AS_ADDRESS = 0b011;
const uint8_t CONST_INT = 0b100;
const uint8_t CONST_FLOAT = 0b101;

const uint8_t ADD = 1;
const uint8_t SUBTRACT = 2;
const uint8_t MULTIPLY = 3;
const uint8_t DIVIDE = 4;
const uint8_t INCREMENT = 5;
const uint8_t DECREMENT = 6;
const uint8_t MOD = 7;
const uint8_t INVERT = 8;

const uint8_t BIN_AND = 10;
const uint8_t BIN_OR = 11;
const uint8_t BIN_XOR = 12;
const uint8_t BIN_LEFT_SHIFT = 13;
const uint8_t BIN_RIGHT_SHIFT = 14;


const uint8_t PUSH = 19;
const uint8_t INC_STACK = 20;
const uint8_t POP = 21;
const uint8_t DEC_STACK = 22;

const uint8_t JUMP_GREATER_EQUALS = 30;
const uint8_t JUMP_GREATER = 31;
const uint8_t JUMP_EQUALS = 32;
const uint8_t JUMP_NON_EQUALS = 33;
const uint8_t JUMP_SMALLER_EQUALS = 34;
const uint8_t JUMP_SMALLER = 35;
const uint8_t JUMP_IF_TRUE = 36;
const uint8_t JUMP_IF_FALSE = 37;
const uint8_t JUMP = 38;

const uint8_t MOVE = 42;
const uint8_t CALL = 43;
const uint8_t RETURN = 44;
const uint8_t PRINT = 50;


#define ARGUMENT_SIZE 3
#define INSTRUCTION_SIZE 6
#define ARG1 6
#define ARG2 9
#define ARG3 12

#define INSTRUCTION(code) ((code) & 0b111111)
#define TYPE1(code) (((code) & 0b111000000) >> ARG1)
#define TYPE2(code) (((code) & 0b111000000000) >> ARG2)
#define TYPE3(code) (((code) & 0b111000000000000) >> ARG3)

#define VALUE(arg, value_pointer, var) switch (arg) { \
case STACK: {\
var = *((uint64_t *)(stack + frame_pointer + (*(value_pointer))));\
break;\
}\
case REGISTER_AS_ADDRESS: {\
var = heap[registers[(*(value_pointer))]];\
break;\
}\
case REGISTER: {\
var = registers[(*(value_pointer))];\
break;\
}\
case CONST_FLOAT:\
case CONST_INT: {\
var = *(value_pointer);\
break;\
}\
default:\
std::cout << "Unknown Type: " << (int)(arg) << std::endl;\
var = 0;\
}                                                     \

#define TARGET(arg , value_pointer,value) switch (arg) {\
case STACK: {\
*((uint64_t *) (stack + frame_pointer + (*(value_pointer)))) = (value);\
}\
case REGISTER_AS_ADDRESS: {\
*(heap + (*(registers + (*(value_pointer))))) = (value);\
break;\
}\
case REGISTER: {\
*(registers + (*(value_pointer))) = (value);\
break;\
}\
case CONST_FLOAT:\
case CONST_INT:\
std::cout << "Cant set a value to a Constant type: " << (int) (arg) << std::endl;\
break;\
default:\
std::cout << "Unknown Type: " << (int) (arg) << std::endl;\
}\

#define INC1ARG instruction_pointer += 10;
#define INC2ARG instruction_pointer += 18;
#define INC3ARG instruction_pointer += 26;

uint8_t *code_segment;
uint64_t *registers;
uint8_t *stack;
uint8_t *heap;

const uint64_t code_segment_size_in_bytes = 512;
const uint64_t stack_size_in_bytes = 512;
const uint64_t heap_size_in_bytes = 512;
const uint64_t max_registers = 16;

uint64_t stack_pointer = 0;
uint64_t frame_pointer = 0;
uint64_t instruction_pointer = 0;

uint64_t max_long = 0xffffffffffffffff;

uint64_t code_builder = 0;

void addInstruction(uint16_t instruction) {
    uint16_t *s = (uint16_t *) (code_segment + code_builder);
    *s = instruction;
    code_builder += 2;
}

void valueFirst(uint64_t value) {
    uint64_t *l = (uint64_t *) (code_segment + code_builder);
    *l = value;
    code_builder += 8;
}

void start() {
    addInstruction(ADD | (REGISTER << ARG1) | (CONST_INT << ARG2) | (CONST_INT << ARG3));
    valueFirst(1);
    valueFirst(17);
    valueFirst(3);

    addInstruction(PRINT | (REGISTER << ARG1));
    valueFirst(1);
    //
    //printInstruction( PUSH | (CONST_INT << ARG1));
    // ValueFirst(1337);

}




void runInstruction() {
    const uint16_t code = *((uint16_t *) (code_segment + instruction_pointer));
    const uint8_t instruction = INSTRUCTION(code);
    const uint8_t arg0 = TYPE1(code);
    const uint8_t arg1 = TYPE2(code);
    const uint8_t arg2 = TYPE3(code);
    std::cout << "Instruction: " << code << std::endl;
    switch (instruction) {
        case PUSH: {
            uint64_t *var = (uint64_t *) (stack + stack_pointer);
            uint64_t i;
            VALUE(arg0, ((uint64_t *) (code_segment + instruction_pointer + 2)), i);
            *var = i;
            stack_pointer += 8;
            INC1ARG
            break;
        }
        case INC_STACK: {
            uint64_t i;
            VALUE(arg0, ((uint64_t *) (code_segment + instruction_pointer + 2)), i);
            stack_pointer += i;
            INC1ARG
            break;
        }
        case DEC_STACK: {
            std::cout << "Not jet implemented" << std::endl;
            break;
        }
        case POP: {
            TARGET(arg0,(uint64_t *) (code_segment + instruction_pointer + 2) ,*(uint64_t *) (stack + stack_pointer) );
            stack_pointer--;
            INC1ARG
            break;
        }
        case MOVE:
            std::cout << "Move" << std::endl;
            break;
        case ADD: {
            uint64_t l;
            uint64_t r;
            VALUE(arg1, ((uint64_t *) (code_segment + instruction_pointer + 10)), l);
            VALUE(arg2, ((uint64_t *) (code_segment + instruction_pointer + 18)), r);
            TARGET(arg0,(uint64_t *) (code_segment + instruction_pointer + 2)  , l + r);
            INC3ARG
            break;
        }
        case PRINT: {
            uint64_t i;
            VALUE(arg0, ((uint64_t *) (code_segment + instruction_pointer + 2)), i);
            std::cout << "[PRINT] " << i << std::endl;
            INC1ARG
            break;
        }
        default:
            std::cout << "Unknown Instruction: " << (int) instruction << std::endl;
    }
}


int main() {
    code_segment = (uint8_t *) malloc(code_segment_size_in_bytes);
    stack = (uint8_t *) malloc(stack_size_in_bytes);
    heap = (uint8_t *) malloc(heap_size_in_bytes);
    registers = (uint64_t *) malloc(max_registers);
    try {
        start();
        runInstruction();
        runInstruction();
    } catch (...) {

    }
    free(code_segment);
    return 0;


}



/*
REGISTER
        STACK
REGISTER_AS_ADDRESS
        CONST_INT
CONST_FLOAT*/
