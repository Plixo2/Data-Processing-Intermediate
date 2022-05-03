//
// Created by Mo on 17.04.2022.
//

#ifndef DATA_PROCESSING_INTERMEDIATE_TRANSLATOR_H
#define DATA_PROCESSING_INTERMEDIATE_TRANSLATOR_H

#include "LexerC.h"
#include "DPI_Types.h"

using namespace types;
typedef std::unordered_map<std::string, FramedVariable>* VarMap;
class Translator {
private:
    std::vector<Constant> constants;
    std::vector<Instruction> instructions;
    std::vector<FunctionPrototype> functions;
    std::unordered_map<std::string, StructBlock *> types;
    std::vector<SyntaxNode *> asts;
public:
    std::unordered_map<std::string, StaticBlock *> namespaces;

    explicit Translator(std::vector<SyntaxNode *> ast);

    void translate();

    void buildPrototypes();

    void buildStruct(StructBlock *block, SyntaxNode *node);

    void buildStatic();

    uint8_t expr(uint8_t _register, VarMap varRegisters , SyntaxNode *expression);
    uint8_t bool_expr(uint8_t _register, VarMap varRegisters , SyntaxNode *expression);
    uint8_t comp_expr(uint8_t _register, VarMap varRegisters , SyntaxNode *expression);
    uint8_t arithmetic(uint8_t _register, VarMap varRegisters , SyntaxNode *expression);
    uint8_t term(uint8_t _register, VarMap varRegisters , SyntaxNode *expression);
    uint8_t factor(uint8_t _register, VarMap varRegisters , SyntaxNode *expression);

    void
    translateDeclaration(Declaration &statement, VarMap parentRegisters);

    void
    translateAssignment(Assignment &statement, VarMap parentRegisters);

    void
    translateStatement(Statement &statement, VarMap parentRegisters);

    void translateBlock(Block &block, VarMap parentRegisters);

    void translateStaticBlocks(StaticBlock *block);

    Block *buildBlock(SyntaxNode *ast);

    Branch *buildBranch(SyntaxNode *ast);

    Assignment *buildAssignment(SyntaxNode *ast);

    Action *buildAction(SyntaxNode *ast);

    Declaration *buildDeclaration(SyntaxNode *ast);

    Statement *buildStatement(SyntaxNode *ast);

    std::vector<Statement *> buildFromList(std::vector<SyntaxNode *> statements);


    StructFunction getFunctionDeclaration(SyntaxNode *node);

    StaticFunction getNamedFunctionDeclaration(SyntaxNode *node);

    std::pair<StructBlock *, bool> getStructVar(SyntaxNode *type);

    void makeNumber(Register *dest, int64_t number);

    void makeAdd(Register *dest, Register *a, Register *b);

    void makeMove(Register *a, Register *b);

    void makeLoadConst(Register *dest, uint64_t index);

    void makeCall(Register *function);

    void *makeEndOfCode();

};


#endif //DATA_PROCESSING_INTERMEDIATE_TRANSLATOR_H
