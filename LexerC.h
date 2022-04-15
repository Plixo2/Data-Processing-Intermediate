//
// Created by Mo on 11.04.2022.
//

#ifndef DATA_PROCESSING_INTERMEDIATE_LEXERC_H
#define DATA_PROCESSING_INTERMEDIATE_LEXERC_H


#include "IterableStream.h"


class AssertionException : public std::exception {
    const char *text;

    virtual const char *what() const noexcept {
        return text;
    }

public:
    explicit AssertionException(std::string *text) : text(text->c_str()) {}
    explicit AssertionException(const char *text) : text(text) {}
};

struct SyntaxNode {
    uint8_t lex_type = 0;
    std::string data;
    SyntaxNode *left;
    SyntaxNode *right;

    ~SyntaxNode() {
        delete left;
        delete right;
    }
};

class LexerC {
private:
    IterableStream *token_stream;
public:
    LexerC(IterableStream *token_stream);
    SyntaxNode *topLevel();

    SyntaxNode *staticBlock();
    SyntaxNode *structBlock();

    SyntaxNode *statement();
    SyntaxNode *blockStatement();
    SyntaxNode *blockStatementList();

    SyntaxNode *flowStatement();
    SyntaxNode *normalStatement();

    SyntaxNode *definitions();
    SyntaxNode *definitionsList();

    SyntaxNode *varDefinition();
    SyntaxNode *varDefinitionShort();
    SyntaxNode *varAssignment();
    SyntaxNode *varCall();

    SyntaxNode *outDefinitions();
    SyntaxNode *inputDefinitions();
    SyntaxNode *inputDefs();
    SyntaxNode *inputDefinitionsShort();
    SyntaxNode *inputDefsShort();

    SyntaxNode *functionDefShort();
    SyntaxNode *functionDef();
    SyntaxNode *typeDef();
    SyntaxNode *idDef();

    SyntaxNode *argList();
    SyntaxNode *member();
    SyntaxNode *varTerminal();
    SyntaxNode *arrayInitializer();

    SyntaxNode *staticList();
    SyntaxNode *staticFunction();

    SyntaxNode *_if();
    SyntaxNode *_for();



    SyntaxNode *expression();

    SyntaxNode *boolArithmetic();
    SyntaxNode *comparisonArithmetic();
    SyntaxNode *arithmetic();
    SyntaxNode *term();
    SyntaxNode *factor();



};



#endif //DATA_PROCESSING_INTERMEDIATE_LEXERC_H
