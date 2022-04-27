//
// Created by Mo on 11.04.2022.
//

#include <stack>
#include <iostream>
#include "DPI_Syntax.h"
#include "LexerC.h"

#define MATCH(_type) if(token_stream->hasEntriesLeft() && token_stream->current().type == (_type).type)
#define TYPEOF(_type) (token_stream->hasEntriesLeft() && token_stream->current().type == (_type).type)
#define NEXT token_stream->consume()
#define REMEMBER(type) std::string (type) = token_stream->current().raw_string
#define FINISH_BI(a0, a1) SyntaxNode *_returnValue = createBiNode( blockStack.top(), a0, a1); blockStack.pop(); return _returnValue
#define FINISH(a0) SyntaxNode *_returnValue = createNode( blockStack.top(), a0); blockStack.pop(); return _returnValue
#define FINISH_LEAF(_literal) SyntaxNode *_returnValue =  createLeaf(blockStack.top(),_literal);  blockStack.pop(); return _returnValue
#define BEGIN(_type) blockStack.push(LexNode::_type)
#define THEN(_node) SyntaxNode *_node = this->_node()

#define TRACE(excptstr)  excptstr += ":\n"; \
int n = blockStack.size();                                               \
for (int i = 0; i < n; ++i) { \
excptstr += LexNode::NAMES[blockStack.top()]; \
excptstr += " \n"; \
blockStack.pop(); \
}

#define END std::string excptstr = "\nAssertion fail. found "; \
excptstr += std::to_string(token_stream->current().type);\
excptstr += " in block ";\
excptstr += LexNode::NAMES[blockStack.top()];                \
TRACE(excptstr)               \
throw AssertionException(&excptstr)

#define ASSERT(_type) if((_type).type != token_stream->current().type) { \
    std::string excptstr = "\nAssertion fail. found "; \
    excptstr += std::to_string(token_stream->current().type);                              \
    excptstr += ", but expected ";\
    excptstr += std::to_string(_type.type);                              \
    excptstr += " in block ";\
    excptstr += LexNode::NAMES[blockStack.top()];                        \
    TRACE(excptstr)                                                                     \
    throw AssertionException(&excptstr);       \
} else {  NEXT; }


std::stack<uint8_t> blockStack;

SyntaxNode *createLeaf(uint8_t lex_type, std::string literal) {


    auto *node = new SyntaxNode{lex_type, std::move(literal)};
    return node;
}

SyntaxNode *empty(uint8_t lex_type) {
    auto *node = new SyntaxNode{lex_type, ""};
    return node;
}

SyntaxNode *createNode(uint8_t lex_type, SyntaxNode *a0) {
    auto *node = new SyntaxNode{lex_type, "", a0};
    return node;
}

SyntaxNode *createBiNode(uint8_t lex_type, SyntaxNode *a0, SyntaxNode *a1) {
    auto *node = new SyntaxNode{lex_type, "", a0, a1};
    return node;
}

LexerC::LexerC(IterableStream *token_stream) : token_stream(token_stream) {

}


std::vector<SyntaxNode *> LexerC::entry() {
    std::vector<SyntaxNode *> list;

    while (token_stream->hasEntriesLeft()) {
        list.push_back(topLevel());
    }
    return list;
}


SyntaxNode *LexerC::topLevel() {
    BEGIN(TOP);
    MATCH(Syntax::STATIC) {
        NEXT;
        THEN(staticBlock);
        FINISH(staticBlock);
    } else MATCH(Syntax::STRUCT) {
        NEXT;
        THEN(structBlock);
        FINISH(structBlock);
    }
    END;
}


SyntaxNode *LexerC::staticBlock() {
    BEGIN(STATIC_BLOCK);
    THEN(idDef);
    ASSERT(Syntax::BRACES_OPEN);
    THEN(staticList);
    ASSERT(Syntax::BRACES_CLOSED);
    FINISH_BI(idDef, staticList);
    END;
}

SyntaxNode *LexerC::staticList() {
    BEGIN(STATIC_LIST);

    THEN(staticFunction);
    MATCH(Syntax::BRACES_CLOSED) {
        FINISH(staticFunction);
    }
    THEN(staticList);
    FINISH_BI(staticFunction, staticList);

    END;
}

