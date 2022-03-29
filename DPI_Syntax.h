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
    Token_Capture SEPARATOR = {6, R"(^,)", R"(^,$)", R"(^,)"};
    Token_Capture WHITESPACE = {7, R"(^\s$)", R"(^[^\S\r\n]+$)", ""};
    Token_Capture B_AND = {8, "^&", "^(&|&&)$", R"(^&&)"};
    Token_Capture B_OR = {9, R"(^\|)", R"(^(\||\|\|)$)", R"(^\|\|)"};
    Token_Capture B_NOT = {10, R"(^!)", R"(^(!)$)", R"(^!)"};
    Token_Capture A_PLUS = {11, R"(^\+)", R"(^(\+)$)", R"(^\+)"};
    Token_Capture A_MINUS = {12, R"(^\-)", R"(^(\-)$)", R"(^\-[^>]+)"};
    Token_Capture A_MULTIPLY = {13, R"(^\*)", R"(^(\*)$)", R"(^\*)"};
    Token_Capture A_DIVIDE = {14, R"(^\/)", R"(^(\/)$)", R"(^\/)"};
    Token_Capture A_MOD = {15, "", "", ""};
    Token_Capture A_POW = {16, R"(^\^)", R"(^(\^)$)", R"(^\^)"};
    Token_Capture SMALLER = {17, R"(^<)", R"(^(<)$)", R"(^<[^=]+)"};
    Token_Capture SMALLER_EQUAL = {18, R"(^<)", R"(^(<|<=)$)", R"(^<=)"};
    Token_Capture GREATER = {19, R"(^>)", R"(^(>)$)", R"(^>[^=]+)"};
    Token_Capture GREATER_EQUAL = {20, R"(^>)", R"(^(>|>=)$)", R"(^>=)"};
    Token_Capture EQUALS = {21, R"(^=)", R"(^(=|==)$)", R"(^==)"};
    Token_Capture NOT_EQUALS = {22, R"(^!)", R"(^(!|!=)$)", R"(^!=)"};
    Token_Capture APPROX_EQUALS = {23, R"(^~)", R"(^(~|~=)$)", R"(^~=)"};
    Token_Capture TRUE = {24, "^t", "^(true|tru|tr|t)$", R"(^true\b)"};
    Token_Capture FALSE = {25, "^f", "^(false|fals|fal|fa|f)$", R"(^false\b)"};
    Token_Capture ASSIGN = {26, R"(^=)", R"(^=$)", R"(^=[^=]+)"};
    Token_Capture ARROW = {27, R"(^-)", R"(^(-|->)$)", R"(^->)"};
    Token_Capture END_OF_STATEMENT = {28, R"(^;)", R"(^;$)", R"(^;)"};
    Token_Capture COMMENT = {29, R"(^/)", R"(^//.*$)", R"(^//)"};
    Token_Capture LINE_COMMENT = {30, R"(^;)", R"(^;$)", R"(^;)"};
    Token_Capture EOL = {31, "\\n", "\\n", ""};
    Token_Capture _EOF = {32, "", "", ""};
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
    const uint8_t FUNCTION_DECLARATION = 2;
    const uint8_t VARIABLE_DECLARATION = 3;
    const uint8_t FUNCTION_INPUT = 4;
    const uint8_t FUNCTION_IO = 5;
    const uint8_t FUNCTION_OUTPUT = 6;
    const uint8_t IDENTIFIER = 7;
    const uint8_t TYPE_IDENTIFIER = 8;
    const uint8_t EXPRESSION = 9;
    const uint8_t BOOL_EXPRESSION = 10;
    const uint8_t FLOAT_CONSTANT = 11;
    const uint8_t INT_CONSTANT = 12;
    const uint8_t ADDITION = 13;
    const uint8_t UNARY = 14;
    const uint8_t NUMBER = 15;
    const uint8_t B_AND = 16;
    const uint8_t B_OR = 17;
    const uint8_t B_NOT = 18;
    const uint8_t A_PLUS = 19;
    const uint8_t A_MINUS = 20;
    const uint8_t A_MULTIPLY = 21;
    const uint8_t A_DIVIDE = 22;
    const uint8_t A_MOD = 23;
    const uint8_t A_POW = 24;
    const uint8_t SMALLER = 25;
    const uint8_t SMALLER_EQUAL = 26;
    const uint8_t GREATER = 27;
    const uint8_t GREATER_EQUAL = 28;
    const uint8_t EQUALS = 29;
    const uint8_t NOT_EQUALS = 30;
    const uint8_t APPROX_EQUALS = 31;
    const uint8_t STATEMENT = 32;

    const std::vector<std::string> NAMES = {
            "ASSIGN_STATEMENT",
            "VAR_CREATION_STATEMENT",
            "FUNCTION_DECLARATION",
            "VARIABLE_DECLARATION",
            "FUNCTION_INPUT",
            "FUNCTION_IO",
            "FUNCTION_OUTPUT",
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
            "APPROX_EQUALS",
            "STATEMENT"
    };
}


#endif //DATA_PROCESSING_INTERMEDIATE_DPI_SYNTAX_H
