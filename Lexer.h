//
// Created by Mo on 24.03.2022.
//

#ifndef DATA_PROCESSING_INTERMEDIATE_LEXER_H
#define DATA_PROCESSING_INTERMEDIATE_LEXER_H

#include "Tokenizer.h"
#include "IterableStream.h"
#include "DPI_Syntax.h"

namespace Lexer {
    class AssertionException : public std::exception {
        const char *text;

        virtual const char *what() const noexcept {
            return text;
        }

    public:
        AssertionException(std::string *text) : text(text->c_str()) {

        }

        AssertionException(const char *text) : text(text) {

        }
    };

    struct Node {
        uint8_t lex_type = 0;
        std::string data;
        std::vector<Node *> childs = std::vector<Node *>();

        ~Node() {
            for (const auto &item: childs) {
                delete item;
            }
        }
    };

#define TOKEN token_stream->current()
#define TYPE token_stream->current().type
#define match(_type) if(token_stream->hasEntriesLeft() && token_stream->current().type == (_type).type)
#define IS(_type) (token_stream->hasEntriesLeft() && token_stream->current().type == (_type).type)
#define NEXT next()
#define REMEMBER(type) std::string (type) = token_stream->current().raw_string
#define expect(str) else { throw (str);}
#define THEN(name) Node *name = this->name()
#define THROW(text) {std::string str= (text); \
    str += std::to_string(TOKEN.type); \
    throw AssertionException(&str);}

