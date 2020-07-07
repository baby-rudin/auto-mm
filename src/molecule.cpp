#include "molecule.h"
#include "constant.h"
#include "atom.h"
#include "random.h"
#include <string>
#include <cmath>
#include <vector>


#define BUFF_LEN 1024

using namespace std;

//==================== Molecule ====================
Molecule::Molecule(void)
    : nAtom(0), charge(0)
{}

Molecule::Molecule(const Molecule &mol)
    : nAtom(mol.nAtom), charge(mol.charge), 
      zval(mol.zval), geom(mol.geom)
{}


// functions of Molecule
string  Molecule::print_geom() const
{
    string  geometry;
    char buff[BUFF_LEN];

    for (size_t i = 0; i < nAtom; i++) {
        sprintf(buff, " %-2s %14.8lf %14.8lf %14.8lf",
                get_atom_name(zval[i]).c_str(), geom[i].x, geom[i].y, geom[i].z);
        geometry += string(buff) + string("\n");
    }
    return  geometry;
}


REAL    Molecule::bond(size_t a, size_t b) const
{
    return (geom[a] - geom[b]).len();
}

REAL  Molecule::angle(size_t a, size_t b, size_t c) const
{
    VecReal b_a = geom[b] - geom[a];
    VecReal b_c = geom[b] - geom[c];
    return  acos((b_a * b_c) / (b_a.len() * b_c.len()));
}

VecReal Molecule::get_min_vector()
{
    VecReal MinVec;

    for (size_t i = 0; i < nAtom; i++) {
        if (i == 0) MinVec = geom[i];
        else {
            MinVec.x = (geom[i].x < MinVec.x ? geom[i].x : MinVec.x);
            MinVec.y = (geom[i].y < MinVec.y ? geom[i].y : MinVec.y);
            MinVec.z = (geom[i].z < MinVec.z ? geom[i].z : MinVec.z);
        }
    }

    return  MinVec;
}

VecReal Molecule::get_max_vector()
{
    VecReal MaxVec;

    for (size_t i = 0; i < nAtom; i++) {
        if (i == 0) MaxVec = geom[i];
        else {
            MaxVec.x = (geom[i].x > MaxVec.x ? geom[i].x : MaxVec.x);
            MaxVec.y = (geom[i].y > MaxVec.y ? geom[i].y : MaxVec.y);
            MaxVec.z = (geom[i].z > MaxVec.z ? geom[i].z : MaxVec.z);
        }
    }

    return  MaxVec;
}

VecReal Molecule::get_center_vector()
{
    return (get_max_vector() + get_min_vector()) / 2.0;
}

void    Molecule::translate(VecReal R)
{
    for (size_t i = 0; i < nAtom; i++)
        geom[i] += R;
}

void    Molecule::centrosym(VecReal R)
{
    for (size_t i = 0; i < nAtom; i++)
        geom[i] = 2.0 * R - geom[i];
}

// Ax + By + Cz + D = 0
void    Molecule::mirrorsym(VecReal N, REAL D)
{
    for (size_t i = 0; i < nAtom; i++) {
        double k = 2.0 * (geom[i]*N + D) / (N*N);
        geom[i] -= (k * N);
    }
}

// operator =
Molecule&   Molecule::operator= (const Molecule &mol)
{
    nAtom   = mol.nAtom;
    charge  = mol.charge;
    zval    = mol.zval;
    geom    = mol.geom;

    return *this;
}

// operator +=
Molecule&   Molecule::operator+=(const Molecule &mol)
{
    nAtom   += mol.nAtom;
    charge  += mol.charge;
    zval.insert(zval.end(), mol.zval.begin(), mol.zval.end());
    geom.insert(geom.end(), mol.geom.begin(), mol.geom.end());

    return *this;
}

// operator +
Molecule    operator+ (const Molecule &mol1, const Molecule &mol2)
{
    Molecule    mol(mol1);
    mol += mol2;

    return mol;
}



// just for graduation. it's too difficult

