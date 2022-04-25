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


void buildFlatVector(std::vector<SyntaxNode *> &collection, SyntaxNode *current, uint8_t list, uint8_t node) {
    auto *next = current->find(list);
    auto *leaf = current->find(node);
    if (leaf) {
        collection.push_back(leaf);
    }
    if (next) {
        buildFlatVector(collection, next, list, node);
    }
}

#define ASSERT_NODE(_type) if(!(_type)) {std::string msg = "a note could not be found"; throw TranslatorAssertionException(&msg); }

void Translator::buildStruct(StructBlock *block, SyntaxNode *node) {
    auto *list = node->find(LexNode::DEFINITIONS_LIST);
    if (list) {
        std::vector<SyntaxNode *> collection;
        buildFlatVector(collection, list, LexNode::DEFINITIONS_LIST, LexNode::DEFINITION);
        for (SyntaxNode *item: collection) {
            auto *def = item->assert();
            if (def->lex_type == LexNode::VAR_DEFINITION_SHORT) {
                auto *typeAndID = def->assert();
                auto *type = typeAndID->assert(LexNode::TYPE);
                auto *type_id = type->assert(LexNode::TYPE_IDENTIFIER);
                auto *type_obj = type->assert(LexNode::TYPE_TYPE);
                auto *name = typeAndID->assert(LexNode::IDENTIFIER);

                StructBlock *typeObj = types[type_id->data];
                if (!typeObj) {
                    std::string exp = "cant find type " + type_id->data;
                    throw TranslatorAssertionException(&exp);
                }
                bool type_of_obj;
                if (type_obj->data == "object") {
                    type_of_obj = false;
                } else if (type_obj->data == "array") {
                    type_of_obj = true;
                } else {
                    std::string exp = "cant resolve its an array or not " + type_id->data;
                    throw TranslatorAssertionException(&exp);
                }
                StructVar var = {name->data, type_of_obj, typeObj};
                block->variables.push_back(var);
            }
        }
    }
}

void Translator::buildPrototypes() {
    std::vector<std::pair<StructBlock *, SyntaxNode *>> structs;
    for (SyntaxNode *item: ast) {
        SyntaxNode *type = item->any();
        ASSERT_NODE(type);
        if (type->lex_type == LexNode::STRUCT_BLOCK) {
            SyntaxNode *id = type->find(LexNode::IDENTIFIER);
            ASSERT_NODE(id);
            std::string name = id->data;
            if (types.contains(name)) {
                std::string msg = name + " was already defined";
                throw TranslatorAssertionException(&msg);
            } else {
                auto *structBlock = new StructBlock{name};
                types[name] = structBlock;
                structs.emplace_back(structBlock, type);
                /*for(std::pair<std::string, StructBlock> s : types) {

                }*/
            }

        }
    }
    std::cout << structs.size() << std::endl;
    for (auto &item: structs) {
        std::cout << "Struct name: " << item.first->name << std::endl;
        buildStruct(item.first, item.second);
    }

}



void Translator::translate() {
    auto *buildInInt = new StructBlock{"int"};
    types["int"] = buildInInt;
    auto *BuildInDouble = new StructBlock{"decimal"};
    types["decimal"] = BuildInDouble;
    buildPrototypes();
}

Translator::Translator(std::vector<SyntaxNode *> ast) {
    this->ast = std::move(ast);
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
