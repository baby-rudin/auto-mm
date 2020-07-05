#ifndef FUNSYS_H
#define FUNSYS_H

#include <string>
#include <vector>
#include "type.hpp"

// a table records all function name
extern  std::vector<std::string>    function_name_table;

// judge whether the name is a function name
bool    is_function_name(std::string name);

// judge whether the token is using a function
bool    is_function_call(std::string token);

// get parameters of a function
std::vector<std::string>    get_function_params(std::string fun_call);


// get random number
INTG    randintg(INTG a, INTG b);
REAL    randreal(REAL a, REAL b);

// used by calcval.cpp
INTG    randintg(std::string expression);
REAL    randreal(std::string expression);

// pow
INTG    powintg(INTG a, INTG b);
REAL    powreal(REAL a, REAL b);

INTG    powintg(std::string expression);
REAL    powreal(std::string expression);

#endif  // FUNSYS_H