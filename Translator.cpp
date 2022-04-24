//
// Created by Mo on 17.04.2022.
//

#include <stack>
#include <string>
#include <set>
#include <iostream>
#include <utility>
#include "Translator.h"
#include "DPI_Types.h"

using namespace types;


class TranslatorAssertionException : public std::exception {
    const char *text;

    virtual const char *what() const noexcept {
        return text;
    }

public:
    explicit TranslatorAssertionException(std::string *text) : text(text->c_str()) {}

    explicit TranslatorAssertionException(const char *text) : text(text) {}
};

/*



uint64_t MAX_REGISTERS = 256;


#define SIZEOF_SCOPE (sizeof(Register) * MAX_REGISTERS)

std::stack<Snapshot *> stack;


void use(Register *_register) {
    _register->isUsed = true;
}

Register *useNew() {
    auto *snapshot = stack.top();
    for (uint64_t i = 0; i < MAX_REGISTERS; ++i) {
        if (!snapshot->registers[i].isUsed) {
            use(&snapshot->registers[i]);
            return &snapshot->registers[i];
        }
    }
    return nullptr;
}

void sync(Snapshot *snapshot) {
    Snapshot *top = stack.top();
    for (uint64_t i = 0; i < MAX_REGISTERS; ++i) {
        snapshot->registers[i].isUsed = top->registers[i].isUsed;
    }
}

void push() {
    auto *snapshot = new Snapshot{};
    snapshot->registers = static_cast<Register *>(malloc(SIZEOF_SCOPE));
    for (uint64_t i = 0; i < MAX_REGISTERS; ++i) {
        snapshot->registers[i] = Register{(uint8_t) i};
    }
    sync(snapshot);
    stack.push(snapshot);
}

void pop() {
    delete stack.top();
    stack.pop();
}

void closeStack() {
    uint64_t size = stack.size();
    for (int i = 0; i < size; ++i) {
        delete stack.top();
        stack.pop();
    }
}
*/


SyntaxNode *find(SyntaxNode *item, uint8_t type);

SyntaxNode *any(SyntaxNode *item);

void buildFlatVector(std::vector<SyntaxNode *> &collection, SyntaxNode *current, uint8_t list, uint8_t node) {
    auto *next = find(current, list);
    auto *leaf = find(current, node);
    if (leaf) {
        collection.push_back(leaf);
    }
    if (next) {
        buildFlatVector(collection, next, list, node);
    }
}

void buildStruct(StructBlock *block, SyntaxNode *node) {
    auto *list = find(node, LexNode::DEFINITIONS_LIST);
    if (list) {
        std::vector<SyntaxNode *> collection;
        buildFlatVector(collection, list, LexNode::DEFINITIONS_LIST, LexNode::DEFINITION);
        for (SyntaxNode *item : collection) {
            std::cout << item->lex_type << std::endl;
        }
    }
}

void Translator::buildPrototypes() {
    std::vector<std::pair<StructBlock, SyntaxNode *>> structs;
    for (SyntaxNode *item: ast) {
        SyntaxNode *type = any(item);
        if (type->lex_type == LexNode::STRUCT_BLOCK) {
            SyntaxNode *id = find(type, LexNode::IDENTIFIER);
            std::string name = id->data;
            if (types.contains(name)) {
                std::string msg = name + " was already defined";
                throw TranslatorAssertionException(&msg);
            } else {
                StructBlock structBlock{name};
                types[name] = structBlock;
                structs.push_back({structBlock, type});
                /*for(std::pair<std::string, StructBlock> s : types) {

                }*/
            }
        }
    }
    std::cout << structs.size() << std::endl;
    for (auto &item: structs) {
        std::cout << "Struct name: " << item.first.name << std::endl;
        buildStruct(&item.first, item.second);
    }

}

void Translator::translate() {
    buildPrototypes();

}

Translator::Translator(std::vector<SyntaxNode *> ast) {
    this->ast = std::move(ast);
}

SyntaxNode *any(SyntaxNode *item) {
    if (item->left) {
        return item->left;
    } else if (item->right) {
        return item->right;
    }
    /*
    std::string msg = "could not any ";
    throw TranslatorAssertionException(&msg);
    */
    return nullptr;
}

SyntaxNode *find(SyntaxNode *item, uint8_t type) {
    if (item->left && item->left->lex_type == type) {
        return item->left;
    } else if (item->right && item->right->lex_type == type) {
        return item->right;
    }
    /*
    std::string msg = "could not find ";
    msg += LexNode::NAMES[type];
    throw TranslatorAssertionException(&msg);
    */
    return nullptr;
}

/*
enum VarType {
    FUNCTION,
    ARRAY,
    PURE
};

class Type;

class TypeDef {
    std::string name;
    std::vector<Type *> input;
    Type *output;
};

class Type {
    TypeDef *name;
    bool isArray;
};


class ScopeObject {

};

class Scope {
public:
    Scope *parent = nullptr;
    std::vector<ScopeObject *> object;
};

class VarDeclaration : ScopeObject {
public:
    explicit VarDeclaration(Type *type) : type(type) {

    }

    std::string name;
    Type *type;
    bool isInput = false;
};

class Statement : ScopeObject {
public:
    SyntaxNode *ast;
};

class Branch : ScopeObject {
public:
    Scope *sub;
    SyntaxNode *ast;
};

TypeDef *findFromAst(SyntaxNode *ast) {
    for (typeDef t: types) {
        if (t.name == ast->data) {
            return t;
        }
    }
    throw "error";
}

Type *typeFromAst(SyntaxNode *ast) {
    TypeDef def = findFromAst(ast->right);
    //TODO: make more
}



void translateBranch(SyntaxNode *ast,Branch branch) {

}
void translateFunction(SyntaxNode *ast) {
    auto *scope = new Scope;
    scope->parent = nullptr;
    scope->object.clear();
    for (SyntaxNode *input: ast.input) {
        auto *declaration = new VarDeclaration(typeFromAst(input));
        declaration->isInput = true;
        scope->object.push_back(declaration);

    }
    for (SyntaxNode *instruction: ast.instruction) {
        if (instruction == Var_Declaration) {
            auto *declaration = new VarDeclaration(typeFromAst(instruction));
            decl.name = instruction->data;

            scope->object.push_back(decl);
        } else if (instruction == Statement) {
            auto *statement = new Statement;
            statement->ast = instruction->ast;
            scope->object.push_back(statement);
        } else if (instruction == Branch) {
            auto *branch = new Branch;
            branch->ast = instruction->ast;
            branch->sub = new Scope;
            branch->sub->parent = scope;
            scope->object.push_back(branch);
            translateBranch(instruction->ast , branch);
        }
    }
}
*/
