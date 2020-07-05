#include "varsys.h"
#include "type.hpp"
#include "stropt.h"
#include "calcval.h"

#include <string>
#include <vector>
#include <map>
#include <regex>
#include <iostream>


using namespace std;


map<string, INTG>       intg_var_tab;
map<string, REAL>       real_var_tab;
map<string, string>     macro_tab;


bool    is_intg_name(string name)
{
    bool    is_name = false;

    for (auto &v : intg_var_tab) {
        if (v.first == name) {
            is_name = true;
            break;
        }
    }

    return is_name;
}

bool    is_real_name(string name)
{
    bool    is_name = false;

    for (auto &v : real_var_tab) {
        if (v.first == name) {
            is_name = true;
            break;
        }
    }

    return is_name;
}

bool    is_macro_name(string name)
{
    bool    is_name = false;

    for (auto &v : macro_tab) {
        if (v.first == name) {
            is_name = true;
            break;
        }
    }

    return is_name;
}

bool    is_var_macro(string name)
{ return is_intg_name(name) || is_real_name(name) || is_macro_name(name); }


bool    is_define_line(string line)
{
    string maybe_whitespace_regex   ("\\s*");
    string must_whitespace_regex    ("\\s+");
    string variable_type_regex      ("((intg)|(real)|(macro))");
    string variable_name_regex      ("[A-Za-z_]\\w*");
    string variable_value_regex     ("(\\S+\\s*)+");

    // regex of a define line
    string def_line_regex =   maybe_whitespace_regex
                            + variable_type_regex
                            + must_whitespace_regex
                            + variable_name_regex
                            + maybe_whitespace_regex
                            + string("=")
                            + maybe_whitespace_regex
                            + variable_value_regex
                            + maybe_whitespace_regex;

    return  regex_match(line, regex(def_line_regex));
}


void    add_var_macro(string def_line)
{
    string line(def_line);

    clean_line(line);
    string variable_type = line.substr(0, line.find_first_of(" "));

    line.erase(0, line.find_first_of(" "));
    auto key_val = str_split(line, string("="));

    if (key_val.size() != 2) {
        cout << "error definition: " << def_line << endl;
        exit(-1);
    }

    string key = key_val[0];    clean_line(key);
    string val = key_val[1];    clean_line(val);

    if (is_var_macro(key)) {
        cout << "error redefinition: " << key << endl;
        exit(-1);
    }

    if (variable_type == string("intg"))        add_intg_var(key, val);
    else if(variable_type == string("real"))    add_real_var(key, val);
    else if(variable_type == string("macro"))   add_macro(key, val);
    else {
        cout << "error definition type: " << variable_type << endl;
        exit(-1);
    }

}


void    add_intg_var(string var_name, string var_value)
{
    INTG    intg_value = calc_intg_value(var_value);
    intg_var_tab.insert(pair<string, INTG>(var_name, intg_value));
}

void    add_real_var(string var_name, string var_value)
{
    REAL    real_value = calc_real_value(var_value);
    real_var_tab.insert(pair<string, REAL>(var_name, real_value));
}

void    add_macro(string macro_name, string macro_value)
{
    macro_tab.insert(pair<string, string>(macro_name, macro_value));
}

