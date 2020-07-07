#include "lammps.h"
#include "type.hpp"
#include "constant.h"
#include <string>
#include <cmath>

#define BUFF_LEN 1024

using namespace std;

string  print_lmp_data(Molecule mol)
{
    // set box boundary
    VecReal MinVec = mol.get_min_vector();
    VecReal MaxVec = mol.get_max_vector();

    REAL enlarge_factor = 0.1;

    MinVec -= (MaxVec - MinVec) * enlarge_factor;
    MaxVec += (MaxVec - MinVec) * enlarge_factor;


    string  lmps_data;
    char    buff[BUFF_LEN];
    INTG    atom_type   = 1;
    INTG    mol_type    = 0;

    // lammps data file's head information
    lmps_data += string("# lammps data file\n\n");
    lmps_data += to_string(mol.nAtom) + string(" atoms\n\n");
    lmps_data += string("1 atom types\n\n");

    sprintf(buff, "%10.2lf  %10.2lf          xlo xhi\n", MinVec.x, MaxVec.x);
    lmps_data += string(buff);
    sprintf(buff, "%10.2lf  %10.2lf          ylo yhi\n", MinVec.y, MaxVec.y);
    lmps_data += string(buff);
    sprintf(buff, "%10.2lf  %10.2lf          zlo zhi\n", MinVec.z, MaxVec.z);
    lmps_data += string(buff);
    lmps_data += string("\n");

    lmps_data += string("Masses\n\n");
    lmps_data += string("1 12.01   # C\n\n");
    lmps_data += string("Atoms\n\n");

    // add coordinates information
    for (size_t i = 0; i < mol.nAtom; i++) {
        sprintf(buff, " %4d %2d %2d %14.8lf %14.8lf %14.8lf",
                INTG(i+1), mol_type, atom_type,
                mol.geom[i].x, mol.geom[i].y, mol.geom[i].z);
        lmps_data += string(buff) + string("\n");
    }

    return  lmps_data;
}


string  print_in(Molecule calc_sys, REAL R, VecReal vec)
{
    REAL    rMax = 0.0;

    for (size_t i = 0; i < calc_sys.nAtom; i++) {
        REAL rTmp;
        rTmp = sqrt(  pow(calc_sys.geom[i].y, 2.0)
                    + pow(calc_sys.geom[i].z, 2.0) );
        
        if (i == 0 || rTmp > rMax)  rMax = rTmp;
    }

    string      lmp_in;
    char        buff[BUFF_LEN];
    VecReal     vMin = -0.5 * vec;
    VecReal     vMax =  0.5 * vec;

    lmp_in += string("# cnt simulation\n\n");
    lmp_in += string("echo        screen\n\n");
    lmp_in += string("units               metal\n");
    lmp_in += string("dimension           3\n");
    lmp_in += string("boundary            s s s\n");
    lmp_in += string("atom_style          atomic\n\n");
    lmp_in += string("# Read Data\n");
    lmp_in += string("read_data           cnt.data\n\n");
    lmp_in += string("# Force field\n");
    lmp_in += string("pair_style          tersoff\n");
    lmp_in += string("pair_coeff          * * SiC.tersoff C\n\n");

    sprintf(buff, "%6.2lf", rMax * 0.85);
    lmp_in += string("region      outer   cylinder x 0 0 ");
    lmp_in += string(buff);
    lmp_in += string(" EDGE EDGE  side out  units box\n");

    lmp_in += string("group       outers  region outer\n");
    lmp_in += string("velocity    outers  set 0.0 0.0 0.0\n");
    lmp_in += string("fix         force   outers setforce 0.0 0.0 0.0\n\n");

    sprintf(buff, "%6.2lf %6.2lf %6.2lf %6.2lf %6.2lf %6.2lf",
            vMin.x, vMax.x, vMin.y, vMax.y, vMin.z, vMax.z);
    lmp_in += string("region      carbon  block ");
    lmp_in += string(buff);
    lmp_in += string("  units box\n");

    lmp_in += string("group       carbons region carbon\n");

    sprintf(buff, "%6.2lf", R * M_SQRT2);
    lmp_in += string("region      cylin   cylinder x 0 0 ");
    lmp_in += string(buff);
    lmp_in += string(" EDGE EDGE units box\n");

    lmp_in += string("fix wall carbons wall/region cylin harmonic 1.0 0.0 2.5\n\n");
    


    lmp_in += string("#Output\n");
    lmp_in += string("thermo              1000\n");
    lmp_in += string("dump                xyz all xyz 1000 cnt.xyz\n\n");
    lmp_in += string("#Simulation setup\n");
    lmp_in += string("timestep            1.0e-05\n\n");
    lmp_in += string("#Nose-Hoover thermostat\n");
    lmp_in += string("fix                 nvt all nvt temp 300 2000 0.001\n");
    lmp_in += string("run                 5000000\n");

    return lmp_in;
}


