
#ifndef DATA_PROCESSING_INTERMEDIATE_VM_H
#define DATA_PROCESSING_INTERMEDIATE_VM_H


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
        uint16_t type;
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
        Op_Code code;
        int32_t as_int;
    } Instruction;


    Object *constants;
    uint64_t max_constants = 256;

    Instruction *instructions = nullptr;
    uint64_t max_instructions = 256;
    uint64_t instructionBuilder = 0;

    uint64_t activation_record_pointer = 0;
    Object *record_stack;
    uint64_t max_records = 256;

    uint64_t pc = 0;

    void add(Instruction i) {
        instructions[instructionBuilder] = i;
        instructionBuilder++;
    }

    void feed() {
        record_stack = (Object *) malloc(max_records * sizeof(Object));
        for (int i = 0; i < max_records; i++) {
            record_stack[i] = {};
        }
        instructions = (Instruction *) malloc(max_instructions * sizeof(Instruction));
        constants = (Object *) malloc(max_constants * sizeof(Object));
        constants[0] = {0, {0}};
        constants[1] = {0, {1}};
        constants[2] = {0, {1984.1337}};

        add({{IRCode::LOAD_CONST, 2, 0, 2}});
        add({{IRCode::PRINT, 2, 0, 0}});
        add({{IRCode::END_OF_CODE}});
    }

    void pushActivationRecord(uint8_t size) {
        activation_record_pointer += size;
    }

    void cycle() {
        //uint32_t full = (a << 16) | (b << 8) | c;
        Instruction i = instructions[pc];
        switch (i.code.op) {
            case IRCode::PRINT: {
                Object &a = Register(i.code.A);
                std::cout << "- Register " << (uint16_t)i.code.A << " -" << std::endl;
                std::cout << "Type: " << a.type << std::endl;
                std::cout << "Float: " << a.value.as_float << std::endl;
                std::cout << "Int: " << a.value.as_int << std::endl;
                std::cout << "---------" << std::endl;
                pc++;
                break;
            }
            case IRCode::LOAD_CONST: {
                Register(i.code.A) = constants[((i.code.B << 8) | i.code.C)];
                pc++;
                break;
            }
            case IRCode::MOVE: {
                Register(i.code.A) = Register(i.code.B);
                pc++;
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
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dest.value.as_float = math_abs(x.value.as_float - y.value.as_float) < 0.0001f;
                //                Register(i.code.A).value.as_number = math_isequals(Register(i.code.B).value.as_number,
//                                                                   Register(i.code.C).value.as_number);
                pc++;
                break;
            }
            case IRCode::ADD: {
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dest.value.as_float = x.value.as_float + y.value.as_float;
//                Register(i.code.A).value.as_number =
//                        Register(i.code.B).value.as_number + Register(i.code.B).value.as_number;
                pc++;
                break;
            }
        }
    }


}

#endif //DATA_PROCESSING_INTERMEDIATE_VM_H