// generate random atoms
Molecule::Molecule(VecReal V, INTG NumAtom, REAL bond_len, INTG init_type)
    : nAtom(0), charge(0)
{
    // predict whether it's too dense or not
    if (V.x * V.y * V.z * 0.7 < pow(bond_len, 3.0) * NumAtom) {
        cout << "Can't generate random atom." << endl;
        return;
    }

    if (init_type == 0){
        for (INTG i = 0; i < NumAtom; i++) {
            VecReal coordinate;
            bool    is_accept;
    
            do {
                is_accept = true;
    
                // generate new coordinate
                VecReal GenAtom;
                GenAtom = random_uniform_real(-0.5 * V, 0.5 * V);

                REAL rTmp = sqrt( GenAtom.y*GenAtom.y + GenAtom.z*GenAtom.z );
                if ( rTmp > V.z / 2.0 ) is_accept = false;
    
                for (INTG j = 0; j < nAtom; j++) {
                    double distance;
                    distance = (geom[j]-GenAtom).len();
            
                    // too close
                    if (distance < bond_len) {
                        is_accept = false;
                        break;
                    }
                }
    
                // record coordinate
                if (is_accept)
                    coordinate = GenAtom;
    
            } while(is_accept != true);
    
            nAtom++;
            geom.push_back(coordinate);
            zval.push_back(6);
        }
    }
    
    if (init_type == 1) {
        ;
    }
}


void    Molecule::move_to_center()
{
    VecReal MaxVec = get_max_vector();
    VecReal MinVec = get_min_vector();

    translate(-0.5 * (MinVec + MaxVec));
}

size_t  Molecule::find_best_id()
{
    // find most low atoms;
    REAL            abs_high;
    vector<size_t>  id_record;
    for (size_t i = 0; i < nAtom; i++) {
        if (i == 0) {
            abs_high = abs(geom[i].z);
            id_record.push_back(i);
        }
        else {
            if (abs(geom[i].z) < abs_high) {
                abs_high = abs(geom[i].z);
                id_record.clear();
                id_record.push_back(i);
            }
            else if(abs(abs_high - abs(geom[i].z)) < 1e-5){
                id_record.push_back(i);
            }
        }
    }

    REAL    xMAx;
    size_t  best_id;
    for (auto i = id_record.begin(); i != id_record.end(); i++) {
        if (i == id_record.begin()) {
            xMAx = geom[*i].x;
            best_id = *i;
        }
        else {
            if (geom[*i].x > xMAx) {
                xMAx = geom[*i].x;
                best_id = *i;
            }
        }
    }

    return best_id;
}

void    Molecule::delete_one_atom()
{
    size_t id = find_best_id();
    zval.erase(zval.begin() + id);
    geom.erase(geom.begin() + id);
    nAtom --;
}

void    Molecule::delete_two_atom()
{
    size_t id = find_best_id();
    zval.erase(zval.begin() + id);
    geom.erase(geom.begin() + id);
    nAtom --;

    // delete one more
    id = find_best_id();
    zval.erase(zval.begin() + id);
    geom.erase(geom.begin() + id);
    nAtom --;
}

void    Molecule::delete_three_atom()
{
    size_t id = find_best_id();
    zval.erase(zval.begin() + id);
    geom.erase(geom.begin() + id);
    nAtom --;

    // delete two more
    for (INTG i = 0; i < 2; i++) {
        id = find_best_id();
        zval.erase(zval.begin() + id);
        geom.erase(geom.begin() + id);
        nAtom --;
    }
}

void    Molecule::add_one_atom()
{
    size_t  id1 = find_best_id();
    size_t  id2;
    REAL    min_bond = -1.0;

    for (size_t i = 0; i < nAtom; i++) {
        if (i == id1) continue;
        
        if (min_bond < 0.0 || bond(id1, i) < min_bond) {
            id2 = i;
            min_bond = bond(id1, i);
        }
    }

    VecReal coordinate = (geom[id1] + geom[id2]) / 2.0;
    coordinate.x += 1.212;

    geom.push_back(coordinate);
    zval.push_back(6);
    nAtom ++;
}


REAL    Molecule::cnt_radius()
{
    REAL    radius;

    for (size_t i = 0; i < nAtom; i++) {
        if (i == 0)
            radius = sqrt(geom[i].x * geom[i].x + geom[i].y + geom[i].y);
        else {
            REAL tmp_radius;
            tmp_radius = sqrt(geom[i].x * geom[i].x + geom[i].y + geom[i].y);
            if (tmp_radius > radius) radius = tmp_radius;
        }
    }

    return radius;
}
