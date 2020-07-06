#include "nanotube.h"
#include "constant.h"
#include "vector.h"
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;


Molecule    generate_nanotube(INTG n, INTG m, REAL l, REAL cc, bool ma)
{
    // Check that input is reasonable
    if (n < 0 || m < 0) {
        cout << "nanotube: n and m must be positive integers" << endl;
        return Molecule();
    }

    if (n == 0 && m == 0) {
        cout << "nanotube: n and m can not both be zero" << endl;
        return Molecule();
    }

    if (l <= 0) {
        cout << "nanotube: Nanotube length must be a positive value" << endl;
        return Molecule();
    }


    // Calculate greatest common divisor d_R
    INTG    num1 = 2*m + n;
    INTG    num2 = 2*n + m;

    while (num1 != num2) {
        if (num1 > num2)    num1 -= num2;
        else                num2 -= num1;
    }

    INTG    d_R = num1;


    // Compute geometric properties
    REAL    C       = cc * sqrt(3.0 * (n*n + m*n + m*m));
    REAL    R       = C / (2.0 * M_PI);
    REAL    L_cell  = sqrt(3.0) * C / d_R;


    // compensate for the fact that inscribed polygons have
    // a shorter circumference than a circle, thus we have
    // to fudge the radius of the NT a bit or bonds will be short
    REAL    nchord  = 2.0 * m * sqrt(3.0 * (n*n + m*n + m*m)) / sqrt(3.0*m*m);
    REAL    rfudge  = M_PI / nchord / sin(M_PI / nchord);

    
    // Number of unit cells
    INTG    N_cell  = ceil(l / L_cell);


    // Index min/max
    INTG    pmin = 0;
    INTG    pmax = ceil(n + (n + 2*m)/d_R);
    INTG    qmin = floor(-(2*n + m)/d_R);
    INTG    qmax = m;
    INTG    i = 0;


    vector<VecReal>     coord1;
    vector<VecReal>     coord2;

    // Generate unit cell coordinates
    for (INTG q = qmin; q <= qmax; q++) {
        for (INTG p = pmin; p <= pmax; p++) {
            
            // First basis atom
            REAL xprime1 = 3.0*cc*cc*(p*(2.0*n+m) + q*(n+2.0*m))/(2.0*C);
            REAL yprime1 = 3.0*sqrt(3.0)*cc*cc*(p*m - q*n)/(2.0*C);

            // Second basis atom
            REAL xprime2 = xprime1 + 3.0*cc*cc*(m+n)/(2.0*C);
            REAL yprime2 = yprime1 - cc*cc*sqrt(3.0)*(n-m)/(2.0*C);

            REAL phi1 = xprime1 / R;
            REAL phi2 = xprime2 / R;

            REAL x1 = rfudge * R * cos(phi1);
            REAL x2 = rfudge * R * cos(phi2);
            REAL y1 = rfudge * R * sin(phi1);
            REAL y2 = rfudge * R * sin(phi2);
            REAL z1 = yprime1;
            REAL z2 = yprime2;

            // Store coordinates of unit cell in an array
            // 0 <= xprime1 < C and 0 <= yprime1 < L_cell
            if (   0 <= xprime1 && 0 <= yprime1
                && p*(2.0*n + m) + q*(n + 2.0*m) < 2.0*(n*n + n*m + m*m)
                && d_R*(p*m - q*n) < 2.0*(n*n + n*m + m*m)  ) {
                    coord1.push_back(VecReal(x1, y1, z1));
                    coord2.push_back(VecReal(x2, y2, z2));

                    i++;
            }
        } // end for p
    } // end for q

    INTG    num_atom = i;
    INTG    k = 0;


    // generate nanotube
    Molecule    nanotube;
    
    nanotube.nAtom  = size_t(2 * num_atom * N_cell);
    nanotube.charge = 0;

    if (ma == true) {   // C-C
        for (size_t ii = 0; ii < nanotube.nAtom; ii++)
            nanotube.zval.push_back(6);
    }
    else {  // B-N
        for (size_t ii = 0; ii < nanotube.nAtom; ii++)
            if (ii % 2) nanotube.zval.push_back(5);
            else        nanotube.zval.push_back(7);
    }

    for (INTG jj = 0; jj < N_cell; jj++) {
        for (INTG ii = 0; ii < num_atom; ii++) {
            nanotube.geom.push_back(VecReal(coord1[ii].x, coord1[ii].y, coord1[ii].z + jj*L_cell));
            nanotube.geom.push_back(VecReal(coord2[ii].x, coord2[ii].y, coord2[ii].z + jj*L_cell));
        }
    }

    return nanotube;
}
