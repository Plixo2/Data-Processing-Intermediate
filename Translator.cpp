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
                const std::pair<StructBlock *, bool> &pair = getStructVar(typeAndID->assert(LexNode::TYPE));
                auto *name = typeAndID->assert(LexNode::IDENTIFIER);

                StructVar var = {name->data, pair.second, pair.first};
                block->variables.push_back(var);
            } else if (def->lex_type == LexNode::FUNCTION_DECLARATION) {
                StructFunction func = getFunctionDeclaration(def);
                block->functions.push_back(func);
            }
        }
    }
}

std::pair<StructBlock *, bool> Translator::getStructVar(SyntaxNode *type) {
    auto *type_id = type->assert(LexNode::TYPE_IDENTIFIER);
    auto *type_obj = type->assert(LexNode::TYPE_TYPE);
    StructBlock *typeObj = types[type_id->data];
    if (!typeObj) {
        std::string exp = "cant find type " + type_id->data;
        throw TranslatorAssertionException(&exp);
    }
    bool isArray;
    if (type_obj->data == "object") {
        isArray = false;
    } else if (type_obj->data == "array") {
        isArray = true;
    } else {
        std::string exp = "cant resolve its an array or not " + type_id->data;
        throw TranslatorAssertionException(&exp);
    }
    return {typeObj, isArray};
}

void Translator::buildPrototypes() {
    std::vector<std::pair<StructBlock *, SyntaxNode *>> structs;
    for (SyntaxNode *item: asts) {
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
            }

        } else if (type->lex_type == LexNode::STATIC_BLOCK) {
            auto *name = type->assert(LexNode::IDENTIFIER);
            std::string block_name = name->data;
            std::vector<SyntaxNode *> funcs;
            buildFlatVector(funcs, type, LexNode::STATIC_LIST, LexNode::STATIC_FUNCTION);
            auto *block = new StaticBlock{block_name};
            namespaces[block_name] = block;
            for (const auto &function: funcs) {
                StaticFunction func = getNamedFunctionDeclaration(function);
                block->functions.push_back(func);
            }
        }
    }
    for (auto &item: structs) {
        buildStruct(item.first, item.second);
    }

}


void Translator::translate() {
    auto *buildInInt = new StructBlock{"int"};
    types["int"] = buildInInt;
    auto *buildInDouble = new StructBlock{"decimal"};
    types["decimal"] = buildInDouble;
    auto *buildVoid = new StructBlock{"void"};
    types["void"] = buildVoid;
    buildPrototypes();
    buildStatic();

    for (const auto &item: namespaces) {
        translateStaticBlocks(item.second);
    }
}


Translator::Translator(std::vector<SyntaxNode *> ast) {
    this->asts = std::move(ast);
}

StructFunction Translator::getFunctionDeclaration(SyntaxNode *def) {
    auto *name = def->assert(LexNode::IDENTIFIER);
    auto *io = def->assert(LexNode::INPUT_AND_OUTPUT);
    auto *out = io->assert(LexNode::OUTPUT_DEFINITION);
    auto *inputList = io->assert(LexNode::INPUT_DEFINITIONS);
    StructFunction func = {name->data};
    if (inputList) {
        std::vector<SyntaxNode *> inputType;
        buildFlatVector(inputType, inputList, LexNode::INPUT_LIST, LexNode::TYPE_AND_ID);
        for (const auto &type_and_id: inputType) {
            const std::pair<StructBlock *, bool> &pair = getStructVar(type_and_id->assert(LexNode::TYPE));
            const StructIOVar ioVar = {pair.second, pair.first};
            func.input.push_back(ioVar);
        }
    }
    const std::pair<StructBlock *, bool> &pair = getStructVar(out->assert(LexNode::TYPE));
    StructIOVar outVar = {pair.second, pair.first};
    func.output = outVar;
    return func;
}

