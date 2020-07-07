#include "funsys.h"
#include "stropt.h"
#include "random.h"
#include "calcval.h"
#include <string>
#include <vector>
#include <iostream>
#include <regex>
#include <cmath>

using namespace std;


vector<string>  function_name_table = { string("randintg"), string("randreal"),
                                        string("powintg"), string("powreal")       };


bool    is_function_name(string name)
{
    bool    is_fun = false;

    for (size_t i = 0; i < function_name_table.size(); i++) {
        if (name == function_name_table[i]) {
            is_fun = true;
            break;
        }
    }

    return is_fun;
}


bool    is_function_call(string token)
{
    string function_name_regex("(");
    for (size_t i = 0; i < function_name_table.size(); i++) {
        function_name_regex += string("(");
        function_name_regex += function_name_table[i];
        function_name_regex += string(")");
        function_name_regex += string("|");
    }

    if (function_name_table.size() > 0)
        function_name_regex.erase(function_name_regex.size() - 1);
    function_name_regex += string(")");

    string maybe_whitespace_regex   ("\\s*");
    string one_param_regex          ("\\S+");

    string function_call_regex;

    function_call_regex += maybe_whitespace_regex;
    function_call_regex += function_name_regex;
    function_call_regex += maybe_whitespace_regex;
    function_call_regex += string("\\(");

    function_call_regex += string("(");
    function_call_regex += string("(");
    function_call_regex += maybe_whitespace_regex;
    function_call_regex += one_param_regex;
    function_call_regex += maybe_whitespace_regex;
    function_call_regex += string(")+");
    function_call_regex += string(",");
    function_call_regex += string(")");
    function_call_regex += string("*");

    function_call_regex += maybe_whitespace_regex;
    function_call_regex += one_param_regex;
    function_call_regex += maybe_whitespace_regex;
    function_call_regex += string("\\)");
    function_call_regex += maybe_whitespace_regex;

    // cout << function_call_regex << endl;

    return regex_match(token, regex(function_call_regex));
}


vector<string>  get_function_params(string fun_call)
{
    vector<string>  params;

    remove_whitespace(fun_call);
    string fun_call_backup(fun_call);

    fun_call.erase(0, fun_call.find_first_of("(") + 1);
    fun_call.erase(fun_call.find_last_of(")"));
    remove_whitespace(fun_call);

    INTG nKet   = 0;
    INTG left   = 0;
    INTG right  = 0; 

    while ( right <= fun_call.size() ) {
        if (fun_call[right] == '(')  nKet++;
        if (fun_call[right] == ')')  nKet--;

        if ( fun_call[right] == ',' && nKet == 0 || right == fun_call.size() ) {
            string tmp = fun_call.substr(left, right-left);
            remove_whitespace(tmp);
            params.push_back(tmp);
            left = right + 1;
        }

        if (nKet < 0) {
            cout << "error parameter list: " << fun_call_backup << endl;
            exit(-1);
        }

        right++;
    }

    if (nKet != 0) {
        cout << "error parameter list: " << fun_call_backup << endl;
        exit(-1);
    }

    return params;
}


// get random number
INTG    randintg(INTG a, INTG b)
{ return random_uniform_intg(a, b); }


REAL    randreal(REAL a, REAL b)
{ return random_uniform_real(a, b); }


// used by calcsys.cpp
INTG    randintg(string expression)
{
    remove_whitespace(expression);
    auto params = get_function_params(expression);

    if (params.size() != 2) {
        cout << "error call function randintg: " << expression << endl;
        exit(-1);
    }
    
    INTG    a = calc_intg_value(params[0]);
    INTG    b = calc_intg_value(params[1]);

    return  randintg(a, b);
}

REAL    randreal(string expression)
{
    remove_whitespace(expression);
    auto params = get_function_params(expression);

    if (params.size() != 2) {
        cout << "error call function randreal: " << expression << endl;
        exit(-1);
    }
    
    REAL    a = calc_real_value(params[0]);
    REAL    b = calc_real_value(params[1]);

    return  randreal(a, b);
}


// pow
INTG    powintg(INTG a, INTG b)
{ return pow(a,b); }

REAL    powreal(REAL a, REAL b)
{ return pow(a,b); }


INTG    powintg(std::string expression)
{
    remove_whitespace(expression);
    auto params = get_function_params(expression);

    if (params.size() != 2) {
        cout << "error call function powintg: " << expression << endl;
        exit(-1);
    }
    
    INTG    a = calc_intg_value(params[0]);
    INTG    b = calc_intg_value(params[1]);

    return  powintg(a, b);
}

REAL    powreal(std::string expression)
{
    remove_whitespace(expression);
    auto params = get_function_params(expression);

    if (params.size() != 2) {
        cout << "error call function powreal: " << expression << endl;
        exit(-1);
    }
    
    REAL    a = calc_real_value(params[0]);
    REAL    b = calc_real_value(params[1]);

    return  powreal(a, b);
}
