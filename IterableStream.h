//
// Created by Mo on 24.03.2022.
//

#ifndef DATA_PROCESSING_INTERMEDIATE_ITERABLESTREAM_H
#define DATA_PROCESSING_INTERMEDIATE_ITERABLESTREAM_H


#include "Tokenizer.h"

class IterableStream {
private:
    std::vector<Token> *v;
    uint64_t index = 0;
public:
    IterableStream(std::vector<Token> *vector) : v(vector) {

    }

    Token current() {
        return v->at(index);
    }
    bool hasEntriesLeft() {
        return index < v->size();
    }

    void consume() {
        index += 1;
    }
    void reset() {
        index = 0;
    }
};


#endif //DATA_PROCESSING_INTERMEDIATE_ITERABLESTREAM_H
