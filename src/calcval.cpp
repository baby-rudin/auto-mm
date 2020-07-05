#include "calcval.h"
#include "type.hpp"
#include "varsys.h"
#include "funsys.h"
#include "stropt.h"
#include <string>
#include <sstream>
#include <regex>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

INTG    calc_intg_value(string expression)
{
    INTG    value;

    remove_whitespace(expression);

    // if the expression is a name of a variable or a macro
    if (is_var_macro(expression)) {

        // cout << "variable name: " << expression << endl;

        if (is_intg_name(expression))     value = intg_var_tab[expression];
        if (is_real_name(expression))     value = real_var_tab[expression];
        if (is_macro_name(expression))    value = calc_intg_value(macro_tab[expression]);
    }

    // if the expression is calling a function
    else if (is_function_call(expression)) {

        // cout << "function call: " << expression << endl;

        string function_name = expression.substr(0, expression.find_first_of("("));
        remove_whitespace(function_name);

        if (function_name == string("randintg"))        value = randintg(expression);
        else if (function_name == string("randreal"))   value = randreal(expression);
        else if (function_name == string("powintg"))    value = powintg(expression);
        else if (function_name == string("powreal"))    value = powreal(expression);
        else {
            cout << "Unknow function name: " << function_name << endl;
            exit(-1);
        }
    }

    // if the expression is a number
    else if (is_number(expression)) {

        // cout << "number: " << expression << endl;

        stringstream ss(expression);
        ss >> value;
    }

    // if the expression is a mathematical expression
    else if (is_math_expression(expression)) {

        // cout << "math expression: " << endl;

        if (expression[0] == '+')       value = 0 + calc_intg_value(expression.substr(1,expression.size() - 1));
        else if (expression[0] == '-')  value = 0 - calc_intg_value(expression.substr(1,expression.size() - 1));
        else {

            vector<INTG>    calc_num;
            vector<string>  calc_opt;

            INTG nKet   = 0;
            INTG left   = 0;
            INTG right  = 0;

            while (right <= expression.size()) {
                if (expression[right] == '(')   nKet++;
                if (expression[right] == ')')   nKet--;

                if ( is_math_operator(expression.substr(right, 1))  && nKet == 0 ) {
                    
                    // calculate num
                    string tmp_num = expression.substr(left, right - left);
                    string tmp_opt = expression.substr(right, 1);

                    remove_whitespace(tmp_num);
                    if (tmp_num[0] == '(' && tmp_num[tmp_num.size()-1] == ')')
                        tmp_num = tmp_num.substr(1, tmp_num.size()-2);
                    
                    // calculate the value of the number
                    INTG    tmp_val = calc_intg_value(tmp_num);

                    // TODO
                    calc_num.push_back(tmp_val);

                    if (calc_opt.empty() || opt_cmp(tmp_opt, calc_opt.back()) > 0) {
                        calc_opt.push_back(tmp_opt);
                    }
                    else {
                        while (!calc_opt.empty() && opt_cmp(tmp_opt, calc_opt.back()) <= 0) {
                            INTG    b = calc_num.back();    calc_num.pop_back();
                            INTG    a = calc_num.back();    calc_num.pop_back();
                            string  o = calc_opt.back();    calc_opt.pop_back();
                            calc_num.push_back(math_calc(a, b, o));
                        }
                        calc_opt.push_back(tmp_opt);
                    }
                    left = right + 1;
                }

                if (nKet < 0) {
                    cout << "error expression: " << expression << endl;
                    exit(-1);
                }

                if ( right == expression.size() ) {
                    string tmp_num = expression.substr(left, right - left);
                    remove_whitespace(tmp_num);
                    if (tmp_num[0] == '(' && tmp_num[tmp_num.size()-1] == ')')
                        tmp_num = tmp_num.substr(1, tmp_num.size()-2);

                    calc_num.push_back(calc_real_value(tmp_num));
                }

                right ++;
            }   // end while

            if (nKet != 0) {
                cout << "error expression: " << expression << endl;
                exit(-1);
            }

            while (!calc_opt.empty()) {
                INTG    b = calc_num.back();    calc_num.pop_back();
                INTG    a = calc_num.back();    calc_num.pop_back();
                string  o = calc_opt.back();    calc_opt.pop_back();
                calc_num.push_back(math_calc(a, b, o));
            }

            if (calc_num.size() != 1) {
                cout << "error math calculate: " << expression << endl;
                exit(-1);
            }

            value = calc_num[0];
        }
    }

    // error case
    else {
        cout << "can not calculate the value of the expression: " << expression << endl;
        exit(-1);
    }

    return value;
}