SyntaxNode *LexerC::staticFunction() {
    BEGIN(STATIC_FUNCTION);

    THEN(functionDef);
    THEN(statement);
    FINISH_BI(functionDef, statement);

    END;
}


SyntaxNode *LexerC::structBlock() {
    BEGIN(STRUCT_BLOCK);
    THEN(idDef);
    ASSERT(Syntax::BRACES_OPEN);
    THEN(definitionsList);
    ASSERT(Syntax::BRACES_CLOSED);
    FINISH_BI(idDef, definitionsList);
    END;
}

SyntaxNode *LexerC::statement() {
    BEGIN(STATEMENT);
    MATCH(Syntax::BRACES_OPEN) {
        THEN(blockStatement);
        FINISH(blockStatement);
    } else if (TYPEOF(Syntax::FOR) || TYPEOF(Syntax::IF)) {
        THEN(flowStatement);
        MATCH(Syntax::END_OF_STATEMENT) {
            NEXT;
        }
        FINISH(flowStatement);
    } else MATCH(Syntax::KEYWORD) {
        THEN(normalStatement);
        MATCH(Syntax::END_OF_STATEMENT) {
            NEXT;
        }
        FINISH(normalStatement);
    } else MATCH(Syntax::BRACES_CLOSED) {
        SyntaxNode *node = empty(LexNode::EMPTY_STATEMENT);
        MATCH(Syntax::END_OF_STATEMENT) {
            NEXT;
        }
        FINISH(node);
    }
    END;
}

SyntaxNode *LexerC::blockStatement() {
    BEGIN(BLOCK_STATEMENT);

    ASSERT(Syntax::BRACES_OPEN);
    THEN(blockStatementList);
    FINISH(blockStatementList);

    END;
}


SyntaxNode *LexerC::blockStatementList() {
    BEGIN(STATEMENT_LIST);


    THEN(statement);
    MATCH(Syntax::BRACES_CLOSED) {
        NEXT;
        FINISH(statement);
    }
    THEN(blockStatementList);
    FINISH_BI(statement, blockStatementList);


    END;
}

SyntaxNode *LexerC::flowStatement() {
    BEGIN(FLOW_STATEMENT);

    MATCH(Syntax::IF) {
        THEN(_if);
        FINISH(_if);
    } else MATCH(Syntax::FOR) {
        THEN(_for);
        FINISH(_for);
    }

    END;
}

SyntaxNode *LexerC::normalStatement() {
    BEGIN(SINGLE_STATEMENT);

    THEN(idDef);
    MATCH(Syntax::KEYWORD) {
        SyntaxNode *type = this->typeDef();
        SyntaxNode *typeAndID = createBiNode(LexNode::TYPE_AND_ID, idDef, type);
        ASSERT(Syntax::ASSIGN);
        THEN(expression);
        SyntaxNode *create = createBiNode(LexNode::VAR_DEFINITION, typeAndID, expression);
        FINISH(create);
    } else {
        THEN(member);
        SyntaxNode *memberStart = createBiNode(LexNode::MEMBER_START, idDef, member);
        MATCH(Syntax::ASSIGN) {
            NEXT;
            THEN(expression);
            SyntaxNode *node = createBiNode(LexNode::VAR_ASSIGNMENT, memberStart, expression);
            FINISH(node);
        }
        SyntaxNode *action = createNode(LexNode::VAR_ACTION, memberStart);
        FINISH(action);
    }
    END;
}

SyntaxNode *LexerC::varDefinition() {
    BEGIN(VAR_DEFINITION);

    THEN(typeDef);
    THEN(idDef);
    SyntaxNode *typeAndID = createBiNode(LexNode::TYPE_AND_ID, typeDef, idDef);
    ASSERT(Syntax::ASSIGN);
    THEN(expression);
    FINISH_BI(expression, typeAndID);

    END;
}


SyntaxNode *LexerC::varAssignment() {
    BEGIN(VAR_ASSIGNMENT);


    END;
}

SyntaxNode *LexerC::varCall() {
    BEGIN(VAR_CALL);


    END;
}

