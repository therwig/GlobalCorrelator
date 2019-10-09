#ifndef MET_H
#define MET_H

#include <iostream>
#include <cmath>
#include "ap_int.h"
#include "ap_fixed.h"
#include "hls_math.h"

// For testing
#define NTEST 20
#define NPART 10
#define FLOATPI 3.141593
#define DEBUG 0

// Input / Output types
#define PT_SIZE 16
#define PT2_SIZE 2*16
#define PHI_SIZE 8
typedef ap_int<PT_SIZE> pt_t;
typedef ap_uint<PT2_SIZE> pt2_t;
//typedef ap_uint<PT_SIZE> pt2_t;
typedef ap_int<PHI_SIZE> phi_t;

//MET alg types
//#define SINCOS_SIZE 32
//#define SINCOS_DEC 16
#define SINCOS_SIZE 10
#define SINCOS_DEC (SINCOS_SIZE-2)
typedef ap_fixed<SINCOS_SIZE,SINCOS_SIZE-SINCOS_DEC> sincos_t; // TODO optimize ( vals in [-1,1] )
typedef ap_int<PT_SIZE> sumxy_t; // TODO optimize ( vals in [-METMAX,METMAX] )
//typedef ap_int<ACOS_SIZE> acos_t; // TODO optimize ( vals in [-1,1] )

// 2 * PHI_SIZE
// #define COS_TABLE_SIZE 256
// #define SIN_TABLE_SIZE 256
#define COS_TABLE_SIZE (1<<PHI_SIZE)
#define SIN_TABLE_SIZE (1<<PHI_SIZE)

// +/- the sin/cos decimals
#define ACOS_SIZE (SINCOS_DEC+1)
// #define ACOS_TABLE_SIZE 256
#define ACOS_TABLE_SIZE (1<<ACOS_SIZE)

// acos(sqrt(x))
//#define AS_SIZE 4
#define AS_SIZE (SINCOS_DEC+1)
#define SQRT_ACOS_TABLE_SIZE (1<<AS_SIZE)
typedef ap_fixed<AS_SIZE+1,1> as_t;
//+1 for +/- with as_size decimal

// for ratio table
#define RT_NUM (1<<AS_SIZE)
#define RT_DEN (1<<AS_SIZE)

// reference and hardware functions
void met_ref(float in_pt[NPART], float in_phi[NPART], float& out_pt, float& out_phi);
void met_hw(pt_t data_pt[NPART], phi_t data_phi[NPART], pt2_t& res_pt2, phi_t& res_phi);


// Cosine init + lookup
template<class res_T>
void init_cos_table(res_T table_out[COS_TABLE_SIZE]) {
    for (int ii = 0; ii < COS_TABLE_SIZE; ii++) {
        // Convert: table index ->  HW x-value -> real x-value
        // (0,PHI_SIZE) -> (-PHI_SIZE/2, PHI_SIZE) -> (-pi,pi)
        float in_val = (ii-(1<<(PHI_SIZE-1)))
            * (2*FLOATPI)/pow(2,PHI_SIZE);
        res_T hw_val = cos(in_val);
        table_out[ii] =  hw_val;
        //std::cout << " " << ii << " " << in_val << " " << hw_val << std::endl;
    }
}
template<class data_T, class res_T>
void Cos(data_T data, res_T &res) {

    // Initialize the lookup table
#ifdef __HLS_SYN__
    bool initialized = false;
    res_T cos_table[COS_TABLE_SIZE];
#else 
    static bool initialized = false;
    static res_T cos_table[COS_TABLE_SIZE];
#endif
    if (!initialized) {
        init_cos_table<res_T>(cos_table);
        initialized = true;
    }

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
        res_T hw_val = sin(in_val);
        table_out[ii] =  hw_val;
    }
}
template<class data_T, class res_T>
void Sin(data_T data, res_T &res) {

    // Initialize the lookup table
#ifdef __HLS_SYN__
    bool initialized = false;
    res_T sin_table[SIN_TABLE_SIZE];
#else 
    static bool initialized = false;
    static res_T sin_table[SIN_TABLE_SIZE];
#endif
    if (!initialized) {
        init_sin_table<res_T>(sin_table);
        initialized = true;
    }

    // Index into the lookup table based on data
    // (phi runs from -PHI_SIZE/2 to PHI_SIZE/2-1)
    ap_uint<PHI_SIZE> index = data + (1<<(PHI_SIZE-1));
    if (index < 0) index = 0;
    if (index >= SIN_TABLE_SIZE) index = SIN_TABLE_SIZE-1;
    res = sin_table[index];
}


