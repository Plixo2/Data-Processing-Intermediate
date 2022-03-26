//
// Created by Mo on 24.03.2022.
//

#ifndef DATA_PROCESSING_INTERMEDIATE_DPI_SYNTAX_H
#define DATA_PROCESSING_INTERMEDIATE_DPI_SYNTAX_H

#include <cstdint>
#include "Tokenizer.h"

namespace Syntax {

    Token_Capture BRACES_OPEN = {8, R"(^\{)", R"(^\{$)", R"(^\{)"};
    Token_Capture BRACES_CLOSED = {9, R"(^\})", R"(^\}$)", R"(^\})"};
    Token_Capture PARENTHESES_OPEN = {6, R"(^\()", R"(^\($)", R"(^\()"};
    Token_Capture PARENTHESES_CLOSED = {7, R"(^\))", R"(^\)$)", R"(^\))"};
    Token_Capture KEYWORD = {2, R"(^[a-zA-Z]+$)",R"(^\w+$)", ""};
    Token_Capture NUMBER = {5, R"(^[.0-9])", R"(^[0-9]*(.[0-9]+)?$)", R"(^([0-9])|(.[0-9]))"};
    Token_Capture WHITESPACE = {1, R"(^\s$)",R"(^[^\S\r\n]+$)", ""};
    Token_Capture B_AND = {10, "^&", "^(&|&&)$", R"(^&&)"};
    Token_Capture B_OR = {11, R"(^\|)", R"(^(\||\|\|)$)", R"(^\|\|)"};
    Token_Capture B_NOT = {15, R"(^!)", R"(^(!)$)", R"(^!)"};
    Token_Capture A_PLUS = {20, R"(^\+)", R"(^(\+)$)", R"(^\+)"};
    Token_Capture A_MINUS = {21, R"(^\-)", R"(^(\-)$)", R"(^\-)"};
    Token_Capture A_MULTIPLY = {22, R"(^\*)", R"(^(\*)$)", R"(^\*)"};
    Token_Capture A_DIVIDE = {23, R"(^\/)", R"(^(\/)$)", R"(^\/)"};
    Token_Capture A_MOD = {0, "", "", ""};
    Token_Capture A_POW = {24, R"(^\^)", R"(^(\^)$)", R"(^\^)"};
    Token_Capture SMALLER = {19, R"(^<)", R"(^(<)$)", R"(^<[^=]+)"};
    Token_Capture SMALLER_EQUAL = {16, R"(^<)", R"(^(<|<=)$)", R"(^<=)"};
    Token_Capture GREATER = {18, R"(^>)", R"(^(>)$)", R"(^>[^=]+)"};
    Token_Capture GREATER_EQUAL = {17, R"(^>)", R"(^(>|>=)$)", R"(^>=)"};
    Token_Capture EQUALS = {12, R"(^=)", R"(^(=|==)$)", R"(^==)"};
    Token_Capture TRUE = {3,"^t","^(true|tru|tr|t)$",R"(^true\b)"};
    Token_Capture FALSE = {4,"^f","^(false|fals|fal|fa|f)$",R"(^false\b)"};
    Token_Capture APPROX_EQUALS = {13, R"(^~)", R"(^(~|~=)$)", R"(^~=)"};
    Token_Capture NOT_EQUALS = {14, R"(^!)", R"(^(!|!=)$)", R"(^!=)"};
    Token_Capture ASSIGN = {26, R"(^=)", R"(^=$)", R"(^=[^=]+)"};
    Token_Capture END_OF_STATEMENT = {30, R"(^;)", R"(^;$)", R"(^;)"};
    Token_Capture EOL = {99, "\\n", "\\n", ""};
    Token_Capture _EOF = {100, "", "", ""};
}

namespace IRCode {

}

namespace LexNode {
    const uint8_t ASSIGN_STATEMENT = 1;
    const uint8_t IDENTIFIER = 2;
    const uint8_t EXPRESSION = 3;
    const uint8_t BOOL_EXPRESSION = 4;
    const uint8_t FLOAT_CONSTANT = 5;
    const uint8_t INT_CONSTANT = 6;
    const uint8_t ADDITION = 7;
    const uint8_t UNARY = 8;

    const uint8_t B_AND = 50;
    const uint8_t B_OR = 51;
    const uint8_t B_NOT = 52;
    const uint8_t A_PLUS = 60;
    const uint8_t A_MINUS = 61;
    const uint8_t A_MULTIPLY = 62;
    const uint8_t A_DIVIDE = 63;
    const uint8_t A_MOD = 64;
    const uint8_t A_POW = 65;
    const uint8_t EQUALS = 66;
    const uint8_t NOT_EQUALS = 67;
}

/*
enum Syntax {
    BRACES_OPEN,
    BRACES_CLOSED,
    PARENTHESES_OPEN,
    PARENTHESES_CLOSED,
    KEYWORD,
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