SyntaxNode *LexerC::_if() {
    BEGIN(IF_STATEMENT);
    ASSERT(Syntax::IF);
    THEN(expression);
    THEN(statement);

    SyntaxNode *body = createBiNode(LexNode::IF_BODY_AND_CONDITION, expression, statement);
    MATCH(Syntax::ELSE) {
        NEXT;
        SyntaxNode *elseBody = createNode(LexNode::ELSE_BODY, this->statement());
        FINISH_BI(body, elseBody);
    }
    FINISH(body);

    END;
}

SyntaxNode *LexerC::_for() {
    return nullptr;
}

SyntaxNode *LexerC::definitionsList() {
    BEGIN(DEFINITIONS_LIST);
    MATCH(Syntax::BRACES_CLOSED) {
        FINISH(empty(LexNode::DEFINITIONS_LIST));
    }
    THEN(definitions);
    MATCH(Syntax::BRACES_CLOSED) {
        FINISH(definitions);
    }
    THEN(definitionsList);
    FINISH_BI(definitions, definitionsList);

    END;
}

SyntaxNode *LexerC::definitions() {
    BEGIN(DEFINITION);

    MATCH(Syntax::FN) {
        THEN(functionDefShort);
        FINISH(functionDefShort);
    }
    THEN(varDefinitionShort);
    FINISH(varDefinitionShort);
    END;
}

SyntaxNode *LexerC::varDefinitionShort() {
    BEGIN(VAR_DEFINITION_SHORT);

    THEN(typeDef);
    THEN(idDef);
    SyntaxNode *typeAndID = createBiNode(LexNode::TYPE_AND_ID, typeDef, idDef);
    FINISH(typeAndID);

    END;
}


SyntaxNode *LexerC::outDefinitions() {
    BEGIN(OUTPUT_DEFINITION);

    THEN(typeDef);
    FINISH(typeDef);

    END;
}

SyntaxNode *LexerC::inputDefinitions() {
    BEGIN(INPUT_DEFINITIONS);

    ASSERT(Syntax::PARENTHESES_OPEN)
    MATCH(Syntax::PARENTHESES_CLOSED) {
        NEXT;
        FINISH(empty(LexNode::EMPTY_INPUT));
    }
    THEN(inputDefs);
    ASSERT(Syntax::PARENTHESES_CLOSED)
    FINISH(inputDefs);

    END;
}

SyntaxNode *LexerC::inputDefs() {
    BEGIN(INPUT_LIST);

    THEN(typeDef);
    THEN(idDef);

    MATCH(Syntax::SEPARATOR) {
        NEXT;
        SyntaxNode *typeAndID = createBiNode(LexNode::TYPE_AND_ID, typeDef, idDef);
        THEN(inputDefs);
        FINISH_BI(typeAndID, inputDefs);
    }
    SyntaxNode *typeAndID = createBiNode(LexNode::TYPE_AND_ID, typeDef, idDef);
    FINISH(typeAndID);

    END;
}

SyntaxNode *LexerC::functionDef() {
    BEGIN(FUNCTION_DECLARATION);

    ASSERT(Syntax::FN);
    THEN(idDef);
    THEN(inputDefinitions);
    ASSERT(Syntax::ARROW);
    THEN(outDefinitions);
    SyntaxNode *io = createBiNode(LexNode::INPUT_AND_OUTPUT, inputDefinitions, outDefinitions);
    FINISH_BI(idDef, io);

    END;
}

SyntaxNode *LexerC::typeDef() {
    BEGIN(TYPE);

    REMEMBER(key);
    NEXT;

    std::string type = "object";
    MATCH(Syntax::BRACKET_OPEN) {
        NEXT;
        ASSERT(Syntax::BRACKET_CLOSED);
        type = "array";
    }
    SyntaxNode *typeID = createLeaf(LexNode::TYPE_IDENTIFIER, key);
    SyntaxNode *typeType = createLeaf(LexNode::TYPE_TYPE, type);
    FINISH_BI(typeID, typeType);

    END;
}

