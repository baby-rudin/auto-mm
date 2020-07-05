#ifndef VARSYS_H
#define VARSYS_H

#include "type.hpp"

#include <string>
#include <vector>
#include <map>


extern  std::map<std::string, INTG>          intg_var_tab;
extern  std::map<std::string, REAL>          real_var_tab;
extern  std::map<std::string, std::string>   macro_tab;

bool    is_intg_name(std::string name);
bool    is_real_name(std::string name);
bool    is_macro_name(std::string name);
bool    is_var_macro(std::string name);

bool    is_define_line(std::string line);       // judge whether the line is a define line
void    add_var_macro(std::string def_line);    // deal with a define line
void    add_intg_var(std::string var_name, std::string var_value);
void    add_real_var(std::string var_name, std::string var_value);
void    add_macro(std::string macro_name, std::string macro_value);



#endif  // VARSYS_H