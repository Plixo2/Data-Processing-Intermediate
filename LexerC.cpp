//
// Created by Mo on 11.04.2022.
//

#include "LexerC.h"
#include "DPI_Syntax.h"

#define MATCH(_type) if(token_stream->hasEntriesLeft() && token_stream->current().type == (_type).type)
#define NEXT token_stream->consume()
#define REMEMBER(type) std::string (type) = token_stream->current().raw_string

#define ASSERT(_type) if((_type).type != token_stream->current().type) { \
    std::string excptstr = "Assertion fail. found "; \
    excptstr += std::to_string(token_stream->current().type);                              \
    excptstr += ", but expected ";\
    excptstr += std::to_string(_type.type); \
    throw AssertionException(&excptstr);       \
}


LexerC::LexerC(IterableStream *token_stream) : token_stream(token_stream) {

}

SyntaxNode *LexerC::topLevel() {
    if (token_stream->current().type == Syntax::STRUCT.type) {

    }
}
SyntaxNode *LexerC::staticBlock() {

}

SyntaxNode *LexerC::structBlock() {

}

SyntaxNode *LexerC::statement() {

}

SyntaxNode *LexerC::blockStatement() {

}

SyntaxNode *LexerC::blockStatementList() {

}

SyntaxNode *LexerC::flowStatement() {

}

SyntaxNode *LexerC::normalStatement() {

}

SyntaxNode *LexerC::definitions() {

}

SyntaxNode *LexerC::varDefinition() {

}

SyntaxNode *LexerC::varAssignment() {

}

SyntaxNode *LexerC::varCall() {

}

SyntaxNode *LexerC::outDefinitions() {

}

SyntaxNode *LexerC::inputDefinitions() {

}

SyntaxNode *LexerC::inputDefs() {

}

SyntaxNode *LexerC::functionDef() {

}

SyntaxNode *LexerC::typeDef() {

}

SyntaxNode *LexerC::idDef() {

}

SyntaxNode *LexerC::argList() {

}

SyntaxNode *LexerC::member() {

}

SyntaxNode *LexerC::varTerminal() {

}

SyntaxNode *LexerC::arrayInitializer() {

}

SyntaxNode *LexerC::expression() {

}

SyntaxNode *LexerC::boolArithmetic() {

}

SyntaxNode *LexerC::comparisonArithmetic() {

}

SyntaxNode *LexerC::arithmetic() {

}

SyntaxNode *LexerC::term() {

}

SyntaxNode *LexerC::factor() {

}


SyntaxNode *createLeaf(uint8_t lex_type, std::string literal) {
    auto *node = new SyntaxNode{lex_type, std::move(literal)};
    return node;
}

SyntaxNode *createNode(uint8_t lex_type, SyntaxNode *child) {
    auto *node = new SyntaxNode{lex_type, "", child};
    return node;
}

SyntaxNode *createNode(uint8_t lex_type, SyntaxNode *left, SyntaxNode *right) {
    auto *node = new SyntaxNode{lex_type, "", left, right};
    return node;
}
