#include <iostream>
#include "Tokenizer.h"
#include "IterableStream.h"

const uint8_t REGISTER = 1;
const uint8_t STACK = 2;
const uint8_t POINTER = 3;
const uint8_t CONST_INT = 4;
const uint8_t CONST_FLOAT = 5;


typedef union {
    float as_float;
    int64_t as_int;
    bool as_bool;
    void *as_pointer;
} Value;

typedef struct {
    uint8_t type;
    Value value;
} Object;

typedef union {
    uint8_t u;
    int8_t s;
} Byte_Field;

typedef struct {
    uint8_t op;
    Byte_Field A;
    Byte_Field B;
    Byte_Field C;
} Op_Code;

typedef union {
    int32_t as_int;
    Op_Code code;
} Instruction;



#define Register(index) record_stack[activation_record_pointer+(index)]

uint64_t activation_record_pointer = 0;
Object *record_stack;



const uint64_t stack_size_default = 64;


int main() {
    std::cout << sizeof(Byte_Field) << std::endl;
    std::cout << sizeof(Op_Code) << std::endl;
    std::cout << sizeof(Instruction) << std::endl;
    std::vector<Token_Capture> caps;
    caps.push_back({0,"^[a-zA-Z]+$","^[a-zA-Z0-9_\\-]+$"});
    caps.push_back({1,"^\\s$","^\\s+$"});

    Tokenizer tokenizer(caps);
    std::string str = "HELLO world";
    const char *string = str.c_str();
    std::vector<Token>* stream = tokenizer.generate_stream(string, str.length());

    IterableStream iterableStream(stream);

    while(iterableStream.hasEntriesLeft()) {
        const Token &current = iterableStream.current();
        std::cout << current.raw_string << " | " << current.type << std::endl;
        iterableStream.consume();
    }

    std::vector<Token> filtered(*stream);
    filtered.erase(std::remove_if(filtered.begin(), filtered.end(),  [](const Token& token) {return token.type == 1;}) , filtered.end());
    std::cout<< filtered.size() << std::endl;
    std::cout<< stream->size() << std::endl;

    delete stream;

    return 0;
}

