
#ifndef DATA_PROCESSING_INTERMEDIATE_VM_H
#define DATA_PROCESSING_INTERMEDIATE_VM_H


#include <cstdint>
#include <cmath>
#include "DPI_Syntax.h"
#include "Math.h"

#define Register(index) record_stack[activation_record_pointer+(index)]
#define Push(obj) {call_stack[stack_pointer] = (obj); stack_pointer++;}
#define Pop(obj) {(obj) = call_stack[--stack_pointer];}
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
    } ByteField;

    typedef struct {
        uint8_t op;
        uint8_t A;
        uint8_t B;
        uint8_t C;
    } OpCode;

    typedef union {
        OpCode code;
        int32_t as_int;
    } Instruction;


    typedef struct {
        uint8_t activation_records;
        uint32_t instruction_index;
    } FunctionProto;

    FunctionProto *functional_prototypes;
    uint64_t max_prototypes = 256;

    Object *constants;
    uint64_t max_constants = 256;

    Instruction *instructions = nullptr;
    uint64_t max_instructions = 256;
    uint64_t instructionBuilder = 0;

    uint64_t activation_record_pointer = 0;
    uint64_t stack_pointer = 0;
    Object *record_stack = nullptr;
    uint32_t *call_stack = nullptr;
    uint64_t max_stack = 256;


    uint64_t pc = 0;
    bool isRunning = false;

    void add(Instruction i) {
        instructions[instructionBuilder] = i;
        instructionBuilder++;
    }

    void feed() {
        record_stack = (Object *) malloc(max_stack * sizeof(Object));
        for (int i = 0; i < max_stack; i++) {
            record_stack[i] = {};
        }
        call_stack = (uint32_t *) malloc(max_stack * sizeof(uint32_t));
        functional_prototypes = (FunctionProto *) malloc(max_prototypes * sizeof(FunctionProto));
        instructions = (Instruction *) malloc(max_instructions * sizeof(Instruction));
        for (int i = 0; i < max_instructions; i++) {
            instructions[i] = {{IRCode::END_OF_CODE}};
        }
        constants = (Object *) malloc(max_constants * sizeof(Object));
        constants[0] = {0, {-0}};
        constants[1] = {0, {2.0}};
        constants[2] = {0, {3.1}};
        constants[3] = {0, {1.0}};

//        add({{IRCode::LOAD_CONST, 1, 0, 2}});
//        add({{IRCode::LOAD_CONST, 2, 0, 3}});
//        add({{IRCode::LOAD_CONSkT, 4, 0, 1}});
//        add({{IRCode::MINUS, 3, 1, 2}});
//        add({{IRCode::EQUALS, 5, 3, 4}});
//        add({{IRCode::PRINT, 5, 0, 0}});
//        add({{IRCode::JUMP, 5, 0, 8}});
//        add({{IRCode::NOP, 0, 0, 0}});
//        add({{IRCode::PRINT, 1, 0, 0}}); //prints "3";
        // add({{IRCode::PRINT, 2, 0, 0}}); //prints "1";
        //add({{IRCode::CALL, 2, 0, 1}});

        FunctionProto proto = {2, 6};
        functional_prototypes[1] = proto;

        add({{IRCode::LOAD_CONST, 0, 0, 0}});
        add({{IRCode::LOAD_CONST, 1, 0, 1}});
        add({{IRCode::SMALLER_EQUAL, 2, 0, 1}});
        add({{IRCode::PRINT, 2}});
//        add({{IRCode::CALL, 0, 0, 1}});
//        add({{IRCode::JUMP_A, 0, 0, 10}});
//        add({{IRCode::NOP}});
//        add({{IRCode::NOP}});
//        add({{IRCode::PRINT, 0}});//unknown
//        add({{IRCode::LOAD_CONST, 0, 0, 1}});
//        add({{IRCode::PRINT, 0}});//"2.0"
//        add({{IRCode::RETURN, 0, 0, 1}});

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
                std::cout << "- Register " << (uint16_t) i.code.A << " -" << std::endl;
                std::cout << "Type: " << a.type << std::endl;
                std::cout << "Float: " << a.value.as_float << std::endl;
                // std::cout << "Int: " << a.value.as_int << std::endl;
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
            case IRCode::JUMP_A: {
                uint16_t position = (i.code.B << 8) | i.code.C;
                pc = position;
                break;
            }
            case IRCode::JUMP: {
                uint16_t position = (i.code.B << 8) | i.code.C;
                if (Register(i.code.A).value.as_float >= 0.5) {
                    pc = position;
                } else {
                    pc++;
                }
                break;
            }
            case IRCode::CALL: {
                uint16_t function = (i.code.A << 16) | (i.code.B << 8) | i.code.C;
                FunctionProto &proto = functional_prototypes[function];
                activation_record_pointer += proto.activation_records;
                Push(pc+1);
                pc = proto.instruction_index;
                break;
            }
            case IRCode::RETURN: {
                uint16_t function = (i.code.A << 16) | (i.code.B << 8) | i.code.C;
                FunctionProto &proto = functional_prototypes[function];
                activation_record_pointer -= proto.activation_records;
                Pop(pc);
                break;
            }
            case IRCode::AND: {
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dest.value.as_float = x.value.as_float && y.value.as_float;
                pc++;
                break;
            }
            case IRCode::OR: {
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dest.value.as_float = x.value.as_float || y.value.as_float;
                pc++;
                break;
            }
            case IRCode::NOT: {
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dest.value.as_float = !x.value.as_float;
                pc++;
                break;
            }
            case IRCode::ADD: {
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dest.value.as_float = x.value.as_float + y.value.as_float;
                pc++;
                break;
            }
            case IRCode::POW: {
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dest.value.as_float = std::pow(x.value.as_float,y.value.as_float);
                pc++;
                break;
            }
            case IRCode::MOD: {
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dest.value.as_float = std::fmod(x.value.as_float,y.value.as_float);
                pc++;
                break;
            }
            case IRCode::MINUS: {
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dest.value.as_float = x.value.as_float - y.value.as_float;
                pc++;
                break;
            }
            case IRCode::DIVIDE: {
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dest.value.as_float = x.value.as_float / y.value.as_float;
                pc++;
                break;
            }
            case IRCode::MULTIPLY: {
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dest.value.as_float = x.value.as_float * y.value.as_float;
                pc++;
                break;
            }
            case IRCode::EQUALS: {
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dest.value.as_float = math_abs(x.value.as_float - y.value.as_float) < 0.0001f;
                pc++;
                break;
            }
            case IRCode::NOT_EQUALS: {
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dest.value.as_float = math_abs(x.value.as_float - y.value.as_float) >= 0.0001f;
                pc++;
                break;
            }
            case IRCode::SMALLER: {
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dest.value.as_float = x.value.as_float < y.value.as_float;
                pc++;
                break;
            }
            case IRCode::SMALLER_EQUAL: {
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dest.value.as_float = x.value.as_float <= y.value.as_float;
                pc++;
                break;
            }
            case IRCode::GREATER: {
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dest.value.as_float = x.value.as_float > y.value.as_float;
                pc++;
                break;
            }
            case IRCode::GREATER_EQUAL: {
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dest.value.as_float = x.value.as_float >= y.value.as_float;
                pc++;
                break;
            }
            case IRCode::NOP: {
                std::cout << "-NOP-" << std::endl;
                pc++;
                break;
            } case IRCode::END_OF_CODE: {
                isRunning = false;
                break;
            }
            default: {
                std::cout << "Unknown command: " << i.code.op << std::endl;
                break;
            }
        }
        std::cout << "Program counter at " << pc << std::endl;
    }


    void run() {
        isRunning = true;
        while (isRunning) {
            cycle();
        }
        std::cout << "######### End Of Program #########" << std::endl;
    }

}

#endif //DATA_PROCESSING_INTERMEDIATE_VM_H
