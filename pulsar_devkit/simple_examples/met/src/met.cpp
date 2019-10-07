/*
HLS implementation of TANH function via LUT
*/
#include "met.h"
#include <cmath>
#include <cassert>
#ifndef __SYNTHESIS__
#include <cstdio>
#endif

// pt, phi are integers
void met_hw(pt_t data_pt[NPART], phi_t data_phi[NPART], pt2_t& res_pt2, phi_t& res_phi){

    // calc components first
    sincos_t si[NPART];
    sincos_t co[NPART];
    for(int i=0; i<NPART;i++){
        Cos<phi_t, sincos_t>(data_phi[i],co[i]);
        Sin<phi_t,sincos_t>(data_phi[i],si[i]);
    }

    // aggregate sums
    sumxy_t met_x = 0;
    sumxy_t met_y = 0;
    for(int i=0; i<NPART;i++){
        met_x -= data_pt[i] * co[i];
        met_y -= data_pt[i] * si[i];
    }

    // get sqrt
    // pt_t tmp = met_x*met_x+met_y*met_y; // TODO use more bits?
    // SquareRoot<pt_t,pt_t>(tmp, res_pt);
    //res_pt = sqrt(met_x*met_x+met_y*met_y);
    res_pt2 = met_x*met_x+met_y*met_y;
    pt2_t x2 = met_x*met_x;
    //
    // guard zero
    if (res_pt2 == 0){
        res_phi=0.;
        return;
    }
    as_t divi; // (x/tot)**2
    division(x2,res_pt2,divi); // bit shift num+den, reduce precision / table size

    // in Q1, phi in (0, pi/2) = (0,64)
    // in Q2, phi in (pi/2,pi) = (64,128)
    // in Q3/Q4 phi negative
    SqrtACos<as_t,phi_t>(divi,res_phi);
    if(met_x<0) res_phi = (1<<(PHI_SIZE-1)) - res_phi;
    if(met_y<0) res_phi = - res_phi;
    res_phi=0.;

    return;
}

// THE END
