#ifndef DIVISION_H
#define DIVISION_H

#include "ap_int.h"
#include "ap_fixed.h"

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
