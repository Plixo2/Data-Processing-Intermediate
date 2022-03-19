#include <iostream>

const int REGISTER = 0b001;
const int STACK = 0b010;
const int ADDRESS = 0b011;
const int CONST_INT = 0b100;
const int CONST_FLOAT = 0b101;

const int MOVE = 0b000001;
const int ADD = 0b001001;

#define ARGUMENT_SIZE 3
#define INSTRUCTION_SIZE 6
#define ARG1 6
#define ARG2 9
#define ARG3 12

#define INSTRUCTION(code) (code & 0b111111)
#define TYPE1(code) ((code & 0b000111000000) >> ARG1)
#define TYPE2(code) ((code & 0b111000000000) >> ARG2)
#define TYPE3(code) ((code & 0b111000000000000) >> ARG3)


int main() {
        int fin = MOVE | (STACK << ARG1) | (CONST_INT << ARG2);

    std::cout << fin << std::endl;
  //  std::cout << TYPE2(fin) << std::endl;
   // std::cout << TYPE1(fin) << std::endl;
  //  std::cout << INSTRUCTION(fin) << std::endl;

    return 0;
}

