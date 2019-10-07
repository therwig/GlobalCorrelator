#ifndef DIVISION_H
#define DIVISION_H

#include "ap_int.h"
#include "ap_fixed.h"

#include "met.h"

// Divide two numbers, each of which run from 0 to (1<<AS_SIZE)
// The numerator is always less than the denominator, allowing us to save space :)
//   I'll skip this implementation for now and just make the n*n table (not n(n+1)/2)

#define RT_NUM (1<<AS_SIZE)
#define RT_DEN (1<<AS_SIZE)

template<class data_T>
void init_ratio_table(data_T table_out[]) {
    // Implement division lookup
    for (int iden = 0; iden < RT_DEN; iden++) {
        for (int inum = 0; inum < RT_NUM; inum++) {
            int index = (inum*RT_NUM)+iden;
            // Compute lookup table function
            data_T real_val = (iden>0) ? float(inum)/iden : 0;
            table_out[index] = real_val;
        }
    }
    return;
}

// lookup
template<class data_T, class res_T>
void ratio(data_T data_num, data_T data_den, res_T &res) {
    // Initialize the lookup table
    res_T division_table[RATIO_TABLE_SIZE];
    init_division_table<res_T>(division_table);

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















// size of the LUT
#define N_TABLE_NUM 256 //Maximum number is 2045 for some reason (SIGSEGV otherwise)
#define N_TABLE_DEN 256 //Maximum number is 2045 for some reason (SIGSEGV otherwise)

// initialize
//template<class data_T, int N_TABLE_NUM, int N_TABLE_DEN>
template<class data_T>
void init_division_table(data_T table_out[N_TABLE_NUM*N_TABLE_DEN]) {
    // Implement division lookup
    for (int inum = 0; inum < N_TABLE_NUM; inum++) {
        for (int iden = 0; iden < N_TABLE_DEN; iden++) {
            int index = (inum*N_TABLE_NUM)+iden;
            // Compute lookup table function
            data_T real_val = (iden>0) ? float(inum)/iden : 0;
            table_out[index] = real_val;
        }
    }
    return;
}

// lookup
//template<class data_T, class res_T, int TABLE_SIZE_NUM, int TABLE_SIZE_DEN>
template<class data_T, class res_T>
void division(data_T _data_num, data_T _data_den, res_T &res) {
    // Initialize the lookup table
    res_T division_table[N_TABLE_NUM*N_TABLE_DEN];
    init_division_table<res_T>(division_table);

    // Index into the lookup table based on data
    int index_num, index_den, index;
    data_T data_num = _data_num;
    data_T data_den = _data_den;

    //#pragma HLS PIPELINE
    if (data_num < 0) data_num = 0;
    if (data_den < 0) data_den = 0;
    if (data_num > N_TABLE_NUM-1) data_num = N_TABLE_NUM-1;
    if (data_den > N_TABLE_DEN-1) data_den = N_TABLE_DEN-1;
    index = (data_num*N_TABLE_NUM) + data_den;
    res = division_table[index];

    return;
}



#endif
