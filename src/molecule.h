#ifndef MOLECULE_H
#define MOLECULE_H

#include "type.hpp"
#include "vector.h"
#include <string>
#include <vector>
#include <iostream>

class Molecule
{
public:
    size_t                  nAtom;
    INTG                    charge;
    std::vector<INTG>       zval;
    std::vector<VecReal>    geom;


    Molecule(void);
    Molecule(const Molecule &mol);

    Molecule&       operator= (const Molecule &mol);
    Molecule&       operator+=(const Molecule &mol);

    std::string     print_geom() const;
    std::string     print_lmps() const;     // just for graduation
    
    REAL            bond(size_t atom1, size_t atom2) const;
    REAL            angle(size_t atom1, size_t atom2, size_t atom3) const;
    VecReal         get_min_vector();
    VecReal         get_max_vector();
    VecReal         get_center_vector();

    void            translate(VecReal R);
    void            centrosym(VecReal R);
    void            mirrorsym(VecReal N, REAL D);  // Ax + By + Cz + D = 0
    void            rotatesym(VecReal A, VecReal B, REAL theta);    // TODO


    // just for graduation
    // it's too difficult. Is there anybody helping me?

    // generate random atoms
    Molecule(VecReal V, INTG NumAtom, REAL bond_len, INTG init_type);

    REAL            cnt_radius();           // origin cnts can be calculated
    size_t          find_best_id();         // i'll tell you the meaning of best
    void            move_to_center();
    void            delete_one_atom();      // use for centrosymmetric cnts
    void            delete_two_atom();      // use for centrosymmetric cnts
    void            delete_three_atom();    // use for centrosymmetric cnts
    void            add_one_atom();         // use for centrosymmetric cnts

};

Molecule    operator+ (const Molecule &mol1, const Molecule &mol2);

#endif // MOLECULE_H
