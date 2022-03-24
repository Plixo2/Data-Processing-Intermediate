//
// Created by Mo on 24.03.2022.
//

#ifndef DATA_PROCESSING_INTERMEDIATE_DPI_SYNTAX_H
#define DATA_PROCESSING_INTERMEDIATE_DPI_SYNTAX_H

#include <cstdint>
#include "Tokenizer.h"

namespace Syntax {

    Token_Capture BRACES_OPEN = {0, "", "", ""};
    Token_Capture BRACES_CLOSED = {0, "", "", ""};
    Token_Capture PARENTHESES_OPEN = {0, "", "", ""};
    Token_Capture PARENTHESES_CLOSED = {0, "", "", ""};
    Token_Capture IDENTIFIER = {0, "", "", ""};
    Token_Capture NUMBER = {0, "", "", ""};
    Token_Capture B_AND = {0, "", "", ""};
    Token_Capture B_OR = {0, "", "", ""};
    Token_Capture B_NOT = {0, "", "", ""};
    Token_Capture A_PLUS = {0, "", "", ""};
    Token_Capture A_MINUS = {0, "", "", ""};
    Token_Capture A_MULTIPLY = {0, "", "", ""};
    Token_Capture A_DIVIDE = {0, "", "", ""};
    Token_Capture A_MOD = {0, "", "", ""};
    Token_Capture A_POW = {0, "", "", ""};
    Token_Capture EQUALS = {0, "", "", ""};
    Token_Capture NOT_EQUALS = {0, "", "", ""};
    Token_Capture ASSIGN = {0, "", "", ""};
    Token_Capture LAMBDA = {0, "", "", ""};
    Token_Capture END_OF_STATEMENT = {0, "", "", ""};
    Token_Capture EOL = {0, "", "", ""};
    Token_Capture _EOF = {0, "", "", ""};
}

namespace IRCode {

}

namespace LexNode {
    const uint8_t ASSIGN_STATEMENT = 1;
    const uint8_t IDENTIFIER = 2;
    const uint8_t EXPRESSION = 3;
}

/*
enum Syntax {
    BRACES_OPEN,
    BRACES_CLOSED,
    PARENTHESES_OPEN,
    PARENTHESES_CLOSED,
    IDENTIFIER,
    NUMBER,
    B_AND,
    B_OR,
    B_NOT,
    A_PLUS,
    A_MINUS,
    A_MULTIPLY,
    A_DIVIDE,
    A_MOD,
    A_POW,
    EQUALS,
    NOT_EQUALS,
    ASSIGN,
    LAMBDA,
    END_OF_STATEMENT,
    EOL,
    EOF
};

*/



#endif //DATA_PROCESSING_INTERMEDIATE_DPI_SYNTAX_H
