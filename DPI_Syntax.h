#ifndef DATA_PROCESSING_INTERMEDIATE_DPI_SYNTAX_H
#define DATA_PROCESSING_INTERMEDIATE_DPI_SYNTAX_H

#include <cstdint>
#include "Tokenizer.h"


namespace Syntax {

    extern Token_Capture BRACES_OPEN;
    extern Token_Capture BRACES_CLOSED;
    extern Token_Capture PARENTHESES_OPEN;
    extern Token_Capture PARENTHESES_CLOSED;
    extern Token_Capture BRACKET_OPEN;
    extern Token_Capture BRACKET_CLOSED;
    extern Token_Capture KEYWORD;
    extern Token_Capture NUMBER;
    extern Token_Capture DOT;
    extern Token_Capture SEPARATOR;
    extern Token_Capture WHITESPACE;
    extern Token_Capture B_AND;
    extern Token_Capture B_OR;
    extern Token_Capture B_NOT;
    extern Token_Capture A_PLUS;
    extern Token_Capture A_MINUS;
    extern Token_Capture A_MULTIPLY;
    extern Token_Capture A_DIVIDE;
    extern Token_Capture A_MOD;
    extern Token_Capture A_POW;
    extern Token_Capture SMALLER;
    extern Token_Capture SMALLER_EQUAL;
    extern Token_Capture GREATER;
    extern Token_Capture GREATER_EQUAL;
    extern Token_Capture EQUALS;
    extern Token_Capture NOT_EQUALS;
    extern Token_Capture APPROX_EQUALS;
    extern Token_Capture TRUE;
    extern Token_Capture FALSE;
    extern Token_Capture IF;
    extern Token_Capture ELSE;
    extern Token_Capture FOR;
    extern Token_Capture IN;
    extern Token_Capture FROM;
    extern Token_Capture STRUCT;
    extern Token_Capture STATIC;
    extern Token_Capture LAMBDA_SHORTFORM;
    extern Token_Capture TO;
    extern Token_Capture AS;
    extern Token_Capture FN;
    extern Token_Capture ASSIGN;
    extern Token_Capture ARROW;
    extern Token_Capture COMMENT;
    extern Token_Capture LINE_COMMENT;
    extern Token_Capture EOL;
    extern Token_Capture END_OF_STATEMENT;
    // Token_Capture END_OF_STATEMENT = {35, R"(^;)", R"(^;$)", R"(^;)"};
}

namespace IRCode {
    extern const uint8_t MOVE;
    extern const uint8_t ADD;
    extern const uint8_t MINUS;
    extern const uint8_t MULTIPLY;
    extern const uint8_t DIVIDE;
    extern const uint8_t MOD;
    extern const uint8_t POW;
    extern const uint8_t LOAD_CONST;
    extern const uint8_t LOAD_MEMBER;
    extern const uint8_t SET_MEMBER;
    extern const uint8_t JUMP;
    extern const uint8_t JUMP_A;
    extern const uint8_t AND;
    extern const uint8_t OR;
    extern const uint8_t NOT;
    extern const uint8_t SMALLER;
    extern const uint8_t SMALLER_EQUAL;
    extern const uint8_t GREATER;
    extern const uint8_t GREATER_EQUAL;
    extern const uint8_t EQUALS;
    extern const uint8_t NOT_EQUALS;
    extern const uint8_t APPROX_EQUALS;
    extern const uint8_t PRINT;
    extern const uint8_t NEW;
    extern const uint8_t NOP;
    extern const uint8_t CALL;
    extern const uint8_t RETURN;
    extern const uint8_t END_OF_CODE;

}

namespace LexNode {
    extern const uint8_t ASSIGN_STATEMENT;
    extern const uint8_t VAR_CREATION_STATEMENT;
    extern const uint8_t FUNCTION_DECLARATION;
    extern const uint8_t VARIABLE_DECLARATION;
    extern const uint8_t ARGUMENT;
    extern const uint8_t FUNCTION_CALL;
    extern const uint8_t CALL_ARGUMENTS;
    extern const uint8_t BRANCH_POSITIVE;
    extern const uint8_t BRANCH_NEGATIVE;
    extern const uint8_t EMPTY_EXPRESSION;
    extern const uint8_t EMPTY_INPUT;
    extern const uint8_t FUNCTION_IO;
    extern const uint8_t FUNCTION_OUTPUT;
    extern const uint8_t IDENTIFIER;
    extern const uint8_t TYPE_IDENTIFIER;
    extern const uint8_t EXPRESSION;
    extern const uint8_t BOOL_EXPRESSION;
    extern const uint8_t COMPARISON_EXPRESSION;
    extern const uint8_t ARITHMETIC;
    extern const uint8_t TERM;
    extern const uint8_t FACTOR;
    extern const uint8_t MEMBER;
    extern const uint8_t VAR_TERMINAL;
    extern const uint8_t ARRAY_INIT;
    extern const uint8_t EMPTY_STATEMENT;
    extern const uint8_t ADDITION;
    extern const uint8_t UNARY;
    extern const uint8_t NUMBER;
    extern const uint8_t B_AND;
    extern const uint8_t B_OR;
    extern const uint8_t B_NOT;
    extern const uint8_t A_PLUS;
    extern const uint8_t A_MINUS;
    extern const uint8_t A_MULTIPLY;
    extern const uint8_t A_DIVIDE;
    extern const uint8_t A_MOD;
    extern const uint8_t A_POW;
    extern const uint8_t SMALLER;
    extern const uint8_t SMALLER_EQUAL;
    extern const uint8_t GREATER;
    extern const uint8_t GREATER_EQUAL;
    extern const uint8_t EQUALS;
    extern const uint8_t NOT_EQUALS;
    extern const uint8_t APPROX_EQUALS;
    extern const uint8_t STATEMENT_BLOCK;
    extern const uint8_t TOP;
    extern const uint8_t STATIC_BLOCK;
    extern const uint8_t STRUCT_BLOCK;
    extern const uint8_t DEFINITION;
    extern const uint8_t DEFINITIONS_LIST;
    extern const uint8_t TYPE;
    extern const uint8_t TYPE_TYPE;
    extern const uint8_t VAR_DEFINITION;
    extern const uint8_t VAR_DEFINITION_SHORT;
    extern const uint8_t TYPE_AND_ID;
    extern const uint8_t INPUT_DEFINITIONS;
    extern const uint8_t INPUT_LIST;
    extern const uint8_t OUTPUT_DEFINITION;
    extern const uint8_t INPUT_AND_OUTPUT;
    extern const uint8_t STATEMENT;
    extern const uint8_t BLOCK_STATEMENT;
    extern const uint8_t FLOW_STATEMENT;
    extern const uint8_t SINGLE_STATEMENT;
    extern const uint8_t STATEMENT_LIST;
    extern const uint8_t STATIC_FUNCTION;
    extern const uint8_t STATIC_LIST;
    extern const uint8_t STATIC_FUNCTION_LIST;
    extern const uint8_t IF_STATEMENT;
    extern const uint8_t ELSE_BODY;
    extern const uint8_t OLD1;
    extern const uint8_t IF_BODY_AND_CONDITION;
    extern const uint8_t OLD0;
    extern const uint8_t VAR_ASSIGNMENT;
    extern const uint8_t VAR_CALL;

    extern const std::vector<std::string> NAMES;
}


#endif //DATA_PROCESSING_INTERMEDIATE_DPI_SYNTAX_H