SyntaxNode *LexerC::idDef() {
    BEGIN(IDENTIFIER);
    REMEMBER(literal);
    ASSERT(Syntax::KEYWORD);
    FINISH_LEAF(literal);
    END;
}

SyntaxNode *LexerC::number() {
    BEGIN(NUMBER);
    REMEMBER(literal);
    ASSERT(Syntax::NUMBER);
    FINISH_LEAF(literal);
    END;
}

SyntaxNode *LexerC::argList() {
    BEGIN(CALL_ARGUMENTS);

    MATCH(Syntax::PARENTHESES_CLOSED) {
        FINISH(empty(LexNode::CALL_ARGUMENTS));
    }
    THEN(expression);
    MATCH(Syntax::SEPARATOR) {
        THEN(argList);
        FINISH_BI(expression, argList);
    }
    FINISH(expression);

    END;
}

SyntaxNode *LexerC::memberStart() {
    BEGIN(MEMBER_START);


    THEN(idDef);
    if (TYPEOF(Syntax::DOT) || TYPEOF(Syntax::PARENTHESES_OPEN) || TYPEOF(Syntax::BRACKET_OPEN)) {
        THEN(member);
        FINISH_BI(idDef, member);
    } else {
        FINISH(idDef);
    }

    END;
}

SyntaxNode *LexerC::member() {
    BEGIN(MEMBER);

    THEN(varTerminal);
    if (TYPEOF(Syntax::DOT) || TYPEOF(Syntax::PARENTHESES_OPEN) || TYPEOF(Syntax::BRACKET_OPEN)) {
        THEN(member);
        FINISH_BI(varTerminal, member);
    }
    FINISH(varTerminal);
    END;
}

SyntaxNode *LexerC::varTerminal() {
    BEGIN(VAR_TERMINAL);
    MATCH(Syntax::DOT) {
        NEXT;
        THEN(idDef);
        FINISH(createNode(LexNode::MEMBER_ACCESS, idDef));
    } else MATCH(Syntax::PARENTHESES_OPEN) {
        NEXT;
        THEN(argList);
        ASSERT(Syntax::PARENTHESES_CLOSED);
        FINISH(createNode(LexNode::FUNCTION_ACCESS, argList));
    } else MATCH(Syntax::BRACKET_OPEN) {
        NEXT;
        THEN(expression);
        ASSERT(Syntax::BRACKET_CLOSED);
        FINISH(createNode(LexNode::ARRAY_ACCESS, expression));
    } else {
        SyntaxNode *_returnValue = empty(blockStack.top());
        blockStack.pop();
        return _returnValue;
    }

    END;
}

SyntaxNode *LexerC::arrayInitializer() {
    BEGIN(ARRAY_INIT);

    ASSERT(Syntax::BRACKET_OPEN);
    THEN(argList);
    ASSERT(Syntax::BRACKET_CLOSED);
    FINISH(argList);


    END;
}

SyntaxNode *LexerC::expression() {
    BEGIN(EXPRESSION);

    THEN(boolArithmetic);
    FINISH(boolArithmetic);

    END;
}

SyntaxNode *LexerC::boolArithmetic() {
    BEGIN(BOOL_EXPRESSION);

    THEN(comparisonArithmetic);
    if (TYPEOF(Syntax::B_AND)) {
        NEXT;
        THEN(boolArithmetic);
        FINISH_BI(comparisonArithmetic, boolArithmetic);
    }
    FINISH(comparisonArithmetic);

    END;
}

SyntaxNode *LexerC::comparisonArithmetic() {
    BEGIN(COMPARISON_EXPRESSION);

    THEN(arithmetic);
    if (TYPEOF(Syntax::SMALLER_EQUAL)) {
        NEXT;
        THEN(comparisonArithmetic);
        FINISH_BI(arithmetic, comparisonArithmetic);
    }
    FINISH(arithmetic);

    END;
}

SyntaxNode *LexerC::arithmetic() {
    BEGIN(ARITHMETIC);

    THEN(term);
    if (TYPEOF(Syntax::A_PLUS)) {
        NEXT;
        THEN(arithmetic);
        FINISH_BI(term, arithmetic);
    }
    FINISH(term);

    END;
}

