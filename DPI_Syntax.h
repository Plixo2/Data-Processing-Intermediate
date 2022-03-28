//
// Created by Mo on 24.03.2022.
//

#ifndef DATA_PROCESSING_INTERMEDIATE_DPI_SYNTAX_H
#define DATA_PROCESSING_INTERMEDIATE_DPI_SYNTAX_H

#include <cstdint>
#include "Tokenizer.h"

namespace Syntax {

    Token_Capture BRACES_OPEN = {0, R"(^\{)", R"(^\{$)", R"(^\{)"};
    Token_Capture BRACES_CLOSED = {1, R"(^\})", R"(^\}$)", R"(^\})"};
    Token_Capture PARENTHESES_OPEN = {2, R"(^\()", R"(^\($)", R"(^\()"};
    Token_Capture PARENTHESES_CLOSED = {3, R"(^\))", R"(^\)$)", R"(^\))"};
    Token_Capture KEYWORD = {4, R"(^[a-zA-Z]+$)", R"(^\w+$)", ""};
    Token_Capture NUMBER = {5, R"(^[.0-9])", R"(^[.0-9]*$)", R"(^([0-9])|(.[0-9]))"};
    Token_Capture WHITESPACE = {6, R"(^\s$)", R"(^[^\S\r\n]+$)", ""};
    Token_Capture B_AND = {7, "^&", "^(&|&&)$", R"(^&&)"};
    Token_Capture B_OR = {8, R"(^\|)", R"(^(\||\|\|)$)", R"(^\|\|)"};
    Token_Capture B_NOT = {9, R"(^!)", R"(^(!)$)", R"(^!)"};
    Token_Capture A_PLUS = {10, R"(^\+)", R"(^(\+)$)", R"(^\+)"};
    Token_Capture A_MINUS = {11, R"(^\-)", R"(^(\-)$)", R"(^\-)"};
    Token_Capture A_MULTIPLY = {12, R"(^\*)", R"(^(\*)$)", R"(^\*)"};
    Token_Capture A_DIVIDE = {13, R"(^\/)", R"(^(\/)$)", R"(^\/)"};
    Token_Capture A_MOD = {14, "", "", ""};
    Token_Capture A_POW = {15, R"(^\^)", R"(^(\^)$)", R"(^\^)"};
    Token_Capture SMALLER = {16, R"(^<)", R"(^(<)$)", R"(^<[^=]+)"};
    Token_Capture SMALLER_EQUAL = {17, R"(^<)", R"(^(<|<=)$)", R"(^<=)"};
    Token_Capture GREATER = {18, R"(^>)", R"(^(>)$)", R"(^>[^=]+)"};
    Token_Capture GREATER_EQUAL = {19, R"(^>)", R"(^(>|>=)$)", R"(^>=)"};
    Token_Capture EQUALS = {20, R"(^=)", R"(^(=|==)$)", R"(^==)"};
    Token_Capture NOT_EQUALS = {24, R"(^!)", R"(^(!|!=)$)", R"(^!=)"};
    Token_Capture APPROX_EQUALS = {23, R"(^~)", R"(^(~|~=)$)", R"(^~=)"};
    Token_Capture TRUE = {21, "^t", "^(true|tru|tr|t)$", R"(^true\b)"};
    Token_Capture FALSE = {22, "^f", "^(false|fals|fal|fa|f)$", R"(^false\b)"};
    Token_Capture ASSIGN = {25, R"(^=)", R"(^=$)", R"(^=[^=]+)"};
    Token_Capture END_OF_STATEMENT = {26, R"(^;)", R"(^;$)", R"(^;)"};
    Token_Capture EOL = {27, "\\n", "\\n", ""};
    Token_Capture _EOF = {28, "", "", ""};
}

namespace IRCode {
    const uint8_t MOVE = 0;
    const uint8_t ADD = 1;
    const uint8_t LOAD_CONST = 2;
    const uint8_t LOAD_MEMBER = 3;
    const uint8_t SET_MEMBER = 4;
    const uint8_t JUMP = 5;
    const uint8_t AND = 6;
    const uint8_t OR = 7;
    const uint8_t NOT = 8;
    const uint8_t SMALLER = 9;
    const uint8_t SMALLER_EQUAL = 10;
    const uint8_t GREATER = 11;
    const uint8_t GREATER_EQUAL = 12;
    const uint8_t EQUALS = 13;
    const uint8_t NOT_EQUALS = 14;
    const uint8_t APPROX_EQUALS = 15;

}

namespace LexNode {
    const uint8_t ASSIGN_STATEMENT = 0;
    const uint8_t VAR_CREATION_STATEMENT = 1;
    const uint8_t IDENTIFIER = 2;
    const uint8_t TYPE_IDENTIFIER = 3;
    const uint8_t EXPRESSION = 4;
    const uint8_t BOOL_EXPRESSION = 5;
    const uint8_t FLOAT_CONSTANT = 6;
    const uint8_t INT_CONSTANT = 7;
    const uint8_t ADDITION = 8;
    const uint8_t UNARY = 9;
    const uint8_t NUMBER = 10;
    const uint8_t B_AND = 11;
    const uint8_t B_OR = 12;
    const uint8_t B_NOT = 13;
    const uint8_t A_PLUS = 14;
    const uint8_t A_MINUS = 15;
    const uint8_t A_MULTIPLY = 16;
    const uint8_t A_DIVIDE = 17;
    const uint8_t A_MOD = 18;
    const uint8_t A_POW = 19;
    const uint8_t SMALLER = 20;
    const uint8_t SMALLER_EQUAL = 21;
    const uint8_t GREATER = 22;
    const uint8_t GREATER_EQUAL = 23;
    const uint8_t EQUALS = 24;
    const uint8_t NOT_EQUALS = 25;
    const uint8_t APPROX_EQUALS = 26;

    const std::vector<std::string> NAMES = {
            "ASSIGN_STATEMENT",
            "VAR_CREATION_STATEMENT",
            "IDENTIFIER",
            "TYPE_IDENTIFIER",
            "EXPRESSION",
            "BOOL_EXPRESSION",
            "FLOAT_CONSTANT",
            "INT_CONSTANT",
            "ADDITION",
            "UNARY",
            "NUMBER",
            "B_AND",
            "B_OR",
            "B_NOT",
            "A_PLUS",
            "A_MINUS",
            "A_MULTIPLY",
            "A_DIVIDE",
            "A_MOD",
            "A_POW",
            "SMALLER",
            "SMALLER_EQUAL",
            "GREATER",
            "GREATER_EQUAL",
            "EQUALS",
            "NOT_EQUALS",
            "APPROX_EQUALS"
    };
}


#endif //DATA_PROCESSING_INTERMEDIATE_DPI_SYNTAX_H
