//
// Created by Mo on 15.04.2022.
//
#include "DPI_Syntax.h"

namespace Syntax {
    Token_Capture BRACES_OPEN = {0, R"(^\{)", R"(^\{$)", R"(^\{)"};
    Token_Capture BRACES_CLOSED = {1, R"(^\})", R"(^\}$)", R"(^\})"};
    Token_Capture PARENTHESES_OPEN = {2, R"(^\()", R"(^\($)", R"(^\()"};
    Token_Capture PARENTHESES_CLOSED = {3, R"(^\))", R"(^\)$)", R"(^\))"};
    Token_Capture BRACKET_OPEN = {4, R"(^\[)", R"(^\[$)", R"(^\[)"};
    Token_Capture BRACKET_CLOSED = {5, R"(^\])", R"(^\]$)", R"(^\])"};
    Token_Capture KEYWORD = {6, R"(^[a-zA-Z]+$)", R"(^\w+$)", ""};
    Token_Capture NUMBER = {7, R"(^[0-9])", R"(^[.0-9]*$)", R"(^[0-9])"};
    Token_Capture DOT = {8, R"(^\.$)", R"(^\.$)", R"(^\.)"};
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
//    Token_Capture LINE_COMMENT = {43, R"(^;)", R"(^;$)", R"(^;)"};
    Token_Capture EOL = {44, "\\n", "\\n", ""};
    Token_Capture END_OF_STATEMENT = {45, R"(^;)", R"(^;$)", R"(^;)"};
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
    const uint8_t JUMP_A = 11;
    const uint8_t AND = 12;
    const uint8_t OR = 13;
    const uint8_t NOT = 14;
    const uint8_t SMALLER = 15;
    const uint8_t SMALLER_EQUAL = 16;
    const uint8_t GREATER = 17;
    const uint8_t GREATER_EQUAL = 18;
    const uint8_t EQUALS = 19;
    const uint8_t NOT_EQUALS = 20;
    const uint8_t APPROX_EQUALS = 21;
    const uint8_t PRINT = 22;
    const uint8_t NEW = 23;
    const uint8_t NOP = 24;
    const uint8_t CALL = 25;
    const uint8_t RETURN = 26;
    const uint8_t END_OF_CODE = 27;

}


namespace LexNode {
    const uint8_t FUNCTION_DECLARATION = 0;
    const uint8_t CALL_ARGUMENTS = 1;
    const uint8_t CALL_ARGUMENT = 2;
    const uint8_t EMPTY_INPUT = 3;
    const uint8_t IDENTIFIER = 4;
    const uint8_t TYPE_IDENTIFIER = 5;
    const uint8_t EXPRESSION = 6;
    const uint8_t BOOL_EXPRESSION = 7;
    const uint8_t COMPARISON_EXPRESSION = 8;
    const uint8_t ARITHMETIC = 9;
    const uint8_t TERM = 10;
    const uint8_t FACTOR = 11;
    const uint8_t MEMBER = 12;
    const uint8_t MEMBER_START = 13;
    const uint8_t VAR_TERMINAL = 14;
    const uint8_t ARRAY_INIT = 15;
    const uint8_t EMPTY_STATEMENT = 16;
    const uint8_t UNARY = 17;
    const uint8_t NUMBER = 18;
    const uint8_t NOT = 19;
    const uint8_t AND = 20;
    const uint8_t OR = 21;
    const uint8_t MUL = 22;
    const uint8_t DIV = 23;
    const uint8_t ADD = 24;
    const uint8_t SUB = 25;
    const uint8_t MOD = 26;
    const uint8_t SMALLER = 27;
    const uint8_t SMALLER_EQ = 28;
    const uint8_t GREATER = 29;
    const uint8_t GREATER_EQ = 30;
    const uint8_t EQUALS = 31;
    const uint8_t NOT_EQUALS = 32;
    const uint8_t TOP = 33;
    const uint8_t STATIC_BLOCK = 34;
    const uint8_t STRUCT_BLOCK = 35;
    const uint8_t DEFINITION = 36;
    const uint8_t DEFINITIONS_LIST = 37;
    const uint8_t TYPE = 38;
    const uint8_t TYPE_TYPE = 39;
    const uint8_t VAR_DEFINITION_SHORT = 40;
    const uint8_t TYPE_AND_ID = 41;
    const uint8_t INPUT_DEFINITIONS = 42;
    const uint8_t INPUT_LIST = 43;
    const uint8_t OUTPUT_DEFINITION = 44;
    const uint8_t INPUT_AND_OUTPUT = 45;
    const uint8_t STATEMENT = 46;
    const uint8_t BLOCK_STATEMENT = 47;
    const uint8_t FLOW_STATEMENT = 48;
    const uint8_t SINGLE_STATEMENT = 49;
    const uint8_t STATEMENT_LIST = 50;
    const uint8_t STATIC_FUNCTION = 51;
    const uint8_t STATIC_LIST = 52;
    const uint8_t IF_STATEMENT = 53;
    const uint8_t ELSE_BODY = 54;
    const uint8_t IF_BODY_AND_CONDITION = 55;
    const uint8_t VAR_DEFINITION = 56;
    const uint8_t VAR_ACTION = 57;
    const uint8_t VAR_ASSIGNMENT = 58;
    const uint8_t VAR_CALL = 59;
    const uint8_t ARRAY_ACCESS = 60;
    const uint8_t MEMBER_ACCESS = 61;
    const uint8_t FUNCTION_ACCESS = 62;

    const std::vector<std::string> NAMES = {
            "FUNCTION_DECLARATION",
            "CALL_ARGUMENTS",
            "CALL_ARGUMENT",
            "EMPTY_INPUT",
            "IDENTIFIER",
            "TYPE_IDENTIFIER",
            "EXPRESSION",
            "BOOL_EXPRESSION",
            "COMPARISON_EXPRESSION",
            "ARITHMETIC",
            "TERM",
            "FACTOR",
            "MEMBER",
            "MEMBER_START",
            "VAR_TERMINAL",
            "ARRAY_INIT",
            "EMPTY_STATEMENT",
            "UNARY",
            "NUMBER",
            "NOT",
            "AND",
            "OR",
            "MUL",
            "DIV",
            "ADD",
            "SUB",
            "MOD",
            "SMALLER",
            "SMALLER_EQ",
            "GREATER",
            "GREATER_EQ",
            "EQUALS",
            "NOT_EQUALS",
            "TOP",
            "STATIC_BLOCK",
            "STRUCT_BLOCK",
            "DEFINITION",
            "DEFINITIONS_LIST",
            "TYPE",
            "TYPE_TYPE",
            "VAR_DEFINITION_SHORT",
            "TYPE_AND_ID",
            "INPUT_DEFINITIONS",
            "INPUT_LIST",
            "OUTPUT_DEFINITION",
            "INPUT_AND_OUTPUT",
            "STATEMENT",
            "BLOCK_STATEMENT",
            "FLOW_STATEMENT",
            "SINGLE_STATEMENT",
            "STATEMENT_LIST",
            "STATIC_FUNCTION",
            "STATIC_LIST",
            "IF_STATEMENT",
            "ELSE_BODY",
            "IF_BODY_AND_CONDITION",
            "VAR_DEFINITION",
            "VAR_ACTION",
            "VAR_ASSIGNMENT",
            "VAR_CALL",
            "ARRAY_ACCESS",
            "MEMBER_ACCESS",
            "FUNCTION_ACCESS",
    };
}