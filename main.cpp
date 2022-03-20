#include <iostream>

const int REGISTER = 0b001;
const int STACK = 0b010;
const int ADDRESS = 0b011;
const int CONST_INT = 0b100;
const int CONST_FLOAT = 0b101;

const int MOVE = 0b000001;
const int PRINT = 0b000010;
const int ADD = 0b001001;

#define ARGUMENT_SIZE 3
#define INSTRUCTION_SIZE 6
#define ARG1 6
#define ARG2 9
#define ARG3 12

#define INSTRUCTION(code) (code & 0b111111)
#define TYPE1(code) ((code & 0b111000000) >> ARG1)
#define TYPE2(code) ((code & 0b111000000000) >> ARG2)
#define TYPE3(code) ((code & 0b111000000000000) >> ARG3)

uint8_t *heap;
const uint64_t heap_size_in_bytes = 512;
#define byte unsigned char

uint64_t max_long = 0xffffffffffffffff;

void start() {
    int fin = PRINT | (CONST_INT << ARG1);
    std::cout << fin << std::endl;
    uint16_t *s = (uint16_t *) (heap);
    uint64_t *l = (uint64_t *) (heap + 2);
    uint64_t *l2 = reinterpret_cast<uint64_t *>(heap + 2 + 8);
    uint64_t *l3 = reinterpret_cast<uint64_t *>(heap + 2 + 8 + 8);


    s[0] = fin;
    l[0] = 1337;
    // l2[0] = 2;
    // l3[0] = 3;

}


void runInstruction() {
    uint64_t pointer = 0l;
    uint8_t *dbg = heap;
    uint16_t code = *((uint16_t *) (heap + pointer));
    switch (INSTRUCTION(code)) {
        case MOVE:
            std::cout << "Move" << std::endl;
            break;
        case ADD:
            std::cout << "Add" << std::endl;
            break;
        case PRINT:
            switch (TYPE1(code)) {
                case CONST_INT: {
                    uint16_t code = *((uint64_t *)(heap + pointer + 2));
                    std::cout << "[PRINT] " << code << std::endl;
                    break;
                }
                default:
                    std::cout << "Unknown Value Type" << std::endl;
            }
            break;
        default:
            std::cout << "Unknown Instruction" << std::endl;
    }
}


int main() {
    heap = (uint8_t *) malloc(heap_size_in_bytes);
    try {
        start();
        runInstruction();
    } catch (...) {

    }
    free(heap);
    return 0;


}