string  print_in2(VecReal vMin, VecReal vMax)
{
    string      lmp_in;
    char        buff[BUFF_LEN];

    lmp_in += string("units           metal\n");
    lmp_in += string("dimension       3\n");
    lmp_in += string("boundary        f f f\n");
    lmp_in += string("atom_style      molecular\n\n");
    lmp_in += string("read_data       cnts.data\n\n");
    lmp_in += string("region          r_fix    block      INF     INF         INF     0.0        INF     INF         units box\n");
    lmp_in += string("region          r_adatom block      ");

    sprintf(buff, "%6.2lf %6.2lf", vMin.x, vMax.x);
    lmp_in += string(buff);
    lmp_in += string("       30      50          ");
    sprintf(buff, "%6.2lf %6.2lf", vMin.z, vMax.z);
    lmp_in += string(buff);
    lmp_in += string("      units box\n\n");

    lmp_in += string("group           g_depo  type    1\n");
    lmp_in += string("group           g_fix   region  r_fix\n");
    lmp_in += string("group           g_fix   region  r_fix\n");
    lmp_in += string("group           Film    union   g_depo\n\n");
    lmp_in += string("mass            1   12\n\n");
    lmp_in += string("pair_style      tersoff\n");
    lmp_in += string("pair_coeff      * * SiC.tersoff C\n\n");
    lmp_in += string("compute         CNT_temp Film temp\n");
    lmp_in += string("compute_modify  CNT_temp dynamic yes\n\n");
    lmp_in += string("velocity        all create 300 102486 mom yes rot yes dist gaussian\n");
    lmp_in += string("fix             1 all nvt temp 1498 1498 0.1 drag 0.5\n\n");
    lmp_in += string("fix             2 g_fix setforce 0.0 0.0 0.0\n");
    lmp_in += string("velocity        all  set 0 0 0 units box\n");
    lmp_in += string("fix             5 g_depo deposit 8000 1 500 343443 region r_adatom units box vy -1 -1\n");
    lmp_in += string("timestep        0.001\n");
    lmp_in += string("thermo          100000\n\n");
    lmp_in += string("thermo_modify   temp CNT_temp lost ignore norm yes\n");
    lmp_in += string("dump            1 all custom 40000 1.lammpstrj id type mass x y z\n");
    lmp_in += string("run             6000000\n\n");
    lmp_in += string("unfix           5\n");

    return lmp_in;
}


string  print_pair_coeff()
{
    string  pair_coeff;

    pair_coeff += string("C   C    C   3.0 1.0 0.0 38049  4.3484   -.57058 .72751\n");
    pair_coeff += string("             0.00000015724 2.2119  346.7   1.95   0.15   3.4879  1393.6\n");
    pair_coeff += string("\n");
    pair_coeff += string("Si  Si  Si  3.0 1.0 0.0  100390  16.217   -.59825 .78734\n");
    pair_coeff += string("            0.0000011     1.73222  471.18  2.85   0.15    2.4799  1830.8\n");
    pair_coeff += string("\n");
    pair_coeff += string("Si  Si  C   3.0 1.0 0.0 100390  16.217   -.59825 0.0\n");
    pair_coeff += string("            0.0 0.0 0.0 2.36   0.15 0.0 0.0\n");
    pair_coeff += string("\n");
    pair_coeff += string("Si  C   C   3.0 1.0 0.0 100390 16.217 -.59825 .787340\n");
    pair_coeff += string("            0.0000011     1.97205 395.126  2.36  0.15    2.9839  1597.3111\n");
    pair_coeff += string("\n");
    pair_coeff += string("C   Si  Si  3.0 1.0 0.0 38049  4.3484  -.57058 .72751\n");
    pair_coeff += string("            0.00000015724 1.97205 395.126  2.36  0.15   2.9839   1597.3111\n");
    pair_coeff += string("\n");
    pair_coeff += string("C   Si  C   3.0 1.0 0.0 38049  4.3484   -.57058 0.0\n");
    pair_coeff += string("            0.0 0.0 0.0 1.95   0.15 0.0 0.0\n");
    pair_coeff += string("\n");
    pair_coeff += string("C   C   Si  3.0 1.0 0.0 38049  4.3484   -.57058 0.0\n");
    pair_coeff += string("            0.0 0.0 0.0 2.36   0.15 0.0 0.0\n");
    pair_coeff += string("\n");
    pair_coeff += string("Si  C   Si  3.0 1.0 0.0 100390 16.217 -.59825 0.0\n");
    pair_coeff += string("            0.0 0.0 0.0 2.85   0.15 0.0 0.0\n");
    pair_coeff += string("\n");

    return pair_coeff;
}
