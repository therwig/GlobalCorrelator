#include <iostream>
#include <cmath>
#include "ap_int.h"
#include "ap_fixed.h"

#include "src/division.h"

void cos_test(){    
    // loop through phi values and print the results
    std::cout<<"Running cos_test" << std::endl;
    phi_t phi;
    ap_uint<PHI_SIZE> index;
    sincos_t result;
    for(int i=0; i<(1<<PHI_SIZE); i++){
        phi = i-(1<<(PHI_SIZE-1));
        index = phi + (1<<(PHI_SIZE-1));
        Cos<phi_t, sincos_t>(phi,result);

        std::cout << phi << " ";
        std::cout << index << " ";
        std::cout << result << " ";
        std::cout << std::endl;
    }
}

void acos_test(){    
    // loop through (-1,1) and print the acos results
    std::cout<<"Running acos_test" << std::endl;
    sincos_t f=0;
    ap_uint<SINCOS_DEC+1> index;
    phi_t result=0;
    for(int i=0; i<(1<<(SINCOS_DEC+1)); i++){
        f = sincos_t(float(i)/(1<<SINCOS_DEC))-sincos_t(1.);
        index = ap_uint<SINCOS_DEC+1>(f*(1<<SINCOS_DEC))-(1<<SINCOS_DEC);
        Acos<sincos_t,phi_t>(f,result);

        std::cout << "x=" << f << ", ";
        std::cout << "acos(x)=" << acos(float(f)) << ", ";
        std::cout << "bit=" << (1<<(PHI_SIZE-1))/FLOATPI*acos(float(f)) << ", ";
        std::cout << "tabi=" << index << ", ";
        std::cout << "result=" << result << ", ";
        std::cout << std::endl;
    }
}

void sqrt_acos_test(){
    // loop through (0,1) and print the acos(sqrt(x)) results
    std::cout<<"Running sqrt_acos_test" << std::endl;
    as_t f=0;
    ap_uint<AS_SIZE> index;
    phi_t result=0;
    for(int i=0; i<(1<<AS_SIZE); i++){
        f = as_t(float(i)/(1<<AS_SIZE));
        index = ap_uint<AS_SIZE>(f*(1<<AS_SIZE));
        SqrtACos<sincos_t,phi_t>(f,result);

        // std::cout << "x=" << f << ", ";
        // std::cout << "sqrt(x)=" << sqrt(float(f)) << ", ";
        // std::cout << "acos(sqrt(x))=" << acos(sqrt(float(f))) << ", ";
        // std::cout << "bit=" << (1<<(PHI_SIZE-1))/FLOATPI*acos(sqrt(float(f))) << ", ";
        // std::cout << "tabi=" << index << ", ";
        // std::cout << "result=" << result << ", ";
        // std::cout << std::endl;
    }
}


void div_test(){
    // typedef ap_uint<2*PT_SIZE> pt2_t;
    // divide two pt^2 values. result always (0,1)
    std::cout<<"Running div_test" << std::endl;
    
    ap_uint<AS_SIZE> index_x;
    ap_uint<AS_SIZE> index_y;
    as_t result=0;

    for(int j=0; j<(1<<AS_SIZE); j++){
        for(int i=0; i<=j; i++){
            index_x = i;
            index_y = j;
            ratio<pt2_t,pt2_t>(index_x,index_y,result);

            std::cout << "x=" << index_x << ", ";
            std::cout << "y=" << index_x << ", ";
            std::cout << "x/y=" << float(i)/j << ", ";
            std::cout << "result=" << result << " ";
            std::cout << std::endl;
    }
}

