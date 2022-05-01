#include <iostream>
#include <fstream>
#include "Tokenizer.h"
#include "IterableStream.h"
#include "DPI_Syntax.h"
#include "LexerC.h"
#include "Translator.h"


void printBT2(const std::string &prefix, const SyntaxNode *node, bool isLeft) {
    if (node != nullptr) {
        std::cout << prefix;
        std::cout << (isLeft ? "|--" : "L..");

        std::cout << LexNode::NAMES[node->lex_type] << ": " << node->data << std::endl;

        if(node->left && node->right) {
            printBT2(prefix + (isLeft ? "|   " : "    "), node->left, true);
            printBT2(prefix + (isLeft ? "|   " : "    "), node->right, false);
        } else {
            printBT2(prefix + (isLeft ? "|   " : "    "), node->left, false);
            printBT2(prefix + (isLeft ? "|   " : "    "), node->right, true);
        }

    }
}

void printBT2(const std::string &prefix, const Statement *node, bool isLeft) {
    if (node != nullptr) {
        std::cout << prefix;
        std::cout << (isLeft ? "|--" : "L..");

        std::string name = "";
        switch (node->type) {
            case DECLARATION_STATEMENT:
                name = "DECLARATION_STATEMENT";
                break;
            case ASSIGNMENT_STATEMENT:
                name = "ASSIGNMENT_STATEMENT";
                break;
            case ACTION_STATEMENT:
                name = "ACTION_STATEMENT";
                break;
            case BRANCH_STATEMENT:
                name = "BRANCH_STATEMENT";
                break;
            case BLOCK_STATEMENT_:
                name = "BLOCK_STATEMENT";
                break;
            case EMPTY_STATEMENT_:
                name = "EMPTY";
                break;
            default:
                name = "Unknown";
                break;
        }
        std::cout << name << "> " << std::endl;

        switch (node->type) {
            case DECLARATION_STATEMENT:
                printBT2(prefix + (isLeft ? "|   " : "    "),node->object.declaration->expression, false);
                break;
            case ASSIGNMENT_STATEMENT:
                printBT2(prefix + (isLeft ? "|   " : "    "),node->object.assignment->value, true);
                printBT2(prefix + (isLeft ? "|   " : "    "), node->object.assignment->member, false);
                break;
            case ACTION_STATEMENT:
                printBT2(prefix + (isLeft ? "|   " : "    "),node->object.action->action, false);
                break;
            case BRANCH_STATEMENT:
                printBT2(prefix + (isLeft ? "|   " : "    "),node->object.branch->condition, true);
                printBT2(prefix + (isLeft ? "|   " : "    "),node->object.branch->content, node->object.branch->elseContent);
                printBT2(prefix + (isLeft ? "|   " : "    "),node->object.branch->elseContent, false);
                break;
            case BLOCK_STATEMENT_:
                for(uint32_t i = 0; i < node->object.block->statements.size(); i++) {
                    printBT2(prefix + (isLeft ? "|   " : "    "), node->object.block->statements[i], i != node->object.block->statements.size()-1);
                }
                break;
            default:
                break;
        }
    }
}



int main() {
    //Interpreter::feed();
    //Interpreter::run();
    std::vector<Token_Capture> caps;

    caps.push_back(Syntax::BRACES_OPEN);
    caps.push_back(Syntax::BRACES_CLOSED);
    caps.push_back(Syntax::PARENTHESES_OPEN);
    caps.push_back(Syntax::PARENTHESES_CLOSED);
    caps.push_back(Syntax::BRACKET_CLOSED);
    caps.push_back(Syntax::BRACKET_OPEN);
    caps.push_back(Syntax::A_PLUS);
    caps.push_back(Syntax::A_MINUS);
    caps.push_back(Syntax::A_MULTIPLY);
    caps.push_back(Syntax::A_DIVIDE);
    caps.push_back(Syntax::A_MOD);
    caps.push_back(Syntax::A_POW);
    caps.push_back(Syntax::SMALLER);
    caps.push_back(Syntax::SMALLER_EQUAL);
    caps.push_back(Syntax::GREATER);
    caps.push_back(Syntax::GREATER_EQUAL);
    caps.push_back(Syntax::EQUALS);
    caps.push_back(Syntax::TRUE);
    caps.push_back(Syntax::FALSE);
    caps.push_back(Syntax::IF);
    caps.push_back(Syntax::ELSE);
    caps.push_back(Syntax::FOR);
    caps.push_back(Syntax::STATIC);
    caps.push_back(Syntax::STRUCT);
    caps.push_back(Syntax::FN);
    caps.push_back(Syntax::APPROX_EQUALS);
    caps.push_back(Syntax::NOT_EQUALS);
    caps.push_back(Syntax::B_AND);
    caps.push_back(Syntax::B_OR);
    caps.push_back(Syntax::B_NOT);
    caps.push_back(Syntax::ASSIGN);
    caps.push_back(Syntax::ARROW);
    caps.push_back(Syntax::SEPARATOR);
    caps.push_back(Syntax::DOT);
    caps.push_back(Syntax::KEYWORD);
    caps.push_back(Syntax::NUMBER);
    caps.push_back(Syntax::COMMENT);
//    caps.push_back(Syntax::LINE_COMMENT);
    caps.push_back(Syntax::WHITESPACE);
    caps.push_back(Syntax::END_OF_STATEMENT);

    Tokenizer tokenizer(caps);


    std::ifstream ifs("../files/test.txt");
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));

    const std::string &str = content;
    const char *string = str.c_str();
    std::vector<Token> *stream = tokenizer.generate_stream(string, str.length());

    std::vector<Token> filtered(*stream);
    filtered.erase(std::remove_if(filtered.begin(), filtered.end(), [](const Token &token) {
        return token.type == Syntax::WHITESPACE.type ||
               token.type == Syntax::EOL.type;
    }), filtered.end());


    IterableStream iterableStream(&filtered);
    uint64_t i = 3;
    uint64_t index = 0;
    while (iterableStream.hasEntriesLeft()) {
        const Token &current = iterableStream.current();
        i += current.type * i;
        iterableStream.consume();
        index++;
    }

    iterableStream.reset();

    LexerC lexer(&iterableStream);
    const std::vector<SyntaxNode *> &vector = lexer.entry();
    for (SyntaxNode *item: vector) {
        printBT2("", item, false);
    }

    Translator translator(vector);
    translator.translate();


    for (const auto &item : translator.namespaces) {
        printBT2("",item.second->statement ,false);
    }


    for (SyntaxNode *item: vector) {
        delete item;
    }

    delete stream;

    return 0;
}