#define ASSERT(_type) if((_type).type != token_stream->current().type) { \
    std::string excptstr = "Assertion fail. found "; \
    excptstr += std::to_string(token_stream->current().type);                              \
    excptstr += ", but expected ";\
    excptstr += std::to_string(_type.type); \
    throw AssertionException(&excptstr);       \
}

    class Lexer {
    private:
        IterableStream *token_stream;
    public:
        Lexer(IterableStream *token_stream) : token_stream(token_stream) {

        }

        void next() {
            token_stream->consume();
        }

        Node *expression() {
            return boolArithmetic();
        }

        Node *boolArithmetic() {
            Node *left = comparisonArithmetic();
            while (IS(Syntax::B_AND) || IS(Syntax::B_OR)) {
                uint8_t id = IS(Syntax::B_AND) ? LexNode::B_AND :
                             IS(Syntax::B_OR) ? LexNode::B_OR : throw AssertionException("Unknown bool type");
                NEXT;
                Node *right = comparisonArithmetic();
                left = createNode(id, left, right);
            }
            return left;
        }

        Node *comparisonArithmetic() {
            Node *left = arithmetic();
            if (IS(Syntax::GREATER) || IS(Syntax::GREATER_EQUAL) || IS(Syntax::EQUALS) ||
                IS(Syntax::NOT_EQUALS) || IS(Syntax::SMALLER) || IS(Syntax::SMALLER_EQUAL) ||
                IS(Syntax::APPROX_EQUALS)) {
                uint8_t id = IS(Syntax::GREATER) ? LexNode::GREATER :
                             IS(Syntax::GREATER_EQUAL) ? LexNode::GREATER_EQUAL :
                             IS(Syntax::EQUALS) ? LexNode::EQUALS :
                             IS(Syntax::NOT_EQUALS) ? LexNode::NOT_EQUALS :
                             IS(Syntax::SMALLER) ? LexNode::SMALLER :
                             IS(Syntax::SMALLER_EQUAL) ? LexNode::SMALLER_EQUAL :
                             IS(Syntax::APPROX_EQUALS) ? LexNode::APPROX_EQUALS : throw AssertionException(
                                     "Unknown bool type");
                NEXT;
                Node *right = arithmetic();
                left = createNode(id, left, right);
            }
            return left;
        }


        Node *arithmetic() {
            Node *left = factor();
            while (IS(Syntax::A_PLUS) || IS(Syntax::A_MINUS) || IS(Syntax::A_MULTIPLY) ||
                   IS(Syntax::A_DIVIDE) || IS(Syntax::A_MOD) || IS(Syntax::A_POW)) {
                uint8_t id = IS(Syntax::A_PLUS) ? LexNode::A_PLUS :
                             IS(Syntax::A_MINUS) ? LexNode::A_MINUS :
                             IS(Syntax::A_MULTIPLY) ? LexNode::A_MULTIPLY :
                             IS(Syntax::A_DIVIDE) ? LexNode::A_DIVIDE :
                             IS(Syntax::A_MOD) ? LexNode::A_MOD :
                             IS(Syntax::A_POW) ? LexNode::A_POW : throw AssertionException("Unknown bool type");
                NEXT;
                Node *right = factor();
                left = createNode(id, left, right);
            }
            return left;
        }

        Node *factor() {
            match(Syntax::A_MINUS) {
                NEXT;
                return createNode(LexNode::UNARY, factor());
            } else match(Syntax::B_NOT) {
                NEXT;
                return createNode(LexNode::B_NOT, factor());
            } else match(Syntax::A_PLUS) {
                NEXT;
                return factor();
            } else match(Syntax::NUMBER) {
                REMEMBER(num);
                NEXT;
                if (num.find('.') != std::string::npos) {
                    return createLeaf(LexNode::FLOAT_CONSTANT, num);
                }
                return createLeaf(LexNode::INT_CONSTANT, num);
            } else match(Syntax::TRUE) {
                NEXT;
                return createLeaf(LexNode::INT_CONSTANT, "1");
            } else match(Syntax::FALSE) {
                NEXT;
                return createLeaf(LexNode::INT_CONSTANT, "0");
            } else match(Syntax::PARENTHESES_OPEN) {
                NEXT;
                THEN(expression);
                ASSERT(Syntax::PARENTHESES_CLOSED);
                NEXT;
                return expression;
            } else match(Syntax::KEYWORD) {
                REMEMBER(name);
                NEXT;
                match(Syntax::PARENTHESES_OPEN) {

                    std::vector<Node *> childs;
                    do {
                        NEXT;
                        childs.push_back(createNode(LexNode::ARGUMENT, expression()));
                    } while (IS(Syntax::SEPARATOR));

                    ASSERT(Syntax::PARENTHESES_CLOSED);
                    NEXT;
                    Node *calls = createNode(LexNode::CALL_ARGUMENTS);
                    calls->childs = childs;
                    Node *call = createNode(LexNode::FUNCTION_CALL, calls, createLeaf(LexNode::IDENTIFIER, name));
                    return call;
                } else {
                    return createLeaf(LexNode::IDENTIFIER, name);
                }
            }
            THROW("Cant resolve factory ");
        }

        Node *functionDeclaration() {
            match(Syntax::KEYWORD) {
                REMEMBER(functionName);
                NEXT;
                THEN(functionInput);
                std::string returnType = "void";
                match(Syntax::ARROW) {
                    NEXT;
                    match(Syntax::KEYWORD) {
                        REMEMBER(type);
                        returnType = type;
                        NEXT;
                    } else THROW("expected a keyword");
                }
                THEN(block);
                Node *io = createNode(LexNode::FUNCTION_IO, functionInput,
                                      createNode(LexNode::FUNCTION_OUTPUT,
                                                 createLeaf(LexNode::TYPE_IDENTIFIER, returnType)));
                Node *name = createLeaf(LexNode::IDENTIFIER, functionName);
                Node *node = createNode(LexNode::FUNCTION_DECLARATION, name, io, block);
                return node;
            }
            THROW("Cant resolve function");
        }

        Node *functionInput() {
            match(Syntax::PARENTHESES_OPEN) {
                NEXT;
                std::vector<Node *> childs;
                match(Syntax::PARENTHESES_CLOSED) {
                    NEXT;
                } else {
                    Node *declaration = varDeclaration();
                    childs.push_back(declaration);
                    while (IS(Syntax::SEPARATOR)) {
                        NEXT;
                        Node *declaration = varDeclaration();
                        childs.push_back(declaration);
                    }
                    ASSERT(Syntax::PARENTHESES_CLOSED);
                    NEXT;
                }
                Node *node = createNode(LexNode::FUNCTION_INPUT);
                node->childs = childs;
                return node;
            }
            THROW("Cant resolve function input ")
        }

        Node *varDeclaration() {
            match(Syntax::KEYWORD) {
                REMEMBER(name);
                NEXT;
                match(Syntax::KEYWORD) {
                    REMEMBER(key);
                    NEXT;
                    Node *node = createNode(LexNode::VARIABLE_DECLARATION,
                                            createLeaf(LexNode::TYPE_IDENTIFIER, name),
                                            createLeaf(LexNode::IDENTIFIER, key)
                    );
                    return node;
                }
            }
            THROW("Cant resolve variable declaration ");
        }

        Node *block() {
            match(Syntax::BRACES_OPEN) {
                NEXT;
                std::vector<Node *> childs;
                while (!IS(Syntax::BRACES_CLOSED)) {
                    THEN(statement);
                    childs.push_back(statement);
                }
                ASSERT(Syntax::BRACES_CLOSED);
                NEXT;
                Node *node = createNode(LexNode::STATEMENT_BLOCK);
                node->childs = childs;
                return node;
            }
            THROW("Cant resolve block");
        }

        Node *statement() {
            match(Syntax::KEYWORD) {
                REMEMBER(typeOrName);
                NEXT;
                match(Syntax::KEYWORD) {
                    REMEMBER(key);
                    NEXT;
                    match(Syntax::ASSIGN) {
                        NEXT;
                        Node *node = createNode(LexNode::VAR_CREATION_STATEMENT,
                                                createLeaf(LexNode::TYPE_IDENTIFIER, typeOrName),
                                                createLeaf(LexNode::IDENTIFIER, key),
                                                createNode(LexNode::EXPRESSION, expression())
                        );
                        return node;
                    }
                } else match(Syntax::ASSIGN) {
                    NEXT;
                    Node *node = createNode(LexNode::ASSIGN_STATEMENT, createLeaf(LexNode::IDENTIFIER, typeOrName),
                                            createNode(LexNode::EXPRESSION, expression()));
                    return node;
                } else match(Syntax::PARENTHESES_OPEN) {

                    std::vector<Node *> childs;
                    do {
                        NEXT;
                        childs.push_back(createNode(LexNode::ARGUMENT, expression()));
                    } while (IS(Syntax::SEPARATOR));

                    ASSERT(Syntax::PARENTHESES_CLOSED);
                    NEXT;

                    Node *calls = createNode(LexNode::CALL_ARGUMENTS);
                    calls->childs = childs;
                    Node *call = createNode(LexNode::FUNCTION_CALL, calls, createLeaf(LexNode::IDENTIFIER, typeOrName));
                    return call;
                }
            } else match(Syntax::FOR) {
               
            } else match(Syntax::IF) {
                NEXT;
                THEN(expression);
                THEN(block);
                Node *elseBlock = nullptr;
                match(Syntax::ELSE) {
                    NEXT;
                    elseBlock = this->block();
                }
                Node *positive = createNode(LexNode::BRANCH_POSITIVE, block);
                Node *negative = createNode(LexNode::BRANCH_NEGATIVE);
                if (elseBlock) {
                    negative->childs.push_back(elseBlock);
                } else {
                    negative->childs.push_back(createNode(LexNode::STATEMENT_BLOCK));
                }
                Node *expr = createNode(LexNode::EXPRESSION, expression);
                Node *node = createNode(LexNode::BRANCH, positive, negative, expr);
                return node;
            }
            THROW("Cant resolve variable statement ");
        }


        Node *createLeaf(uint8_t lex_type, std::string literal) {
            Node *node = new Node{lex_type, literal};
            return node;
        }

        Node *createNode(uint8_t lex_type) {
            Node *node = new Node{lex_type, ""};
            return node;
        }

        Node *createNode(uint8_t lex_type, Node *child) {
            Node *node = new Node{lex_type, ""};
            node->childs.push_back(child);
            return node;
        }

        Node *createNode(uint8_t lex_type, Node *childA, Node *childB) {
            Node *node = new Node{lex_type, ""};
            node->childs.push_back(childA);
            node->childs.push_back(childB);
            return node;
        }

        Node *createNode(uint8_t lex_type, Node *childA, Node *childB, Node *childC) {
            Node *node = new Node{lex_type, ""};
            node->childs.push_back(childA);
            node->childs.push_back(childB);
            node->childs.push_back(childC);
            return node;
        }
    };
}

#endif //DATA_PROCESSING_INTERMEDIATE_LEXER_H
