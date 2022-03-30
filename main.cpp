#include <iostream>
#include <fstream>
#include "Tokenizer.h"
#include "IterableStream.h"
#include "DPI_Syntax.h"
#include "Lexer.h"
#include "Vm.h"


void printBT(const std::string& prefix, const  Lexer::Node* node, bool isLeft)
{
    if( node != nullptr )
    {
        std::cout << prefix;

        std::cout << (isLeft ? "|--" : "L--" );

        // print the value of the node
        std::cout <<  LexNode::NAMES[node->lex_type] << ": " << node->data << std::endl;

        // enter the next tree level - left and right branch
        if(node->childs.size() >= 1) {
            for(int i = 0; i < node->childs.size()-1; i++) {
                printBT( prefix + (isLeft ? "|   " : "    "), node->childs[i], true);
            }
            printBT( prefix + (isLeft ? "|   " : "    "), node->childs[node->childs.size()-1], false);

        }
    }
}

int main() {
    Interpreter::feed();

    Interpreter::cycle();
    Interpreter::cycle();
    /*std::vector<Token_Capture> caps;

    caps.push_back(Syntax::BRACES_OPEN);
    caps.push_back(Syntax::BRACES_CLOSED);
    caps.push_back(Syntax::PARENTHESES_OPEN);
    caps.push_back(Syntax::PARENTHESES_CLOSED);
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
    caps.push_back(Syntax::AS);
    caps.push_back(Syntax::IN);
    caps.push_back(Syntax::FROM);
    caps.push_back(Syntax::TO);
    caps.push_back(Syntax::APPROX_EQUALS);
    caps.push_back(Syntax::NOT_EQUALS);
    caps.push_back(Syntax::B_AND);
    caps.push_back(Syntax::B_OR);
    caps.push_back(Syntax::B_NOT);
    caps.push_back(Syntax::ASSIGN);
    caps.push_back(Syntax::ARROW);
    caps.push_back(Syntax::SEPARATOR);
    caps.push_back(Syntax::KEYWORD);
    caps.push_back(Syntax::NUMBER);
    caps.push_back(Syntax::COMMENT);
    caps.push_back(Syntax::LINE_COMMENT);
    caps.push_back(Syntax::WHITESPACE);

    Tokenizer tokenizer(caps);
//    plus , minus , div , mul , pow , mod
//    greater equals, greater, smaller , smaller equals, equals , not equals . approx equals
//    and / or
//    parentheses


    std::ifstream ifs("../files/test.txt");
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );

    std::string str = content;
    std::cout << str << std::endl;
    const char *string = str.c_str();
    std::vector<Token> *stream = tokenizer.generate_stream(string, str.length());

    std::vector<Token> filtered(*stream);
    filtered.erase(std::remove_if(filtered.begin(), filtered.end(), [](const Token &token) {
        return token.type == Syntax::WHITESPACE.type ||
               token.type == Syntax::EOL.type;
    }), filtered.end());

//    std::cout << filtered.size() << std::endl;
//    std::cout << stream->size() << std::endl;

    IterableStream iterableStream(&filtered);
    uint64_t i = 3;
    while (iterableStream.hasEntriesLeft()) {
        const Token &current = iterableStream.current();
        std::cout << current.type << " | " << current.raw_string << std::endl;
        i += current.type * i;
        iterableStream.consume();
    }
    std::cout << "h: " << i % 10001 << std::endl;

    iterableStream.reset();
    Lexer::Lexer lexer(&iterableStream);
    Lexer::Node *pNode = lexer.functionDeclaration();

    printBT("", pNode, false);

    delete pNode;
    delete stream;*/

    return 0;
}


