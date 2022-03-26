#include <iostream>
#include "Tokenizer.h"
#include "IterableStream.h"
#include "DPI_Syntax.h"

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

    caps.push_back(Syntax::BRACES_OPEN);
    caps.push_back(Syntax::BRACES_CLOSED);
    caps.push_back(Syntax::PARENTHESES_OPEN);
    caps.push_back(Syntax::PARENTHESES_CLOSED);
    caps.push_back(Syntax::A_PLUS);
    caps.push_back(Syntax::A_MINUS);
    caps.push_back(Syntax::A_MULTIPLY);
    caps.push_back(Syntax::A_DIVIDE);
    caps.push_back(Syntax::A_MOD);
    caps.push_back(Syntax::A_POW);
    caps.push_back(Syntax::SMALLER);
    caps.push_back(Syntax::SMALLER_EQUAL);
    caps.push_back(Syntax::GREATER);
    caps.push_back(Syntax::GREATER_EQUAL);
    caps.push_back(Syntax::EQUALS);
    caps.push_back(Syntax::TRUE);
    caps.push_back(Syntax::FALSE);
    caps.push_back(Syntax::APPROX_EQUALS);
    caps.push_back(Syntax::NOT_EQUALS);
    caps.push_back(Syntax::B_AND);
    caps.push_back(Syntax::B_OR);
    caps.push_back(Syntax::B_NOT);
    caps.push_back(Syntax::ASSIGN);
    caps.push_back(Syntax::END_OF_STATEMENT);
    caps.push_back(Syntax::KEYWORD);
    caps.push_back(Syntax::NUMBER);
    caps.push_back(Syntax::WHITESPACE);

    Tokenizer tokenizer(caps);
    std::string str = "HELLO *true / tru+ tr ^ 2 -t ~= true < > *= != && >= <= (  { ) } !false !=hello && truea; || atrue === true";
    const char *string = str.c_str();
    std::vector<Token> *stream = tokenizer.generate_stream(string, str.length());
    stream->push_back({Syntax::_EOF.type, "", str.length(), str.length()});


    std::vector<Token> filtered(*stream);
    filtered.erase(std::remove_if(filtered.begin(), filtered.end(), [](const Token &token) {
        return token.type == Syntax::WHITESPACE.type ||
               token.type == Syntax::EOL.type ||
               token.type == Syntax::_EOF.type;
    }), filtered.end());

//    std::cout << filtered.size() << std::endl;
//    std::cout << stream->size() << std::endl;

    IterableStream iterableStream(&filtered);
    uint64_t i = 3;
    while (iterableStream.hasEntriesLeft()) {
        const Token &current = iterableStream.current();
        std::cout << current.type << " | " << current.raw_string << std::endl;
        i += current.type * i;
        iterableStream.consume();
    }
    std::cout << "h: " << i % 10000 << std::endl;

    delete stream;

    return 0;
}

