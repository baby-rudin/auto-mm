#ifndef RANDOM_H
#define RANDOM_H

#include "type.hpp"
#include "vector.h"

// Uniform distributions
REAL        random_uniform_real(REAL a, REAL b);
INTG        random_uniform_intg(INTG a, INTG b);
VecReal     random_uniform_real(VecReal a, VecReal b);
VecIntg     random_uniform_intg(VecIntg a, VecIntg b);

#endif // RANDOM_H