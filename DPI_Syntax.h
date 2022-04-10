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
    Token_Capture BRACKET_OPEN = {4, R"(^\[)", R"(^\[$)", R"(^\[)"};
    Token_Capture BRACKET_CLOSED = {5, R"(^\])", R"(^\]$)", R"(^\])"};
    Token_Capture KEYWORD = {6, R"(^[a-zA-Z]+$)", R"(^\w+$)", ""};
    Token_Capture NUMBER = {7, R"(^[0-9])", R"(^[.0-9]*$)", R"(^[0-9])"};
    Token_Capture DOT = {8, R"(^.$)", R"(^.$)", R"(^.)"};
    Token_Capture SEPARATOR = {9, R"(^,)", R"(^,$)", R"(^,)"};
    Token_Capture WHITESPACE = {10, R"(^\s$)", R"(^[^\S\r\n]+$)", ""};
    Token_Capture B_AND = {11, "^&", "^(&|&&)$", R"(^&&)"};
    Token_Capture B_OR = {12, R"(^\|)", R"(^(\||\|\|)$)", R"(^\|\|)"};
    Token_Capture B_NOT = {13, R"(^!)", R"(^(!)$)", R"(^!)"};
    Token_Capture A_PLUS = {14, R"(^\+)", R"(^(\+)$)", R"(^\+)"};
    Token_Capture A_MINUS = {15, R"(^\-)", R"(^(\-)$)", R"(^\-[^>]+)"};
    Token_Capture A_MULTIPLY = {16, R"(^\*)", R"(^(\*)$)", R"(^\*)"};
    Token_Capture A_DIVIDE = {17, R"(^\/)", R"(^(\/)$)", R"(^\/)"};
    Token_Capture A_MOD = {18, "", "", ""};
    Token_Capture A_POW = {19, R"(^\^)", R"(^(\^)$)", R"(^\^)"};
    Token_Capture SMALLER = {20, R"(^<)", R"(^(<)$)", R"(^<[^=]+)"};
    Token_Capture SMALLER_EQUAL = {21, R"(^<)", R"(^(<|<=)$)", R"(^<=)"};
    Token_Capture GREATER = {22, R"(^>)", R"(^(>)$)", R"(^>[^=]+)"};
    Token_Capture GREATER_EQUAL = {23, R"(^>)", R"(^(>|>=)$)", R"(^>=)"};
    Token_Capture EQUALS = {24, R"(^=)", R"(^(=|==)$)", R"(^==)"};
    Token_Capture NOT_EQUALS = {25, R"(^!)", R"(^(!|!=)$)", R"(^!=)"};
    Token_Capture APPROX_EQUALS = {26, R"(^~)", R"(^(~|~=)$)", R"(^~=)"};
    Token_Capture TRUE = {27, "^t", "^(true|tru|tr|t)$", R"(^true\b)"};
    Token_Capture FALSE = {28, "^f", "^(false|fals|fal|fa|f)$", R"(^false\b)"};
    Token_Capture IF = {29, "^i", "^(if|i)$", R"(^if\b)"};
    Token_Capture ELSE = {30, "^e", "^(else|els|el|e)$", R"(^else\b)"};
    Token_Capture FOR = {31, "^f", "^(for|fo|f)$", R"(^for\b)"};
    Token_Capture IN = {32, "^i", "^(in|i)$", R"(^in\b)"};
    Token_Capture FROM = {33, "^f", "^(from|fro|fr|f)$", R"(^from\b)"};
    Token_Capture STRUCT = {34, "^s", "^(struct|struc|stru|str|st|s)$", R"(^struct\b)"};
    Token_Capture STATIC = {35, "^s", "^(static|stati|stat|sta|st|s)$", R"(^static\b)"};
    Token_Capture LAMBDA_SHORTFORM = {36, "^\\s$", "^\\s*$", R"(^\s*\w+\s+\w+\:)"};
    Token_Capture TO = {37, "^t", "^(to|t)$", R"(^to\b)"};
    Token_Capture AS = {38, "^a", "^(as|a)$", R"(^as\b)"};
    Token_Capture FN = {39, "^f", "^(fn|f)$", R"(^fn\b)"};
    Token_Capture ASSIGN = {40, R"(^=)", R"(^=$)", R"(^=[^=]+)"};
    Token_Capture ARROW = {41, R"(^-)", R"(^(-|->)$)", R"(^->)"};
    Token_Capture COMMENT = {42, R"(^/)", R"(^//.*$)", R"(^//)"};
    Token_Capture LINE_COMMENT = {43, R"(^;)", R"(^;$)", R"(^;)"};
    Token_Capture EOL = {44, "\\n", "\\n", ""};
    // Token_Capture END_OF_STATEMENT = {35, R"(^;)", R"(^;$)", R"(^;)"};
}

