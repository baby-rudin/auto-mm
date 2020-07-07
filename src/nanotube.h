#ifndef NANOTUBE_H
#define NANOTUBE_H

#include "type.hpp"
#include "vector.h"
#include "molecule.h"

Molecule    generate_nanotube(INTG n, INTG m, REAL l, REAL cc = 1.418, bool ma = true);

class Nanotube: public Molecule {

public:
    INTG    n;
    INTG    m;
    REAL    l;
    REAL    radius;

    Nanotube(void);
    Nanotube(const Nanotube &cnt);
    Nanotube(INTG n, INTG m, REAL l, REAL cc = 1.418);

    
    REAL            cnt_radius();           // origin cnts can be calculated
    size_t          find_best_id();         // i'll tell you the meaning of best
    void            move_to_center();
    void            delete_one_atom();      // use for centrosymmetric cnts
    void            delete_two_atom();      // use for centrosymmetric cnts
    void            delete_three_atom();    // use for centrosymmetric cnts
    void            add_one_atom();         // use for centrosymmetric cnts
    

};

#endif  // NANOTUBE_H