// arccos init + lookup
template<class res_T>
void init_acos_table(res_T table_out[ACOS_TABLE_SIZE]) {
    for (int ii = 0; ii < ACOS_TABLE_SIZE; ii++) {
        // Convert: table index ->  HW x-value -> real x-value
        // (0,PHI_SIZE) -> (-PHI_SIZE/2, PHI_SIZE) -> (-pi,pi)
        float in_val = float(ii)/(1<<(ACOS_SIZE-1))-1.;
        res_T hw_val = (1<<(PHI_SIZE-1))/FLOATPI * acos(in_val);
        table_out[ii] =  hw_val;
        // std::cout << " " << ii << " " << in_val << " " << acos(in_val) << " " << table_out[ii] << std::endl;
    }
}
template<class data_T, class res_T>
void Acos(data_T data, res_T &res) {

    // Initialize the lookup table
#ifdef __HLS_SYN__
    bool initialized = false;
    res_T acos_table[ACOS_TABLE_SIZE];
#else 
    static bool initialized = false;
    static res_T acos_table[ACOS_TABLE_SIZE];
#endif
    if (!initialized) {
        init_acos_table<res_T>(acos_table);
        initialized = true;
    }
    /* res_T acos_table[ACOS_TABLE_SIZE]; */
    /* init_acos_table<res_T>(acos_table); */

    // Index into the lookup table based on data
    // (phi runs from -PHI_SIZE/2 to PHI_SIZE/2-1)
    ap_uint<ACOS_SIZE> index = ap_uint<SINCOS_DEC+1>(data*(1<<SINCOS_DEC))-(1<<SINCOS_DEC);
    if (index < 0) index = 0;
    if (index >= ACOS_TABLE_SIZE) index = ACOS_TABLE_SIZE-1;
    res = acos_table[index];
    /* std::cout << " -> "; */
    /* std::cout << data << " "; */
    /* std::cout << index << " "; */
    /* std::cout << res << " "; */
    /* std::cout << std::endl; */
}


// arccos(sqrt(x)) init + lookup
template<class res_T>
void init_sqrt_acos_table(res_T table_out[SQRT_ACOS_TABLE_SIZE]) {
    for (int ii = 0; ii < SQRT_ACOS_TABLE_SIZE; ii++) {
        float in_val = float(ii)/(1<<AS_SIZE);
        float x = (1<<(PHI_SIZE-1))/FLOATPI * acos(sqrt(in_val));
        res_T hw_val = x;
        table_out[ii] =  hw_val;
//        std::cout << " " << ii << " " << in_val << " " << 
//            (1<<(PHI_SIZE-1))/FLOATPI * acos(sqrt(in_val)) << " " 
//                  << table_out[ii] << std::endl;
    }
}
template<class data_T, class res_T>
void SqrtACos(data_T data, res_T &res) {

#ifdef __HLS_SYN__
    bool initialized = false;
    res_T sqrt_acos_table[SQRT_ACOS_TABLE_SIZE];
#else 
    static bool initialized = false;
    static res_T sqrt_acos_table[SQRT_ACOS_TABLE_SIZE];
#endif
    if (!initialized) {
        init_sqrt_acos_table<res_T>(sqrt_acos_table);
        initialized = true;
    }

    // Initialize the lookup table
    /* res_T sqrt_acos_table[SQRT_ACOS_TABLE_SIZE]; */
    /* init_sqrt_acos_table<res_T>(sqrt_acos_table); */
    //return;
    // Index into the lookup table based on data
    // (phi runs from -PHI_SIZE/2 to PHI_SIZE/2-1)
    ap_uint<AS_SIZE> index = data*(1<<AS_SIZE);
    if (index < 0) index = 0;
    if (index >= SQRT_ACOS_TABLE_SIZE) index = SQRT_ACOS_TABLE_SIZE-1;
    res = sqrt_acos_table[index];
    /* std::cout << " -> "; */
    /* std::cout << data << " "; */
    /* std::cout << index << " "; */
    /* std::cout << res << " "; */
    /* std::cout << std::endl; */
}

// division = multiplication and bit shift
// if a, b uint<16>, then a in (0,2^16-1) and 1==2^16
// then 1/b=2^16/b and a/b=a*(2^16/b)
// can convert to decimal by shifting 16 bits
// a/b = a*(2^16/b) >> 16

typedef DIVI_TAB_SIZE = 512
void init_shiftPT2_invert_table(ap_uint<PT2_SIZE> table_out[]) {
    // Implement division lookup
    table_out[0] = (1<<PT2_SIZE)-1;
    for (int i = 1; i < DIVI_TAB_SIZE; i++) {
        // shift and divide
        table_out[i] = (1<<PT2_SIZE)/i;
        }
    }
    return;
}
void Divide_PT2_asT(ap_uint<PT2_SIZE> data_num, ap_uint<PT2_SIZE> data_den, as_T &res) {

#ifdef __HLS_SYN__
    bool initialized = false;
    ap_uint<PT2_SIZE> inv_table[DIVI_TAB_SIZE];
#else 
    static bool initialized = false;
    static ap_uint<PT2_SIZE> inv_table[DIVI_TAB_SIZE];
#endif
    if (!initialized) {
        init_shiftPT2_invert_table<res_T>(division_table);
        initialized = true;
    }

    // what precision to use here?
    //ap_uint<PT2_SIZE> num_x_denInverse = data_num * inv_table[]
    
    // Index into the lookup table based on data
    int index_num, index_den, index;
    /* data_T data_num = _data_num; */
    /* data_T data_den = _data_den; */

    //#pragma HLS PIPELINE
    if (data_num < 0) data_num = 0;
    if (data_den < 0) data_den = 0;
    if (data_num > RT_NUM-1) data_num = RT_NUM-1;
    if (data_den > RT_DEN-1) data_den = RT_DEN-1;
    index = (data_num*RT_NUM) + data_den;
    res = division_table[index];

    return;
}



