#include "random.h"
#include "type.hpp"
#include <random>

// Uniform distributions
REAL    random_uniform_real(REAL a, REAL b)
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(a, b);

    return dis(gen);
}

INTG    random_uniform_intg(INTG a, INTG b)
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(a, b);

    return dis(gen);
}

VecReal random_uniform_real(VecReal a, VecReal b)
{
    VecReal rand_vec;

    rand_vec.x = random_uniform_real(a.x, b.x);
    rand_vec.y = random_uniform_real(a.y, b.y);
    rand_vec.z = random_uniform_real(a.z, b.z);

    return rand_vec;
}

VecIntg random_uniform_intg(VecIntg a, VecIntg b)
{
    VecIntg rand_vec;

    rand_vec.x = random_uniform_intg(a.x, b.x);
    rand_vec.y = random_uniform_intg(a.y, b.y);
    rand_vec.z = random_uniform_intg(a.z, b.z);

    return rand_vec;
}