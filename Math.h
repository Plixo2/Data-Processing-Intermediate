//
// Created by Mo on 28.03.2022.
//

#ifndef DATA_PROCESSING_INTERMEDIATE_MATH_H
#define DATA_PROCESSING_INTERMEDIATE_MATH_H


#include <cmath>
#include <limits>
#include <cstdint>

#define dpi_float double
#define dpi_int int64_t

const double epsilon = std::numeric_limits<double>::epsilon();

dpi_float math_abs(dpi_float num) {
    return std::fabs(num);
}
dpi_float math_isequals(dpi_float a , dpi_float b) {
    return a == b;
   // return math_abs(a-b) > epsilon;
}

#endif //DATA_PROCESSING_INTERMEDIATE_MATH_H
