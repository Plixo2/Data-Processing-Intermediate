//
// Created by Mo on 19.04.2022.
//

#ifndef DATA_PROCESSING_INTERMEDIATE_DPI_TYPES_H
#define DATA_PROCESSING_INTERMEDIATE_DPI_TYPES_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include "DPI_Syntax.h"


namespace types {
    struct _objTable;
    struct _funcProto;
    struct _prototype;
    struct _statement;

    typedef union {
        double as_float;
        uint64_t as_int;
        _objTable *as_object;
        _funcProto *as_function;
        _prototype *as_prototype;
    } Value;

    typedef struct {
        uint16_t type;
        Value value;
    } Object;

    typedef struct {
        uint8_t op;
        uint8_t A;
        uint8_t B;
        uint8_t C;
    } OpCode;

    typedef union {
        OpCode code;
        int32_t as_int;
    } Instruction;

    typedef struct _funcProto {
        uint8_t registers;
        uint32_t instruction_location;
    } FunctionPrototype;

    typedef struct _prototype {
        uint16_t type;
        uint16_t alloc_size;
//        FunctionPrototype *constructors;
    } ObjectPrototype;

    typedef struct _objTable {
        Object *members;
        ObjectPrototype *prototype;

        ~_objTable() {
            delete members;
        }
    } ObjectTable;


    typedef struct {
        uint8_t type;
        Value value;
    } Constant;

#define CONST_INT 1
#define CONST_FLOAT 2
#define CONST_OBJECT 3
#define CONST_FUNCTION 4
#define CONST_OBJECT_PROTO 5


    typedef struct {
        uint8_t index;
        bool isUsed;
    } Register;

    typedef struct _snapshot {
        Register *registers;

        ~_snapshot() {
            delete registers;
        }
    } Snapshot;

    typedef struct _var {
        std::string name;
        std::string type;
        uint32_t level;
    } Var;

    struct _structBlock;

    typedef struct {
        std::string name;
        bool isArray;
        _structBlock *type;
    } StructVar;

    typedef struct {
        bool isArray;
        _structBlock *type;
    } StructIOVar;

    typedef struct {
        std::string name;
        std::vector<StructIOVar> input;
        StructIOVar output;
    } StructFunction;

    typedef struct _structBlock {
        std::string name;
        std::vector<StructVar> variables;
        std::vector<StructFunction> functions;
    } StructBlock;

    typedef struct {
        std::string name;
        std::vector<StructVar> input;
        StructIOVar output;
        SyntaxNode *statements;
        _statement *preProcessed;
    } StaticFunction;

    typedef struct {
        std::string name;
        std::vector<StaticFunction> functions;
    } StaticBlock;

/*    typedef struct {
        StaticFunction *function;
    } TopLevelScope;*/

    typedef struct {
        std::vector<StructVar> variables;
        std::vector<_statement> statements;
    } Scope;

    typedef struct {
        StructVar typeAndName;
        SyntaxNode *expression;
    } Declaration;

    typedef struct _block {
        std::vector<_statement*> statements;
    } Block;

    typedef struct {
        SyntaxNode *action;
    } Action;

    typedef struct {
       SyntaxNode * member;
        SyntaxNode *value;
    } Assignment;

    typedef struct {
        SyntaxNode *condition;
        _statement *content;
        _statement *elseContent = nullptr;
    } Branch;


#define DECLARATION_STATEMENT 1
#define ASSIGNMENT_STATEMENT 2
#define BRANCH_STATEMENT 3
#define ACTION_STATEMENT 4
#define BLOCK_STATEMENT_ 5
#define EMPTY_STATEMENT_ 6

    typedef union _statementObj {
        Block *block;
        Branch *branch;
        Action *action;
        Declaration *declaration;
        Assignment *assignment;
    } StatementObj;


    typedef struct _statement {
        uint8_t type;
        StatementObj object;

        ~_statement() {
            std::cout << "statement out of scope" << std::endl;
            if (type == DECLARATION_STATEMENT) {
                delete object.declaration;
            } else if (type == ASSIGNMENT_STATEMENT) {
                delete object.assignment;
            } else if (type == BRANCH_STATEMENT) {
                delete object.branch->elseContent;
                delete object.branch->content;
                delete object.branch;
            } else if (type == ACTION_STATEMENT) {
                delete object.action;
            } else if (type == BLOCK_STATEMENT_) {
                for (auto *statement : object.block->statements) {
                    delete statement;
                }
                delete object.block;
            }
        }
    } Statement;

    typedef struct {
        StructBlock *type;
        bool isArray;
        uint8_t _register;
        std::string name;
    } FramedVariable;
}
namespace std {
    template<>
    struct hash<types::Var> {
        size_t operator()(const types::Var &k) {
            return std::hash<std::string>{}(k.name);
        }
    };
}
namespace types {
    typedef struct {
        std::unordered_map<Var, uint8_t> variables;
    } FunctionScope;
}


#endif //DATA_PROCESSING_INTERMEDIATE_DPI_TYPES_H