SyntaxNode *LexerC::term() {
    BEGIN(TERM);

    SyntaxNode *left = factor();
    while (TYPEOF(Syntax::A_MULTIPLY)) {
        NEXT;
        SyntaxNode *right = factor();
        left = createBiNode(LexNode::A_MULTIPLY, left, right);
    }
    FINISH(left);

    END;
}

SyntaxNode *LexerC::factor() {
    BEGIN(FACTOR);


    MATCH(Syntax::PARENTHESES_OPEN) {
        NEXT;
        THEN(expression);
        ASSERT(Syntax::PARENTHESES_CLOSED);
        FINISH(expression);
    } else MATCH(Syntax::A_MINUS) {
        NEXT;
        THEN(factor);
        SyntaxNode *node = createNode(LexNode::UNARY, factor);
        FINISH(node);
    } else MATCH(Syntax::A_PLUS) {
        NEXT;
        THEN(factor);
        FINISH(factor);
    } else MATCH(Syntax::B_NOT) {
        NEXT;
        THEN(factor);
        SyntaxNode *node = createNode(LexNode::B_NOT, factor);
        FINISH(node);
    } else MATCH(Syntax::NUMBER) {
        THEN(number);
        FINISH(number);
    } else MATCH(Syntax::FALSE) {
        NEXT;
        SyntaxNode *node = createLeaf(LexNode::NUMBER, "0");
        FINISH(node);
    } else MATCH(Syntax::TRUE) {
        NEXT;
        SyntaxNode *node = createLeaf(LexNode::NUMBER, "1");
        FINISH(node);
    } else MATCH(Syntax::BRACKET_OPEN) {
        THEN(arrayInitializer);
        FINISH(arrayInitializer);
    } else MATCH(Syntax::KEYWORD) {
        THEN(memberStart);
        FINISH(memberStart);
    }

    END;
}


SyntaxNode *LexerC::inputDefinitionsShort() {
    BEGIN(INPUT_DEFINITIONS);

    ASSERT(Syntax::PARENTHESES_OPEN)
    THEN(inputDefsShort);
    ASSERT(Syntax::PARENTHESES_CLOSED)
    FINISH(inputDefsShort);

    END;
}

SyntaxNode *LexerC::inputDefsShort() {
    BEGIN(INPUT_LIST);

    THEN(typeDef);

    MATCH(Syntax::SEPARATOR) {
        NEXT;
        SyntaxNode *typeAndID = createNode(LexNode::TYPE_AND_ID, typeDef);
        THEN(inputDefsShort);
        FINISH_BI(typeAndID, inputDefsShort);
    }
    SyntaxNode *typeAndID = createNode(LexNode::TYPE_AND_ID, typeDef);
    FINISH(typeAndID);

    END;
}

SyntaxNode *LexerC::functionDefShort() {
    BEGIN(FUNCTION_DECLARATION);

    ASSERT(Syntax::FN);
    THEN(idDef);
    THEN(inputDefinitionsShort);
    ASSERT(Syntax::ARROW);
    THEN(outDefinitions);
    SyntaxNode *io = createBiNode(LexNode::INPUT_AND_OUTPUT, inputDefinitionsShort, outDefinitions);
    FINISH_BI(idDef, io);

    END;
}


SyntaxNode *SyntaxNode::find(uint8_t type) {
    if (left && left->lex_type == type) {
        return left;
    } else if (right && right->lex_type == type) {
        return right;
    }
    return nullptr;
}

SyntaxNode *SyntaxNode::assert(uint8_t type) {
    if (left && left->lex_type == type) {
        return left;
    } else if (right && right->lex_type == type) {
        return right;
    }
    std::string excptstr = "cant find " + LexNode::NAMES[type];
    throw AssertionException(&excptstr);
}

SyntaxNode *SyntaxNode::any() {
    if (left) {
        return left;
    } else if (right) {
        return right;
    }
    return nullptr;
}

SyntaxNode *SyntaxNode::assert() {
    if (left) {
        return left;
    } else if (right) {
        return right;
    }
    std::string excptstr = "cant resolve any type";
    throw AssertionException(&excptstr);
}
