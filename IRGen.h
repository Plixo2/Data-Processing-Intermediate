//
// Created by Mo on 18.04.2022.
//

#ifndef DATA_PROCESSING_INTERMEDIATE_IRGEN_H
#define DATA_PROCESSING_INTERMEDIATE_IRGEN_H


#include "Translator.h"

#define Reg Register*
namespace Gen {
    Reg makeMove(Reg src, Reg dest);
    Reg makeAdd(Reg a, Reg b);
    Reg makeInt(uint64_t number);
    Reg makeFloat(double number);
    //Reg makeCall(double number);
}


#endif //DATA_PROCESSING_INTERMEDIATE_IRGEN_H