REAL    calc_real_value(string expression)
{
    REAL    value;

    remove_whitespace(expression);

    // if the expression is a name of a variable or a macro
    if (is_var_macro(expression)) {


        // cout << "variable name: " << expression << endl;


        if (is_intg_name(expression))     value = intg_var_tab[expression];
        if (is_real_name(expression))     value = real_var_tab[expression];
        if (is_macro_name(expression))    value = calc_real_value(macro_tab[expression]);
    }

    // if the expression is calling a function
    else if (is_function_call(expression)) {

        // cout << "function call: " << expression << endl;

        string function_name = expression.substr(0, expression.find_first_of("("));
        remove_whitespace(function_name);

        if (function_name == string("randintg"))        value = randintg(expression);
        else if (function_name == string("randreal"))   value = randreal(expression);
        else if (function_name == string("powintg"))    value = powintg(expression);
        else if (function_name == string("powreal"))    value = powreal(expression);
        else {
            cout << "Unknow function name: " << function_name << endl;
            exit(-1);
        }
    }

    // if the expression is a number
    else if (is_number(expression)) {

        // cout << "number: " << expression << endl;

        stringstream ss(expression);
        ss >> value;
    }

    // if the expression is a mathematical expression
    else if (is_math_expression(expression)) {

        // cout << "math expression: " << expression << endl;

        if (expression[0] == '+')       value = 0 + calc_real_value(expression.substr(1,expression.size() - 1));
        else if (expression[0] == '-')  value = 0 - calc_real_value(expression.substr(1,expression.size() - 1));
        else {

            vector<REAL>    calc_num;
            vector<string>  calc_opt;

            INTG nKet   = 0;
            INTG left   = 0;
            INTG right  = 0;

            while (right <= expression.size()) {
                if (expression[right] == '(')   nKet++;
                if (expression[right] == ')')   nKet--;

                if ( is_math_operator(expression.substr(right, 1))  && nKet == 0 ) {
                    
                    // calculate num
                    string tmp_num = expression.substr(left, right - left);
                    string tmp_opt = expression.substr(right, 1);

                    remove_whitespace(tmp_num);
                    if (tmp_num[0] == '(' && tmp_num[tmp_num.size()-1] == ')')
                        tmp_num = tmp_num.substr(1, tmp_num.size()-2);
                    
                    // calculate the value of the number
                    REAL    tmp_val = calc_real_value(tmp_num);

                    // TODO
                    calc_num.push_back(tmp_val);

                    if (calc_opt.empty()  || opt_cmp(tmp_opt, calc_opt.back()) > 0) {
                        calc_opt.push_back(tmp_opt);
                    }
                    else {
                        while (!calc_opt.empty() && opt_cmp(tmp_opt, calc_opt.back()) <= 0) {
                            REAL    b = calc_num.back();    calc_num.pop_back();
                            REAL    a = calc_num.back();    calc_num.pop_back();
                            string  o = calc_opt.back();    calc_opt.pop_back();
                            calc_num.push_back(math_calc(a, b, o));
                        }
                        calc_opt.push_back(tmp_opt);
                    }

                    left = right + 1;
                }

                if (nKet < 0) {
                    cout << "error expression: " << expression << endl;
                    exit(-1);
                }

                if ( right == expression.size() ) {
                    string tmp_num = expression.substr(left, right - left);
                    remove_whitespace(tmp_num);
                    if (tmp_num[0] == '(' && tmp_num[tmp_num.size()-1] == ')')
                        tmp_num = tmp_num.substr(1, tmp_num.size()-2);

                    calc_num.push_back(calc_real_value(tmp_num));
                }

                right ++;
            }   // end while

            if (nKet != 0) {
                cout << "error expression: " << expression << endl;
                exit(-1);
            }

            while (!calc_opt.empty()) {
                REAL    b = calc_num.back();    calc_num.pop_back();
                REAL    a = calc_num.back();    calc_num.pop_back();
                string  o = calc_opt.back();    calc_opt.pop_back();
                calc_num.push_back(math_calc(a, b, o));
            }

            if (calc_num.size() != 1) {
                cout << "error math calculate: " << expression << endl;
                exit(-1);
            }

            value = calc_num[0];
        }
    }

    // error case
    else {
        cout << "can not calculate the value of the expression: " << expression << endl;
        exit(-1);
    }

    // cout << value << endl;

    return value;
}


INTG    opt_cmp(string opt1, string opt2)
{
    INTG    cmp = 0;

    if (   (opt1 == string("+") || opt1 == string("-"))
        && (opt2 == string("*") || opt2 == string("/")) )
        cmp = -1;

    if (   (opt1 == string("*") || opt1 == string("/"))
        && (opt2 == string("+") || opt2 == string("-")) )
        cmp = 1;

    return cmp;
}


bool    is_intg_number(string token)
{
    string intg_number_regex("\\s*(\\+|-)?\\s*\\d+\\s*");
    return regex_match(token, regex(intg_number_regex));
}

bool    is_real_number(std::string token)
{
    string real_number_regex("\\s*(\\+|-)?\\s*\\d+\\.?\\d*\\s*");
    return regex_match(token, regex(real_number_regex));
}

bool    is_number(std::string token)
{ return is_intg_number(token) || is_real_number(token); }


bool    is_math_operator(std::string str){
    return  str == string("+") ||
            str == string("-") ||
            str == string("*") ||
            str == string("/");
}


// TODO ================================================================================
bool    is_math_expression(std::string token)
{
    return true;
}


INTG    math_calc(INTG a, INTG b, std::string opt)
{
    INTG    value = 0;

    if (b == 0 && opt == string("/")) {
        cout << "error divide by zero" << endl;
        exit(-1);
    }

    if (opt == string("+"))         value = a + b;
    else if (opt == string("-"))    value = a - b;
    else if (opt == string("*"))    value = a * b;
    else if (opt == string("/"))    value = a / b;
    else {
        cout << "error math operator: " << opt << endl;
        exit(-1);
    }

    return value;
}

REAL    math_calc(REAL a, REAL b, std::string opt)
{
    REAL    value = 0;

    if (fabs(b) < 1e-9 && opt == string("/")) {
        cout << "error divide by zero" << endl;
        exit(-1);
    }

    if (opt == string("+"))         value = a + b;
    else if (opt == string("-"))    value = a - b;
    else if (opt == string("*"))    value = a * b;
    else if (opt == string("/"))    value = a / b;
    else {
        cout << "error math operator: " << opt << endl;
        exit(-1);
    }

    return value;
}