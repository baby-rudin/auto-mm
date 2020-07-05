#include <iostream>
#include <fstream>
#include <stdio.h>

#include "type.hpp"
#include "vector.h"
#include "atom.h"
#include "molecule.h"
#include "random.h"
#include "lammps.h"
#include "constant.h"
#include "nanotube.h"
#include "stropt.h"
#include "varsys.h"
#include "funsys.h"
#include "calcval.h"

using namespace std;

int main(int argc, char* argv[])
{
    Molecule    fragment;
    Molecule    modeling;

    string line;

    while (getline(cin, line)) {
        
        clean_line(line);

        if (line.empty())   continue;

        // define variable or macro
        if (is_define_line(line)) {
            add_var_macro(line);
        }

        // nanotube
        else if (line.find(string("nanotube")) == 0) {
            auto nml = get_function_params(line);

            if (nml.size() != 3) {
                cout << "error nanotube parameters: " << line << endl;
                exit(-1);
            }

            INTG    n = calc_intg_value(nml[0]);
            INTG    m = calc_intg_value(nml[1]);
            REAL    l = calc_real_value(nml[2]);

            fragment = generate_nanotube(n, m, l);
        }

        // add
        else if (line.find(string("add")) == 0) {
            modeling += fragment;
        }

        // move
        else if (line.find(string("move")) == 0) {
            auto xyz = get_function_params(line);

            if (xyz.size() != 3) {
                cout << "error move parameters: " << line << endl;
                exit(-1);
            }

            REAL    mx = calc_real_value(xyz[0]);
            REAL    my = calc_real_value(xyz[1]);
            REAL    mz = calc_real_value(xyz[2]);

            fragment.translate(VecReal(mx, my, mz));
        }

        // mirror
        else if (line.find(string("mirror")) == 0) {
            auto xyza = get_function_params(line);

            if (xyza.size() != 4) {
                cout << "error mirror parameters: " << line << endl;
                exit(-1);
            }

            REAL    vx = calc_real_value(xyza[0]);
            REAL    vy = calc_real_value(xyza[1]);
            REAL    vz = calc_real_value(xyza[2]);
            REAL    a  = calc_real_value(xyza[3]);

            fragment.mirrorsym(VecReal(vx, vy, vz), a);
        }

        // create
        else if (line.find(string("create")) == 0) {
            line.erase(0, 6);
            auto files = str_split(line, " ");

            if (files.empty()) {
                cout << "create: input filename" << endl;
                exit(-1);
            }

            for (INTG i = 0; i < files.size(); i++) {
                ofstream lmps_data_file(files[i]);
                lmps_data_file << print_lmp_data(modeling) << endl;
                lmps_data_file.close();
            }
        }
        
        // other case
        else {
            cout << "error command: " << line << endl;
            exit(-1);
        }
    }


    return 0;
}