StaticFunction Translator::getNamedFunctionDeclaration(SyntaxNode *ast) {
    auto *node = ast->assert(LexNode::FUNCTION_DECLARATION);
    auto *name = node->assert(LexNode::IDENTIFIER);
    auto *io = node->assert(LexNode::INPUT_AND_OUTPUT);
    auto *out = io->assert(LexNode::OUTPUT_DEFINITION);
    auto *inputList = io->assert(LexNode::INPUT_DEFINITIONS);
    StaticFunction func = {name->data};
    if (inputList) {
        std::vector<SyntaxNode *> inputType;
        buildFlatVector(inputType, inputList, LexNode::INPUT_LIST, LexNode::TYPE_AND_ID);
        for (const auto &type_and_id: inputType) {
            const std::pair<StructBlock *, bool> &pair = getStructVar(type_and_id->assert(LexNode::TYPE));
            const StructVar ioVar = {type_and_id->assert(LexNode::IDENTIFIER)->data, pair.second, pair.first};
            func.input.push_back(ioVar);
        }
    }
    const std::pair<StructBlock *, bool> &pair = getStructVar(out->assert(LexNode::TYPE));
    StructIOVar outVar = {pair.second, pair.first};
    func.output = outVar;
    func.statements = ast->assert(LexNode::STATEMENT);
    return func;
}

void Translator::buildStatic() {
    for (std::pair<std::string, StaticBlock *> block: namespaces) {
        for (StaticFunction &function: block.second->functions) {
            SyntaxNode *node = function.statements->assert();
            Statement *build = buildStatement(node);
            function.preProcessed = build;
        }
    }
}

Statement *Translator::buildStatement(SyntaxNode *ast) {
    auto *node = ast;
    if (node->lex_type == LexNode::STATEMENT) {
        node = node->assert();
    }
    if (node->lex_type == LexNode::EMPTY_STATEMENT) {
        return new Statement{EMPTY_STATEMENT_, {}};
    } else if (node->lex_type == LexNode::BLOCK_STATEMENT) {
        StatementObj obj;
        obj.block = buildBlock(node->assert());
        return new Statement{BLOCK_STATEMENT_, obj};
    } else if (node->lex_type == LexNode::SINGLE_STATEMENT) {
        auto type = node->assert();
        if (type->lex_type == LexNode::VAR_ACTION) {
            StatementObj obj;
            obj.action = buildAction(type);
            return new Statement{ACTION_STATEMENT, obj};
        } else if (type->lex_type == LexNode::VAR_DEFINITION) {
            StatementObj obj;
            obj.declaration = buildDeclaration(type);
            return new Statement{DECLARATION_STATEMENT, obj};
        } else if (type->lex_type == LexNode::VAR_ASSIGNMENT) {
            StatementObj obj;
            obj.assignment = buildAssignment(type);
            return new Statement{ASSIGNMENT_STATEMENT, obj};
        } else {
            throw TranslatorAssertionException("error in single statement building");
        }
    } else if (node->lex_type == LexNode::FLOW_STATEMENT) {
        StatementObj obj;
        obj.branch = buildBranch(node);
        return new Statement{BRANCH_STATEMENT, obj};
    } else {
        std::string msg = "error in statement building: ";
        msg += LexNode::NAMES[node->lex_type];
        throw TranslatorAssertionException(&msg);
    }
}

std::vector<Statement *> Translator::buildFromList(std::vector<SyntaxNode *> statements) {
    std::vector<Statement *> list;
    for (auto &node: statements) {
        list.push_back(buildStatement(node));
    }
    return list;
}

Block *Translator::buildBlock(SyntaxNode *ast) {
    std::vector<SyntaxNode *> flatStatements;
    buildFlatVector(flatStatements, ast, LexNode::STATEMENT_LIST, LexNode::STATEMENT);
    std::vector<Statement *> statements = buildFromList(flatStatements);
    return new Block{statements};
}

Action *Translator::buildAction(SyntaxNode *ast) {
    return new Action{ast->assert(LexNode::MEMBER_START)};
}


Branch *Translator::buildBranch(SyntaxNode *ast) {
    if (ast->find(LexNode::IF_STATEMENT)) {
        SyntaxNode *_if = ast->assert(LexNode::IF_STATEMENT);
        SyntaxNode *body_and_expr = _if->assert(LexNode::IF_BODY_AND_CONDITION);
        Statement *statement = buildStatement(body_and_expr->assert(LexNode::STATEMENT));
        SyntaxNode *_else = _if->find(LexNode::ELSE_BODY);
        Statement *elseContent = nullptr;
        if (_else) {
            elseContent = buildStatement(_else->assert(LexNode::STATEMENT));
        }
        return new Branch{
                body_and_expr->assert(LexNode::EXPRESSION),
                statement,
                elseContent
        };
    }
    throw TranslatorAssertionException("Unknown Branch statement");
}

Assignment *Translator::buildAssignment(SyntaxNode *ast) {
    SyntaxNode *member = ast->assert(LexNode::MEMBER_START);
    return new Assignment{member, ast->assert(LexNode::EXPRESSION)};
}

