//
// Created by Mo on 17.04.2022.
//

#include <stack>
#include <unordered_map>
#include <string>
#include "Translator.h"
#include "DPI_Syntax.h"


class TranslatorAssertionException : public std::exception {
    const char *text;

    virtual const char *what() const noexcept {
        return text;
    }

public:
    explicit TranslatorAssertionException(std::string *text) : text(text->c_str()) {}

    explicit TranslatorAssertionException(const char *text) : text(text) {}
};

typedef struct {
    uint64_t index;
    bool isUsed;
} Register;

uint64_t MAX_REGISTERS = 256;
typedef struct _snapshot {
    Register *registers;

    ~_snapshot() {
        delete registers;
    }
} Snapshot;

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
        snapshot->registers[i] = Register{i};
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

void move(Register *dest, Register *loc) {

}

#define TYPE(_type) if(ast->lex_type == (LexNode::_type))

#define GET(_name, _var) if(ast->left->lex_type == LexNode::_name) { \
_var = ast->left;\
} else if(ast->right->lex_type == LexNode::_name) {\
_var = ast->right;\
} else {\
throw TranslatorAssertionException("error lol");\
}

#define DOWN(_type) GET(_type,ast)

typedef struct {
    std::string name;
    std::string type;
} Var;

namespace std {
    template<>
    struct hash<Var> {
        size_t operator()(const Var &k) {
            return std::hash<std::string>{}(k.name);
        }
    };
}


typedef struct {
    std::unordered_map<Var, uint8_t> variables;
} FunctionScope;

std::stack<FunctionScope *> functionStack;

Register *translateFactor(SyntaxNode *ast) {
    if(ast->lex_type == LexNode::NUMBER) {

        return makeNumber(ast->data.);
    }
    return nullptr;
}
Register translateNumber(SyntaxNode *ast) {

    if(ast->data.find('.') != std::string::npos) {

    }
}

Register *makeNumber(int64_t number) {
    auto *ptr = useNew();
    //make instruction
    //add({IRCode::LOAD_CONST_INT,ptr->index});
    //add(number);
    return ptr;
}

Register *makeAdd(Register *a, Register *b) {
    auto *ptr = useNew();
    //make instruction
    //add({IRCode::A_ADD,a->index , b->index,ptr->index});
    return ptr;
}

Register *translateMul(SyntaxNode *ast) {
    push();
    auto *left = translateFactor(ast->left);
    auto *right = translateFactor(ast->right);
    pop();

    auto *add = makeAdd(left, right);
    return add;
}

FunctionScope *makeFunction(std::vector<Var> *variables) {
    auto *scope = new FunctionScope;

    auto *baseScope = new Snapshot{};
    baseScope->registers = static_cast<Register *>(malloc(SIZEOF_SCOPE));
    for (uint64_t i = 0; i < MAX_REGISTERS; ++i) {
        baseScope->registers[i] = Register{i};
    }

    uint64_t size = variables->size();
    for (int i = 0; i < size; ++i) {
        Var &at = variables->at(i);
        scope->variables[at] = i;
        use(&baseScope->registers[i]);
    }

    stack.push(baseScope);
    functionStack.push(scope);

    return scope;
}


void Translator::translate(SyntaxNode *ast) {
    push();
    Register *a = useNew();
    Register *b = useNew();
    move(a, b);

    pop();


    closeStack();
}