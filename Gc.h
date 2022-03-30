//
// Created by Mo on 30.03.2022.
//

#ifndef DATA_PROCESSING_INTERMEDIATE_GC_H
#define DATA_PROCESSING_INTERMEDIATE_GC_H

#include <cstdint>
#include <cmath>
#include <sstream>
#include <vector>
#include <set>
#include "Vm.h"

std::set<Interpreter::ObjectTable *> found_objects;

void markObjects(Interpreter::ObjectTable *obj) {
    for (int i = 0; i < obj->prototype->members; i++) {
        Interpreter::Object &child = obj->members[i];
        if (child.type > 5) {
            found_objects.insert(child.value.as_pointer);
            markObjects(child.value.as_pointer);
        }
    }
}

void run(Interpreter::ObjectTable *entry, std::vector<Interpreter::ObjectTable *> *objs) {
    found_objects.clear();
    found_objects.insert(entry);
    markObjects(entry);
    uint64_t size = objs->size();
    for (int i = 0; i < size; i++) {
        Interpreter::ObjectTable *&item = objs->at(i);
        if (!found_objects.contains(item)) {
            delete item;
        }
    }
    objs->erase(std::remove_if(objs->begin(), objs->end(), [](Interpreter::ObjectTable *obj) {
        return found_objects.contains(obj);
    }), objs->end());
}

#endif //DATA_PROCESSING_INTERMEDIATE_GC_H
