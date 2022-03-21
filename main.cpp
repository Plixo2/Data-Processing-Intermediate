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

#define INSTRUCTION(code) (code & 0b111111)
#define TYPE1(code) ((code & 0b111000000) >> ARG1)
#define TYPE2(code) ((code & 0b111000000000) >> ARG2)
#define TYPE3(code) ((code & 0b111000000000000) >> ARG3)


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
    code_builder+=2;
}
void valueFirst(uint64_t value) {
    uint64_t *l = (uint64_t *) (code_segment + code_builder);
    *l = value;
    code_builder+=8;
}

void start() {
    addInstruction(PUSH | (CONST_INT << ARG1));
    valueFirst(233);

    addInstruction(PRINT | (STACK << ARG1));
    valueFirst(0);
   //
    //printInstruction( PUSH | (CONST_INT << ARG1));
   // ValueFirst(1337);

}


uint64_t getValue(uint8_t arg, uint64_t *value_pointer) {
    const uint64_t value_value = *value_pointer;
    switch (arg) {
        case STACK: {
            return *((uint64_t *)(stack + frame_pointer + value_value));
        }
        case REGISTER_AS_ADDRESS: {
            return heap[registers[value_value]];
        }
        case REGISTER: {
            return registers[value_value];
        }
        case CONST_FLOAT:
        case CONST_INT: {
            return *value_pointer;
        }
        default:
            std::cout << "Unknown Type: " << (int)arg << std::endl;
            return 0;
    }
}
//1011001000000000000000000000000011101001

void runInstruction() {
    const uint16_t code = *((uint16_t *) (code_segment + instruction_pointer));
    const uint8_t instruction = INSTRUCTION(code);
    const uint8_t arg0 = TYPE1(code);
    const uint8_t arg1 = TYPE2(code);
    const uint8_t arg2 = TYPE3(code);
    std::cout << "Instruction: " <<  code << std::endl;
    switch (instruction) {
        case PUSH: {
            uint64_t *var = (uint64_t *)(stack+stack_pointer);
            uint64_t i = getValue(arg0, (uint64_t *) (code_segment + instruction_pointer + 2));
            //float *i2 = (float*)&i;
            //std::cout << *i2 << " pushed"  << std::endl;
            *var = i;
            stack_pointer+=8;
            instruction_pointer += 10;
            break;
        }
        case MOVE:
            std::cout << "Move" << std::endl;
            break;
        case ADD:
            std::cout << "Add" << std::endl;
            break;
        case PRINT: {
            uint64_t i = getValue(arg0, (uint64_t *) (code_segment + instruction_pointer + 2));
            std::cout << "[PRINT] " << i << std::endl;
            instruction_pointer += 10;
            break;
        }
        default:
            std::cout << "Unknown Instruction: " << (int)instruction << std::endl;
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
