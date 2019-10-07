#ifndef MET_H
#define MET_H

#include <iostream>
#include <cmath>
#include "ap_int.h"
#include "ap_fixed.h"
#include "hls_math.h"

#include "division.h"

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
#define SINCOS_SIZE 32
#define SINCOS_DEC 16
typedef ap_fixed<SINCOS_SIZE,SINCOS_DEC> sincos_t; // TODO optimize ( vals in [-1,1] )
typedef ap_int<PT_SIZE> sumxy_t; // TODO optimize ( vals in [-METMAX,METMAX] )
#define ACOS_SIZE 8
typedef ap_int<ACOS_SIZE> acos_t; // TODO optimize ( vals in [-1,1] )

// 2 * PHI_SIZE
#define COS_TABLE_SIZE 256
#define SIN_TABLE_SIZE 256
#define SQRT_TABLE_SIZE 256

#define ACOS_BITS 8
#define ACOS_TABLE_SIZE 256

// reference and hardware functions
void met_ref(float in_pt[NPART], float in_phi[NPART], float& out_pt, float& out_phi);
void met_hw(pt_t data_pt[NPART], phi_t data_phi[NPART], pt_t& res_pt, phi_t& res_phi);


// Cosine init + lookup
template<class res_T>
void init_cos_table(res_T table_out[COS_TABLE_SIZE]) {
    //static bool CosIsFilled = false;
    for (int ii = 0; ii < COS_TABLE_SIZE; ii++) {
        // Convert: table index ->  HW x-value -> real x-value
        // (0,PHI_SIZE) -> (-PHI_SIZE/2, PHI_SIZE) -> (-pi,pi)
        float in_val = (ii-(1<<(PHI_SIZE-1)))
            * (2*FLOATPI)/pow(2,PHI_SIZE);
        sincos_t hw_val = cos(in_val);
        table_out[ii] =  hw_val;
        //std::cout << " " << ii << " " << in_val << " " << hw_val << std::endl;
    }
    //CosIsFilled = true;
}
template<class data_T, class res_T>
void Cos(data_T data, res_T &res) {

    // Initialize the lookup table
    res_T cos_table[COS_TABLE_SIZE];
    init_cos_table<res_T>(cos_table);

    // Index into the lookup table based on data
    // (phi runs from -PHI_SIZE/2 to PHI_SIZE/2-1)
    ap_uint<PHI_SIZE> index = data + (1<<(PHI_SIZE-1));
    if (index < 0) index = 0;
    if (index >= COS_TABLE_SIZE) index = COS_TABLE_SIZE-1;
    res = cos_table[index];    
    /* std::cout << " -> "; */
    /* std::cout << data << " "; */
    /* std::cout << index << " "; */
    /* std::cout << res  << " "; */
    /* std::cout << std::endl; */
}


// Sine init + lookup
template<class res_T>
void init_sin_table(res_T table_out[SIN_TABLE_SIZE]) {
    for (int ii = 0; ii < SIN_TABLE_SIZE; ii++) {
        // Convert: table index ->  HW x-value -> real x-value
        // (0,PHI_SIZE) -> (-PHI_SIZE/2, PHI_SIZE) -> (-pi,pi)
        float in_val = (ii-(1<<(PHI_SIZE-1)))
            * (2*FLOATPI)/pow(2,PHI_SIZE);
        sincos_t hw_val = sin(in_val);
        table_out[ii] =  hw_val;
    }
}
template<class data_T, class res_T>
void Sin(data_T data, res_T &res) {

    // Initialize the lookup table
    res_T sin_table[SIN_TABLE_SIZE];
    init_sin_table<res_T>(sin_table);

    // Index into the lookup table based on data
    // (phi runs from -PHI_SIZE/2 to PHI_SIZE/2-1)
    ap_uint<PHI_SIZE> index = data + (1<<(PHI_SIZE-1));
    if (index < 0) index = 0;
    if (index >= SIN_TABLE_SIZE) index = SIN_TABLE_SIZE-1;
    res = sin_table[index];    
}


// ArcCosine init + lookup
template<class res_T>
void init_acos_table(res_T table_out[ACOS_TABLE_SIZE]) {
    for (int ii = 0; ii < ACOS_TABLE_SIZE; ii++) {
        // Convert: table index ->  HW x-value -> real x-value
        // acos : (-1,1) -> (0,pi)
        // (0,ACOS_TABLE_SIZE) -> (-PHI_SIZE/2, PHI_SIZE) -> (-1,1)
        // 

        // (0,PHI_SIZE) -> (-PHI_SIZE/2, PHI_SIZE) -> 
        float in_val = (ii-PHI_SIZE/2) * (2*FLOATPI)/pow(2,PHI_SIZE);
        sincos_t hw_val = acos(in_val);
        table_out[ii] =  hw_val;
    }
}
template<class data_T, class res_T>
void Acos(data_T data, res_T &res) {

    // Initialize the lookup table
    res_T acos_table[ACOS_TABLE_SIZE];
    init_acos_table<res_T>(acos_table);

    // Index into the lookup table based on data
    // (phi runs from -PHI_SIZE/2 to PHI_SIZE/2-1)
    int index = data+PHI_SIZE/2;

    // data is sincos_t = fixed pt(SINCOS_SIZE,SINCOS_DEC)
    // ACOS_BITS
    // shift (-1,1) to (0,2^ACOS_BITS)
    //ap_uint<ACOS_BITS> index = ap_uint<ACOS_BITS>(data+1) * ap_uint<ACOS_BITS>(1<<ACOS_BITS);
    if (index < 0) index = 0;
    if (index >= ACOS_TABLE_SIZE) index = ACOS_TABLE_SIZE-1;
    res = acos_table[index];    
}

/*
// Sqrt init + lookup
template<class res_T>
void init_sqrt_table(res_T table_out[SQRT_TABLE_SIZE]) {
    for (int ii = 0; ii < SQRT_TABLE_SIZE; ii++) {
        // Convert: table index ->  HW x-value -> real x-value
        // (0,PHI_SIZE) -> (-PHI_SIZE/2, PHI_SIZE) -> (-pi,pi)
        float in_val = (ii-PHI_SIZE/2) * (2*FLOATPI)/pow(2,PHI_SIZE);
        sincos_t hw_val = sqrt(in_val);
        table_out[ii] =  hw_val;
    }
}
template<class data_T, class res_T>
void SquareRoot(data_T data, res_T &res) {

    // Initialize the lookup table
    res_T sqrt_table[SQRT_TABLE_SIZE];
    init_sqrt_table<res_T>(sqrt_table);

    // Index into the lookup table based on data
    // (phi runs from -PHI_SIZE/2 to PHI_SIZE/2-1)
    int index = data+PHI_SIZE/2;
    if (index < 0) index = 0;
    if (index >= SQRT_TABLE_SIZE) index = SQRT_TABLE_SIZE-1;
    res = sqrt_table[index];    
}
*/
#endif
