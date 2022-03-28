
#ifndef DATA_PROCESSING_INTERMEDIATE_INTERPRETER_H
#define DATA_PROCESSING_INTERMEDIATE_INTERPRETER_H


#include <cstdint>
#include <cmath>
#include "DPI_Syntax.h"
#include "Math.h"

#define Register(index) record_stack[activation_record_pointer+(index)]
namespace Interpreter {
    const uint8_t type_number = 0;
    typedef union {
        dpi_float as_float;
        dpi_int as_int;
        void *as_pointer;
    } Value;

    typedef struct {
        uint8_t type;
        Value value;
    } Object;

    typedef union {
        uint8_t u;
        int8_t s;
    } Byte_Field;

    typedef struct {
        uint8_t op;
        uint8_t A;
        uint8_t B;
        uint8_t C;
    } Op_Code;

    typedef union {
        int32_t as_int;
        Op_Code code;
    } Instruction;

    Instruction *instructions = nullptr;

    uint64_t activation_record_pointer = 0;
    Object *record_stack;

    uint64_t pc = 0;

    void cycle() {
        //uint32_t full = (a << 16) | (b << 8) | c;
        Instruction i = instructions[pc];
        switch (i.code.op) {
            case IRCode::MOVE: {
                Register(i.code.A) = Register(i.code.B);
                break;
            }
            case IRCode::JUMP: {
                uint16_t position = (i.code.B << 8) | i.code.C;
                if (Register(i.code.A).value.as_int >= 0.5) {
                    pc = position;
                } else {
                    pc++;
                }
                break;
            }
            case IRCode::EQUALS: {
//                Register(i.code.A).value.as_number = math_isequals(Register(i.code.B).value.as_number,
//                                                                   Register(i.code.C).value.as_number);
                break;
            }
            case IRCode::ADD: {
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dpi_float f = (x.type == 0 ? x.value.as_int : x.value.as_float) +
                              (y.type == 0 ? y.value.as_int : y.value.as_float);
                if (dest.type == 0) {
                    dest.value.as_int = (int) f;
                } else {
                    dest.value.as_float = f;
                }

//                Register(i.code.A).value.as_number =
//                        Register(i.code.B).value.as_number + Register(i.code.B).value.as_number;
                break;
            }
        }
    }


}

#endif //DATA_PROCESSING_INTERMEDIATE_INTERPRETER_H