Declaration *Translator::buildDeclaration(SyntaxNode *ast) {
    SyntaxNode *typeAndID = ast->assert(LexNode::TYPE_AND_ID);
    SyntaxNode *_type = typeAndID->assert(LexNode::TYPE);
    SyntaxNode *type = typeAndID->assert(LexNode::IDENTIFIER);
    SyntaxNode *name = _type->assert(LexNode::TYPE_IDENTIFIER);
    SyntaxNode *type_obj = _type->assert(LexNode::TYPE_TYPE);
    SyntaxNode *expression = ast->assert(LexNode::EXPRESSION);
    StructVar typeAndName;

    StructBlock *typeOf = types[type->data];
    if (typeOf == nullptr) {
        std::string msg = "type " + type->data + " not found";
        throw TranslatorAssertionException(&msg);
    }
    typeAndName.type = typeOf;
    typeAndName.name = name->data;
    bool is_array;
    if (type_obj->data == "object") {
        is_array = false;
    } else if (type_obj->data == "array") {
        is_array = true;
    } else {
        std::string exp = "cant resolve its an array or not " + type_obj->data;
        throw TranslatorAssertionException(&exp);
    }
    typeAndName.isArray = is_array;
    return new Declaration{typeAndName, expression};
}

void Translator::translateDeclaration(Declaration &declaration,
                                      VarMap varRegisters) {

    uint8_t size = varRegisters->size();
    if (varRegisters->contains(declaration.typeAndName.name)) {
        std::string msg = "name collision for \"" + declaration.typeAndName.name + "\"";
        throw TranslatorAssertionException(&msg);
    } else {
        std::cout << "variable " << declaration.typeAndName.name << " in register R-" << (int) size << std::endl;
        varRegisters->operator[](declaration.typeAndName.name) = {
                declaration.typeAndName.type,
                declaration.typeAndName.isArray,
                size,
                declaration.typeAndName.name
        };
    }
    expr(size, varRegisters, declaration.expression);
}


void
Translator::translateStatement(Statement &statement, VarMap parentRegisters) {
    switch (statement.type) {
        case DECLARATION_STATEMENT: {
            translateDeclaration(*statement.object.declaration, parentRegisters);
            break;
        }
        case ASSIGNMENT_STATEMENT: {
            translateAssignment(*statement.object.assignment, parentRegisters);
            break;
        }
        case BRANCH_STATEMENT: {

            break;
        }
        case ACTION_STATEMENT: {

            break;
        }
        case BLOCK_STATEMENT_: {
            translateBlock(*statement.object.block, parentRegisters);
            break;
        }
        case EMPTY_STATEMENT_: {

            break;
        }
        default: {
            std::cout << "unknown type" << std::endl;
        }
    }
}

void Translator::translateBlock(Block &block, std::unordered_map<std::string, FramedVariable> *parentRegisters) {
    std::unordered_map<std::string, FramedVariable> localRegisters;
    std::cout << "pushing variable stack" << std::endl;
    for (const auto &item: *parentRegisters) {
        localRegisters[item.first] = item.second;
    }

    for (Statement *item: block.statements) {
        translateStatement(*item, &localRegisters);
    }
    std::cout << "popping variable stack" << std::endl;

}

void Translator::translateStaticBlocks(StaticBlock *block) {
    std::cout << "-----" << std::endl;
    for (const StaticFunction &func: block->functions) {
        std::unordered_map<std::string, FramedVariable> varRegisters;

        for (const StructVar &input: func.input) {
            if (varRegisters.contains(input.name)) {
                std::string msg = "name collision for \"" + input.name + "\"";
                throw TranslatorAssertionException(&msg);
            } else {
                std::cout << "input " << input.name << " in register R-" << (int) varRegisters.size() << std::endl;
                varRegisters[input.name] = {
                        input.type,
                        input.isArray,
                        (uint8_t) varRegisters.size(),
                        input.name
                };
            }
        }
        translateStatement(*func.preProcessed, &varRegisters);
    }
}

uint8_t Translator::expr(uint8_t _register, VarMap varRegisters, SyntaxNode *expression) {
    if (expression->find(LexNode::BOOL_EXPRESSION)) {
        bool_expr(_register, varRegisters, expression->find(LexNode::BOOL_EXPRESSION));
    } else {
        std::cout << "not yet implemented" << std::endl;
    }
    return _register;
}

