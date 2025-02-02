#ifndef EXPR_ENUMS_H
#define EXPR_ENUMS_H

typedef enum{
    MATH_CPP_INT,       // Tokens with integer values
    MATH_CPP_DOUBLE,    // Tokens with floating point values
    MATH_CPP_VARIABLE,  // Tokens with alphanumeric values
    MATH_CPP_IPV4,      // Recognizes IPV4 addresses
    MATH_CPP_DTYPE_MAX
}exprcpp_dtypes_t;

typedef enum{
    PARSERL_EOL = (int) MATH_CPP_DTYPE_MAX + 1
}EXTRA_ENUMS;

#endif 