//
// Created by Mo on 17.04.2022.
//

#ifndef DATA_PROCESSING_INTERMEDIATE_TRANSLATOR_H
#define DATA_PROCESSING_INTERMEDIATE_TRANSLATOR_H

#include "LexerC.h"
#include "DPI_Types.h"

using namespace types;

class Translator {
private:
    std::vector<Constant> constants;
    std::vector<Instruction> instructions;
    std::vector<FunctionPrototype> functions;
    std::unordered_map<std::string, StructBlock *> types;
    std::unordered_map<std::string, StaticBlock *> namespaces;
    std::vector<SyntaxNode *> ast;
public:
    explicit Translator(std::vector<SyntaxNode *> ast);

    void translate();

    void buildPrototypes();

    void buildStruct(StructBlock *block, SyntaxNode *node);

    void buildStatic();

    StructFunction getFunctionDeclaration(SyntaxNode *node);
    StaticFunction getFunctionDeclaration2(SyntaxNode *node);

    std::pair<StructBlock *, bool> getStructVar(SyntaxNode *type);

    void makeNumber(Register *dest, int64_t number);

    void makeAdd(Register *dest, Register *a, Register *b);

    void makeMove(Register *a, Register *b);

    void makeLoadConst(Register *dest, uint64_t index);

    void makeCall(Register *function);

    void *makeEndOfCode();

};


#endif //DATA_PROCESSING_INTERMEDIATE_TRANSLATOR_H