namespace IRCode {
    const uint8_t MOVE = 0;
    const uint8_t ADD = 1;
    const uint8_t MINUS = 2;
    const uint8_t MULTIPLY = 3;
    const uint8_t DIVIDE = 4;
    const uint8_t MOD = 5;
    const uint8_t POW = 6;
    const uint8_t LOAD_CONST = 7;
    const uint8_t LOAD_MEMBER = 8;
    const uint8_t SET_MEMBER = 9;
    const uint8_t JUMP = 10;
    const uint8_t JUMP_A = 66;
    const uint8_t AND = 11;
    const uint8_t OR = 12;
    const uint8_t NOT = 13;
    const uint8_t SMALLER = 14;
    const uint8_t SMALLER_EQUAL = 15;
    const uint8_t GREATER = 16;
    const uint8_t GREATER_EQUAL = 17;
    const uint8_t EQUALS = 18;
    const uint8_t NOT_EQUALS = 19;
    const uint8_t APPROX_EQUALS = 20;
    const uint8_t PRINT = 21;
    const uint8_t NEW = 27;
    const uint8_t NOP = 22;
    const uint8_t CALL = 23;
    const uint8_t RETURN = 24;
    const uint8_t END_OF_CODE = 62;

}

namespace LexNode {
    const uint8_t ASSIGN_STATEMENT = 0;
    const uint8_t VAR_CREATION_STATEMENT = 1;
    const uint8_t FUNCTION_DECLARATION = 2;
    const uint8_t VARIABLE_DECLARATION = 3;
    const uint8_t ARGUMENT = 4;
    const uint8_t FUNCTION_CALL = 5;
    const uint8_t CALL_ARGUMENTS = 6;
    const uint8_t BRANCH_POSITIVE = 7;
    const uint8_t BRANCH_NEGATIVE = 8;
    const uint8_t BRANCH = 9;
    const uint8_t FUNCTION_INPUT = 10;
    const uint8_t FUNCTION_IO = 11;
    const uint8_t FUNCTION_OUTPUT = 12;
    const uint8_t IDENTIFIER = 13;
    const uint8_t TYPE_IDENTIFIER = 14;
    const uint8_t EXPRESSION = 15;
    const uint8_t BOOL_EXPRESSION = 16;
    const uint8_t FLOAT_CONSTANT = 17;
    const uint8_t INT_CONSTANT = 18;
    const uint8_t ADDITION = 19;
    const uint8_t UNARY = 20;
    const uint8_t NUMBER = 21;
    const uint8_t B_AND = 22;
    const uint8_t B_OR = 23;
    const uint8_t B_NOT = 24;
    const uint8_t A_PLUS = 25;
    const uint8_t A_MINUS = 26;
    const uint8_t A_MULTIPLY = 27;
    const uint8_t A_DIVIDE = 28;
    const uint8_t A_MOD = 29;
    const uint8_t A_POW = 30;
    const uint8_t SMALLER = 31;
    const uint8_t SMALLER_EQUAL = 32;
    const uint8_t GREATER = 33;
    const uint8_t GREATER_EQUAL = 34;
    const uint8_t EQUALS = 35;
    const uint8_t NOT_EQUALS = 36;
    const uint8_t APPROX_EQUALS = 37;
    const uint8_t STATEMENT_BLOCK = 38;
    const uint8_t TOP = 39;

    const std::vector<std::string> NAMES = {
            "ASSIGN_STATEMENT",
            "VAR_CREATION_STATEMENT",
            "FUNCTION_DECLARATION",
            "VARIABLE_DECLARATION",
            "ARGUMENT",
            "FUNCTION_CALL",
            "CALL_ARGUMENTS",
            "BRANCH_POSITIVE",
            "BRANCH_NEGATIVE",
            "BRANCH",
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
            "STATEMENT_BLOCK",
            "TOP",
    };
}


#endif //DATA_PROCESSING_INTERMEDIATE_DPI_SYNTAX_H
