#ifndef MET_H
#define MET_H

#include <iostream>
#include <cmath>
#include "ap_int.h"
#include "ap_fixed.h"

// For testing
#define NTEST 4
#define NPART 10
#define FLOATPI 3.141593

// Input / Output types
#define PT_SIZE 16
#define PT_DEC 4
#define PHI_SIZE 8
typedef ap_int<PT_SIZE> pt_t;
typedef ap_int<PHI_SIZE> phi_t;

//MET alg types
typedef ap_fixed<18,15> sincos_t; // TODO optimize ( vals in [-1,1] )
typedef ap_int<PT_SIZE> sumxy_t; // TODO optimize ( vals in [-METMAX,METMAX] )
#define ACOS_SIZE 8
typedef ap_int<ACOS_SIZE> acos_t; // TODO optimize ( vals in [-1,1] )


// reference and hardware functions
void met_ref(float in_pt[NPART], float in_phi[NPART], float& out_pt, float& out_phi);
void met_hw(pt_t data_pt[NPART], phi_t data_phi[NPART], pt_t& res_pt, phi_t& res_phi);


// Cosine init + lookup
template<class res_T, int N_TABLE>
void init_cos_table(res_T table_out[N_TABLE]) {
    int mymax = std::min(PHI_SIZE,N_TABLE);
    for (int ii = 0; ii < mymax; ii++) {
        // Convert: table index ->  HW x-value -> real x-value
        // (0,PHI_SIZE) -> (-PHI_SIZE/2, PHI_SIZE) -> (-pi,pi)
        float in_val = (ii-PHI_SIZE/2) * (2*FLOATPI)/pow(2,PHI_SIZE);
        sincos_t hw_val = cos(in_val);
        table_out[ii] =  hw_val;
    }
}
template<class data_T, class res_T, int N_TABLE>
void Cos(data_T data, res_T &res) {

    // Initialize the lookup table
    res_T cos_table[N_TABLE];
    init_cos_table<res_T,N_TABLE>(cos_table);

    // Index into the lookup table based on data
    // (phi runs from -PHI_SIZE/2 to PHI_SIZE/2-1)
    int index = data+PHI_SIZE/2;
    if (index < 0) index = 0;
    if (index >= N_TABLE) index = N_TABLE-1;
    res = cos_table[index];    
}
// default tab size
template<class data_T, class res_T>
void Cos(data_T data, res_T &res) {
    const int tab_size = pow(2,PHI_SIZE);
    Cos<data_T, res_T, tab_size>(data,res);
}

/*
// Sine init + lookup
template<int N_TABLE>
void init_sin_table(res_T table_out[N_TABLE]) {
    for (int ii = 0; ii < PHI_SIZE; ii++) {
        // Convert: table index ->  HW x-value -> real x-value
        // (0,PHI_SIZE) -> (-PHI_SIZE/2, PHI_SIZE) -> (-pi,pi)
        float in_val = (ii-PHI_SIZE/2) * (2*FLOATPI)/pow(2,PHI_SIZE);
        sincos_t hw_val = sin(in_val);
        table_out[ii] =  hw_val;
    }
}
template<class data_T, class res_T, int N_TABLE>
void Sin(T_in data, rT_co &res) {

    // Initialize the lookup table
    res_T sin_table[N_TABLE];
    init_sin_table<N_TABLE>(sin_table);

    // Index into the lookup table based on data
    // (phi runs from -PHI_SIZE/2 to PHI_SIZE/2-1)
    int index = data+PHI_SIZE/2;
    if (index < 0) index = 0;
    if (index >= N_TABLE) index = N_TABLE-1;
    res = sin_table[index];    
}
*/

/*
// ArcCosine init + lookup
template<int N_TABLE>
void init_acos_table(res_T table_out[N_TABLE]) {
    for (int ii = 0; ii < PHI_SIZE; ii++) {
        // Convert: table index ->  HW x-value -> real x-value
        // (0,PHI_SIZE) -> (-PHI_SIZE/2, PHI_SIZE) -> (-pi,pi)
        float in_val = (ii-PHI_SIZE/2) * (2*FLOATPI)/pow(2,PHI_SIZE);
        sincos_t hw_val = acos(in_val);
        table_out[ii] =  hw_val;
    }
}
template<class data_T, class res_T, int N_TABLE>
void Acos(T_in data, rT_co &res) {

    // Initialize the lookup table
    res_T acos_table[N_TABLE];
    init_acos_table<N_TABLE>(acos_table);

    // Index into the lookup table based on data
    // (phi runs from -PHI_SIZE/2 to PHI_SIZE/2-1)
    int index = data+PHI_SIZE/2;
    if (index < 0) index = 0;
    if (index >= N_TABLE) index = N_TABLE-1;
    res = acos_table[index];    
}
*/

#endif