uint8_t Translator::bool_expr(uint8_t _register, VarMap varRegisters, SyntaxNode *expression) {
    if (expression->find(LexNode::COMPARISON_EXPRESSION)) {
        comp_expr(_register, varRegisters, expression->find(LexNode::COMPARISON_EXPRESSION));
    } else {
        std::cout << "not yet implemented" << std::endl;
    }
    return _register;
}

uint8_t Translator::comp_expr(uint8_t _register, VarMap varRegisters, SyntaxNode *expression) {
    if (expression->find(LexNode::ARITHMETIC)) {
        arithmetic(_register, varRegisters, expression->find(LexNode::ARITHMETIC));
    } else {
        std::cout << "not yet implemented" << std::endl;
    }
    return _register;
}

uint8_t Translator::arithmetic(uint8_t _register, VarMap varRegisters, SyntaxNode *expression) {
    if (expression->find(LexNode::TERM)) {
        term(_register, varRegisters, expression->find(LexNode::TERM));
    } else {
        std::cout << "not yet implemented" << std::endl;
    }
    return _register;
}

uint8_t Translator::term(uint8_t _register, VarMap varRegisters, SyntaxNode *expression) {
    if (expression->find(LexNode::FACTOR)) {
        factor(_register, varRegisters, expression->find(LexNode::FACTOR));
    } else {
        std::cout << "not yet implemented" << std::endl;
    }
    return _register;
}

uint8_t Translator::factor(uint8_t _register, VarMap varRegisters, SyntaxNode *expression) {
    SyntaxNode *num = expression->find(LexNode::NUMBER);
    SyntaxNode *expr = expression->find(LexNode::EXPRESSION);
    SyntaxNode *keywords = expression->find(LexNode::MEMBER_START);
    SyntaxNode *unary = expression->find(LexNode::UNARY);
    SyntaxNode *_not = expression->find(LexNode::NOT);
    if (num) {
        std::cout << "move number " << num->data << " into R-" << (int) _register << std::endl;
    } else if (expr) {
        return this->expr(_register, varRegisters, expr);
    } else if (keywords) {
        std::string name = keywords->assert(LexNode::IDENTIFIER)->data;
        if (varRegisters->contains(name)) {
            FramedVariable var = varRegisters->operator[](name);
            std::cout << "move R-" << (int) var._register << " into R-" << (int) _register
                      << std::endl;
        } else {
            std::cout << "not yet implemented" << std::endl;
        }

        std::vector<SyntaxNode *> members;
        buildFlatVector(members, keywords, LexNode::MEMBER, LexNode::VAR_TERMINAL);
        for (SyntaxNode *member: members) {
            SyntaxNode *arrayAccess = member->find(LexNode::ARRAY_ACCESS);
            SyntaxNode *funcAccess = member->find(LexNode::FUNCTION_ACCESS);
            if (arrayAccess) {
                SyntaxNode *arrayExpr = arrayAccess->assert(LexNode::EXPRESSION);
                const uint8_t exprRegister = this->expr(_register + 1, varRegisters, arrayExpr);
                std::cout << "array access from R-" << (int) _register << " at position R-" << (int) exprRegister
                          << " into R-" << (int) _register << std::endl;
            } else if (funcAccess) {
                std::vector<SyntaxNode *> arguments;
                buildFlatVector(arguments, funcAccess, LexNode::CALL_ARGUMENTS,
                                LexNode::EXPRESSION);
                uint8_t registers = 0;
                for (SyntaxNode *arg: arguments) {
                    registers += 1;
                    const uint8_t exprRegister = this->expr(_register + registers, varRegisters, arg);
                    std::cout << "Arg moved R-" << (int) exprRegister << " into R-[Max-"
                              << (int) (arguments.size() - registers) << "]" << std::endl;
                }
                std::cout << "calling R-" << (int) _register << std::endl;
            }
        }
        return _register;
    } else if (unary) {
        const uint8_t exprRegister = this->factor(_register, varRegisters, unary->assert(LexNode::kFACTOR));
        std::cout << "move minus of R-" << (int) exprRegister << " into R-" << (int)_register << std::endl;
        return exprRegister;
    } else if (_not) {
        const uint8_t exprRegister = this->factor(_register, varRegisters, _not->assert(LexNode::FACTOR));
        std::cout << "move logic negation of R-" << (int) exprRegister << " into R-" << (int)_register << std::endl;
        return exprRegister;
    }
    return _register;
}

void Translator::translateAssignment(Assignment &statement, VarMap varRegisters) {
    uint8_t size = varRegisters->size();
    expr(size, varRegisters, statement.value);

}







