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
#define IS(_type) token_stream->hasEntriesLeft() && token_stream->current().type == (_type).type
#define NEXT next()
#define AS(type) std::string (type) = token_stream->current().raw_string
#define expect(str) else { throw (str);}
#define THEN(name) Node *name = this->name()
#define THROW(text) {std::string str= (text); \
    str += std::to_string(TOKEN.type); \
    throw AssertionException(&str);}

#define ASSERT(_type) if((_type).type != token_stream->current().type) { \
    std::string excptstr = "Assertion fail for "; \
    excptstr += std::to_string(token_stream->current().type);                              \
    excptstr += " instead of ";\
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
            return boolExpression();
        }

        Node *boolExpression() {
            Node *left = varComparison();
            while (IS(Syntax::B_AND) || IS(Syntax::B_OR)) {
                uint8_t id = IS(Syntax::B_AND) ? LexNode::B_AND :
                             IS(Syntax::B_OR) ? LexNode::B_OR : throw AssertionException("Unknown bool type");
                NEXT;
                Node *right = varComparison();
                left = createNode(id, left, right);
            }
            return left;
        }

        Node *varComparison() {
            Node *left = varExpression();
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
                Node *right = varExpression();
                left = createNode(id, left, right);
            }
            return left;
        }


        Node *varExpression() {
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
                AS(num);
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
            }
            THROW("Cant resolve factory ");
        }

        Node *functionDeclaration() {
            match(Syntax::KEYWORD) {
                AS(functionName);
                NEXT;
                THEN(functionInput);
                std::string returnType = "";
                match(Syntax::ARROW) {
                    NEXT;
                    match(Syntax::KEYWORD) {
                        AS(type);
                        returnType = type;
                        NEXT;
                    } else THROW("expected a keyword");
                }
                THEN(statement);
                Node *io = createNode(LexNode::FUNCTION_IO, functionInput,
                                      createNode(LexNode::FUNCTION_OUTPUT,
                                                 createLeaf(LexNode::TYPE_IDENTIFIER, returnType)));
                Node *name = createLeaf(LexNode::IDENTIFIER, functionName);
                Node *node = createNode(LexNode::FUNCTION_DECLARATION, name, io);
                node->childs.push_back(createNode(LexNode::STATEMENT,statement));
                return node;
            }
            THROW("Cant resolve function");
        }

        Node *statement() {
            return createLeaf(LexNode::ASSIGN_STATEMENT, "statement end leaf");
        }

        Node *functionInput() {
            std::vector<Node *> childs;
            match(Syntax::PARENTHESES_OPEN) {
                NEXT;
                match(Syntax::PARENTHESES_CLOSED) {
                    NEXT;
                } else {
                    do {
                        Node *declaration = varDeclaration();
                        childs.push_back(declaration);
                    } while (IS(Syntax::SEPARATOR));
                    ASSERT(Syntax::PARENTHESES_CLOSED);
                    NEXT;
                }

            }
            Node *node = createNode(LexNode::FUNCTION_INPUT);
            node->childs = childs;
            return node;
        }

        Node *varDeclaration() {
            match(Syntax::KEYWORD) {
                AS(name);
                NEXT;
                match(Syntax::KEYWORD) {
                    AS(key);
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

        Node *varDeclarationStatement() {
            match(Syntax::KEYWORD) {
                AS(typeOrName);
                NEXT;
                match(Syntax::KEYWORD) {
                    AS(key);
                    NEXT;
                    match(Syntax::ASSIGN) {
                        NEXT;
                        Node *node = createNode(LexNode::VAR_CREATION_STATEMENT,
                                                createLeaf(LexNode::TYPE_IDENTIFIER, typeOrName),
                                                createLeaf(LexNode::IDENTIFIER, key));
                        node->childs.push_back(createNode(LexNode::EXPRESSION, expression()));
                        return node;
                    }
                } else match(Syntax::ASSIGN) {
                    NEXT;
                    Node *node = createNode(LexNode::ASSIGN_STATEMENT, createLeaf(LexNode::IDENTIFIER, typeOrName),
                                            createNode(LexNode::EXPRESSION, expression()));
                    return node;
                }
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

    };
}

#endif //DATA_PROCESSING_INTERMEDIATE_LEXER_H
