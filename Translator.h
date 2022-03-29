//
// Created by Mo on 30.03.2022.
//

#ifndef DATA_PROCESSING_INTERMEDIATE_TRANSLATOR_H
#define DATA_PROCESSING_INTERMEDIATE_TRANSLATOR_H

#include "Lexer.h"

using namespace Lexer;

class Translator {
 private:
    Node *node;
public:
    Translator(Node *node): node(node) {

    }

};

#endif //DATA_PROCESSING_INTERMEDIATE_TRANSLATOR_H
