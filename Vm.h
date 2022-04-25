
#ifndef DATA_PROCESSING_INTERMEDIATE_VM_H
#define DATA_PROCESSING_INTERMEDIATE_VM_H


#include <cstdint>
#include <cmath>
#include <sstream>
#include <chrono>
#include <iostream>
#include "DPI_Types.h"
#include "Math.h"

#define Register(index) record_stack[activation_record_pointer+(index)]
#define Push(obj) {call_stack[stack_pointer++] = (obj);}
#define Pop(obj) {(obj) = call_stack[--stack_pointer];}

using namespace types;

namespace Interpreter {


    std::vector<ObjectTable *> object_pool;

    ObjectPrototype *prototypes;
    uint64_t max_prototypes;


    // FunctionProto *functional_prototypes;
    //  uint64_t max_function_prototypes = 256;

    Object *constants;
    uint64_t max_constants = 256;

    Instruction *instructions = nullptr;
    uint64_t max_instructions = 256;
    uint64_t instructionBuilder = 0;

    uint64_t activation_record_pointer = 0;
    uint64_t stack_pointer = 0;
    Object *record_stack = nullptr;
    uint64_t *call_stack = nullptr;
    uint64_t max_stack = 256;

    uint64_t active_record_size = 0;

    uint64_t pc = 0;
    bool isRunning = false;

    uint64_t runns = 0;

    void add(Instruction i) {
        instructions[instructionBuilder] = i;
        instructionBuilder++;
    }

    void allocate() {
        record_stack = (Object *) malloc(max_stack * sizeof(Object));
        for (int i = 0; i < max_stack; i++) {
            record_stack[i] = {};
        }
        call_stack = (uint64_t *) malloc(max_stack * sizeof(uint64_t));
        //  functional_prototypes = (FunctionProto *) malloc(max_function_prototypes * sizeof(FunctionProto));
        instructions = (Instruction *) malloc(max_instructions * sizeof(Instruction));
        for (int i = 0; i < max_instructions; i++) {
            instructions[i] = {{IRCode::END_OF_CODE}};
        }
        prototypes = (ObjectPrototype *) malloc(max_prototypes * sizeof(ObjectPrototype));
        constants = (Object *) malloc(max_constants * sizeof(Object));
    }

    void feed() {
        allocate();
        constants[0] = {0, {1.0}};
        constants[1] = {0, {0.2}};
        constants[2] = {0, {3.1}};
        constants[3] = {0, {10000000.0}};


        // FunctionProto proto = {2, 6};
        // functional_prototypes[1] = proto;

        ObjectPrototype protObj = {2, 1337};
        prototypes[1] = protObj;

        add({{IRCode::LOAD_CONST, 0, 0, 0}});
        add({{IRCode::LOAD_CONST, 1, 0, 1}});
        add({{IRCode::LOAD_CONST, 6, 0, 3}});
        add({{IRCode::ADD, 2,2,0}});
       // add({{IRCode::MINUS, 2,2,1}});
        add({{IRCode::GREATER_EQUAL, 5,2,6}});
        add({{IRCode::JUMP, 5,0,7}});
        add({{IRCode::JUMP_A, 0,0,9}});
        add({{IRCode::PRINT, 2}});
        add({{IRCode::END_OF_CODE}});

       // add({{IRCode::PRINT, 1}});

        add({{IRCode::JUMP_A,0,0,3}});
        add({{IRCode::END_OF_CODE}});
    }

    void cycle() {
        runns++;
        //uint32_t full = (a << 16) | (b << 8) | c;
        Instruction i = instructions[pc];
        switch (i.code.op) {
            case IRCode::NEW: {
                ObjectPrototype *proto = Register(i.code.A).value.as_prototype;
                auto *members = (Object *) malloc(proto->alloc_size * sizeof(Object));
                auto *obj = new ObjectTable{members, proto};
                object_pool.push_back(obj);
                Object object;
                object.type = proto.type;
                object.value.as_object = obj;
                Register(i.code.A) = object;
                pc++;
                break;
            }
            case IRCode::PRINT: {
                Object &a = Register(i.code.A);
                std::cout << "- Register " << (uint16_t) i.code.A << ": " << std::endl;
                std::cout << "   Type: " << a.type << std::endl;
                std::cout << "   Float: " << a.value.as_float << std::endl;
                std::stringstream stream;
                stream << std::hex << a.value.as_int;
                std::string result(stream.str());
                std::cout << "   Int: " << result << std::endl;
                pc++;
                break;
            }
            case IRCode::LOAD_CONST: {
                Register(i.code.A) = constants[((i.code.B << 8) | i.code.C)];
                pc++;
                break;
            }
            case IRCode::LOAD_MEMBER: {
                Register(i.code.A) = Register(i.code.B).value.as_object->members[i.code.C];
                pc++;
                break;
            }
            case IRCode::SET_MEMBER: {
                Register(i.code.A).value.as_object->members[i.code.B] = Register(i.code.C);
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
                Push(pc + 1);
                activation_record_pointer += active_record_size;
                Push(active_record_size);
                FunctionPrototype *proto = Register(i.code.A).value.as_function;
                active_record_size = proto->registers;
                pc = proto->instruction_location;
                break;
            }
            case IRCode::RETURN: {
                Pop(active_record_size);
                activation_record_pointer -= active_record_size;
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
                dest.value.as_float = std::pow(x.value.as_float, y.value.as_float);
                pc++;
                break;
            }
            case IRCode::MOD: {
                Object &x = Register(i.code.B);
                Object &y = Register(i.code.C);
                Object &dest = Register(i.code.A);
                dest.value.as_float = std::fmod(x.value.as_float, y.value.as_float);
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
            }
            case IRCode::END_OF_CODE: {
                isRunning = false;
                break;
            }
            default: {
                std::cout << "Unknown command: " << i.code.op << std::endl;
                break;
            }
        }
//       std::cout << "Program counter at " << pc << std::endl;
    }


    void run() {
        isRunning = true;
        auto t1 = std::chrono::high_resolution_clock::now();
        while (isRunning) {
            cycle();
        }
        auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms_double = t2 - t1;
        std::cout << "######### End Of Program #########" << std::endl;
        std::cout << "took " << ms_double.count() << " ms" << std::endl;
        std::cout << "deleted " << object_pool.size() << " objects" << std::endl;
        for (const auto &item: object_pool) {
            delete item;
        }
    }

}

#endif //DATA_PROCESSING_INTERMEDIATE_VM_H