/* // Divide two numbers, each of which run from 0 to (1<<AS_SIZE) */
/* // The numerator is always less than the denominator, allowing us to save space :) */
/* //   I'll skip this implementation for now and just make the n*n table (not n(n+1)/2) */
/* template<class data_T> */
/* void init_division_table(data_T table_out[]) { */
/*     // Implement division lookup */
/*     for (int iden = 0; iden < RT_DEN; iden++) { */
/*         for (int inum = 0; inum < RT_NUM; inum++) { */
/*             int index = (inum*RT_NUM)+iden; */
/*             // Compute lookup table function */
/*             data_T real_val = (iden>0) ? float(inum)/iden : 0; */
/*             table_out[index] = real_val; */
/*         } */
/*     } */
/*     return; */
/* } */
/* template<class data_T, class res_T> */
/* void Divide(data_T data_num, data_T data_den, res_T &res) { */

/* #ifdef __HLS_SYN__ */
/*     bool initialized = false; */
/*     res_T division_table[RT_NUM*RT_DEN]; */
/* #else  */
/*     static bool initialized = false; */
/*     static res_T division_table[RT_NUM*RT_DEN]; */
/* #endif */
/*     if (!initialized) { */
/*         init_division_table<res_T>(division_table); */
/*         initialized = true; */
/*     } */

/*     /\* // Initialize the lookup table *\/ */
/*     /\* res_T division_table[RT_NUM*RT_DEN]; *\/ */
/*     /\* init_division_table<res_T>(division_table); *\/ */

/*     // Index into the lookup table based on data */
/*     int index_num, index_den, index; */
/*     /\* data_T data_num = _data_num; *\/ */
/*     /\* data_T data_den = _data_den; *\/ */

/*     //#pragma HLS PIPELINE */
/*     if (data_num < 0) data_num = 0; */
/*     if (data_den < 0) data_den = 0; */
/*     if (data_num > RT_NUM-1) data_num = RT_NUM-1; */
/*     if (data_den > RT_DEN-1) data_den = RT_DEN-1; */
/*     index = (data_num*RT_NUM) + data_den; */
/*     res = division_table[index]; */

/*     return; */
/* } */



/* // ArcCosine init + lookup */
/* template<class res_T> */
/* void init_acos_table(res_T table_out[ACOS_TABLE_SIZE]) { */
/*     for (int ii = 0; ii < ACOS_TABLE_SIZE; ii++) { */
/*         // Convert: table index ->  HW x-value -> real x-value */
/*         // acos : (-1,1) -> (0,pi) */
/*         // (0,ACOS_TABLE_SIZE) -> (-PHI_SIZE/2, PHI_SIZE) -> (-1,1) */
/*         //  */

/*         // (0,PHI_SIZE) -> (-PHI_SIZE/2, PHI_SIZE) ->  */
/*         float in_val = (ii-PHI_SIZE/2) * (2*FLOATPI)/pow(2,PHI_SIZE); */
/*         sincos_t hw_val = acos(in_val); */
/*         table_out[ii] =  hw_val; */
/*     } */
/* } */
/* template<class data_T, class res_T> */
/* void Acos(data_T data, res_T &res) { */

/*     // Initialize the lookup table */
/*     res_T acos_table[ACOS_TABLE_SIZE]; */
/*     init_acos_table<res_T>(acos_table); */

/*     // Index into the lookup table based on data */
/*     // (phi runs from -PHI_SIZE/2 to PHI_SIZE/2-1) */
/*     int index = data+PHI_SIZE/2; */

/*     // data is sincos_t = fixed pt(SINCOS_SIZE,SINCOS_DEC) */
/*     // ACOS_BITS */
/*     // shift (-1,1) to (0,2^ACOS_BITS) */
/*     //ap_uint<ACOS_BITS> index = ap_uint<ACOS_BITS>(data+1) * ap_uint<ACOS_BITS>(1<<ACOS_BITS); */
/*     if (index < 0) index = 0; */
/*     if (index >= ACOS_TABLE_SIZE) index = ACOS_TABLE_SIZE-1; */
/*     res = acos_table[index];     */
/* } */

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
