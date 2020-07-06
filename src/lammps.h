#ifndef LAMMPS_H
#define LAMMPS_H

#include "type.hpp"
#include "molecule.h"
#include "vector.h"

#include <string>

std::string     print_lmp_data(Molecule mol);

std::string     print_in (Molecule calc_sys, REAL R, VecReal vec);
std::string     print_in2(VecReal vMin, VecReal vMax);
std::string     print_pair_coeff();

#endif  // LAAMMPS_H
