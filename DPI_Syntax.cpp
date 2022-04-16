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
    Token_Capture LINE_COMMENT = {43, R"(^;)", R"(^;$)", R"(^;)"};
    Token_Capture EOL = {44, "\\n", "\\n", ""};
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
    const uint8_t COMPARISON_EXPRESSION = 17;
    const uint8_t ARITHMETIC = 18;
    const uint8_t TERM = 19;
    const uint8_t FACTOR = 20;
    const uint8_t MEMBER = 21;
    const uint8_t VAR_TERMINAL = 22;
    const uint8_t ARRAY_INIT = 23;
    const uint8_t INT_CONSTANT = 24;
    const uint8_t ADDITION = 25;
    const uint8_t UNARY = 26;
    const uint8_t NUMBER = 27;
    const uint8_t B_AND = 28;
    const uint8_t B_OR = 29;
    const uint8_t B_NOT = 30;
    const uint8_t A_PLUS = 31;
    const uint8_t A_MINUS = 32;
    const uint8_t A_MULTIPLY = 33;
    const uint8_t A_DIVIDE = 34;
    const uint8_t A_MOD = 35;
    const uint8_t A_POW = 36;
    const uint8_t SMALLER = 37;
    const uint8_t SMALLER_EQUAL = 38;
    const uint8_t GREATER = 39;
    const uint8_t GREATER_EQUAL = 40;
    const uint8_t EQUALS = 41;
    const uint8_t NOT_EQUALS = 42;
    const uint8_t APPROX_EQUALS = 43;
    const uint8_t STATEMENT_BLOCK = 44;
    const uint8_t TOP = 45;
    const uint8_t STATIC_BLOCK = 46;
    const uint8_t STRUCT_BLOCK = 47;
    const uint8_t DEFINITION = 48;
    const uint8_t DEFINITIONS_LIST = 49;
    const uint8_t TYPE = 50;
    const uint8_t TYPE_TYPE = 51;
    const uint8_t VAR_DEFINITION = 52;
    const uint8_t VAR_DEFINITION_SHORT = 53;
    const uint8_t TYPE_AND_ID = 54;
    const uint8_t INPUT_DEFINITIONS = 55;
    const uint8_t INPUT_LIST = 56;
    const uint8_t OUTPUT_DEFINITION = 57;
    const uint8_t INPUT_AND_OUTPUT = 58;
    const uint8_t STATEMENT = 59;
    const uint8_t BLOCK_STATEMENT = 60;
    const uint8_t FLOW_STATEMENT = 61;
    const uint8_t SINGLE_STATEMENT = 62;
    const uint8_t STATEMENT_LIST = 63;
    const uint8_t STATIC_FUNCTION = 64;
    const uint8_t STATIC_LIST = 65;
    const uint8_t STATIC_FUNCTION_LIST = 66;
    const uint8_t IF_STATEMENT = 67;
    const uint8_t ELSE_BODY = 68;
    const uint8_t IF_BODY_AND_CONDITION = 69;
    const uint8_t VAR_ASSIGNMENT = 70;
    const uint8_t VAR_CALL = 71;


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
            "COMPARISON_EXPRESSION",
            "ARITHMETIC",
            "TERM",
            "FACTOR",
            "MEMBER",
            "VAR_TERMINAL",
            "ARRAY_INIT",
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
            "STATIC_BLOCK",
            "STRUCT_BLOCK",
            "DEFINITION",
            "DEFINITIONS_LIST",
            "TYPE",
            "TYPE_TYPE",
            "VAR_DEFINITION",
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
            "STATIC_FUNCTION_LIST",
            "IF_STATEMENT",
            "ELSE_BODY",
            "IF_BODY_AND_CONDITION",
            "VAR_ASSIGNMENT",
            "VAR_CALL",
    };
}