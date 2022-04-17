//
// Created by Mo on 17.04.2022.
//

#include <stack>
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
    snapshot->registers = static_cast<Register *>(malloc(sizeof(Register) * MAX_REGISTERS));
    for (uint64_t i = 0; i < MAX_REGISTERS; ++i) {
        snapshot->registers[i] = Register{i};
    }
    if (!stack.empty()) {
        sync(snapshot);
    }
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

void translateBoolExpression(SyntaxNode *ast) {
    push();


    pop();
}

void translateExpression(SyntaxNode *ast) {
    push();
    TYPE(BOOL_EXPRESSION) {
        DOWN(BOOL_EXPRESSION);
        DOWN(COMPARISON_EXPRESSION);
        DOWN(ARITHMETIC);
        DOWN(TERM);
        TYPE(A_MULTIPLY) {

        }
    }

    pop();
}

void translateStatement(SyntaxNode *ast) {
    push();

    TYPE(SINGLE_STATEMENT) {
        DOWN(SINGLE_STATEMENT);
        TYPE(VAR_DEFINITION) {
            DOWN(VAR_DEFINITION);
            DOWN(EXPRESSION);
            translateExpression(ast);
        }
    }

    pop();
}


void Translator::translate(SyntaxNode *ast) {
    push();
    Register *a = useNew();
    Register *b = useNew();
    move(a, b);

    pop();


    closeStack();
}