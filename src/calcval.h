#ifndef CALCVAL_H
#define CALCVAL_H

#include "type.hpp"
#include <string>

// calculate value of a expression
INTG    calc_intg_value(std::string expression);
REAL    calc_real_value(std::string expression);

// compare operator
INTG    opt_cmp(std::string opt1, std::string opt2);

// judge whether the token is an expression
bool    is_intg_number(std::string token);
bool    is_real_number(std::string token);
bool    is_number(std::string token);

bool    is_math_operator(std::string str);

bool    is_math_expression(std::string token);


INTG    math_calc(INTG a, INTG b, std::string opt);
REAL    math_calc(REAL a, REAL b, std::string opt);


#endif  // CALCVAL_H
