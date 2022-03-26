//
// Created by Mo on 24.03.2022.
//

#ifndef DATA_PROCESSING_INTERMEDIATE_LEXER_H
#define DATA_PROCESSING_INTERMEDIATE_LEXER_H

#include "Tokenizer.h"
#include "IterableStream.h"
#include "DPI_Syntax.h"

class AssertionException: public std::exception
{
    virtual const char* what() const noexcept
    {
        return "Failed Assertion";
    }
} excp;

struct Node {
    uint8_t lex_type = 0;
    std::string data;
    std::vector<Node *> childs = std::vector<Node *>();
};
#define TOKEN token_stream->current()
#define TYPE token_stream->current().type
#define match(_type) if(token_stream->current().type == (_type).type)
#define IS(_type) token_stream->current().type == (_type).type
#define NEXT next()
#define AS(type) std::string (type) = token_stream->current().raw_string
#define expect(str) else { throw (str);}
#define THEN(name) Node *name = this->name()
#define ASSERT(_type) if((_type).type != token_stream->current().type) { throw excp; }

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
        match(Syntax::PARENTHESES_OPEN) {
            NEXT;

            //WHILE_NOT(Syntax::PARENTHESES_CLOSED, expression() , exp);
            // RET {LexNode::ST};
        } else match(Syntax::NUMBER) {

        } expect("1 or 2");
    }

    Node *varExpression() {
        Node *left = factor();
        NEXT;
        while (IS(Syntax::A_PLUS) || IS(Syntax::A_MINUS) || IS(Syntax::A_MULTIPLY) ||
               IS(Syntax::A_DIVIDE) || IS(Syntax::A_MOD) || IS(Syntax::A_POW)) {
            NEXT;
            Node *right = factor();
            uint8_t id = IS(Syntax::A_PLUS) ? LexNode::A_PLUS :
                         IS(Syntax::A_MINUS) ? LexNode::A_MINUS :
                         IS(Syntax::A_MULTIPLY) ? LexNode::A_MULTIPLY :
                         IS(Syntax::A_DIVIDE) ? LexNode::A_DIVIDE :
                         IS(Syntax::A_MOD) ? LexNode::A_MOD :
                         LexNode::A_POW;

            left = createNode(id , left , right);
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
            return expression;
        }
    }

    Node *statement() {
        match(Syntax::BRACES_OPEN) {
            NEXT;
            THEN(statement);
            ASSERT(Syntax::BRACES_CLOSED)
            return statement;
        } else match(Syntax::KEYWORD) {
            AS(ID);
            NEXT;
            match(Syntax::ASSIGN) {
                NEXT;
                THEN(expression);
                Node *node = createNode(LexNode::ASSIGN_STATEMENT);
                node->childs.push_back(createLeaf(LexNode::IDENTIFIER, ID));
                node->childs.push_back(createNode(LexNode::EXPRESSION, expression));
                return node;
            }
        } expect("1 or 2")
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
    Node *createNode(uint8_t lex_type, Node *childA , Node *childB) {
        Node *node = new Node{lex_type, ""};
        node->childs.push_back(childA);
        node->childs.push_back(childB);
        return node;
    }

};


#endif //DATA_PROCESSING_INTERMEDIATE_LEXER_H
