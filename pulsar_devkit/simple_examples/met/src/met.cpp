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
void met_hw(pt_t data_pt[NPART], phi_t data_phi[NPART], pt_t& res_pt, phi_t& res_phi){

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
    // square_root(tmp, res_pt);
    //res_pt = sqrt(met_x*met_x+met_y*met_y);
    res_pt = (met_x*met_x+met_y*met_y);

    // guard zero
    if (res_pt == 0){
        res_phi=0.;
        return;
    }
    sincos_t divi; // x/tot
    division(met_x,res_pt,divi);
    // Acos<phi_t,sincos_t>(divi,res_phi);
    // if(met_y<0) res_phi = - res_phi;
    res_phi=0.;

    return;
}

// THE END
