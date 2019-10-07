#include <iostream>
#include <cmath>
#include "ap_int.h"
#include "ap_fixed.h"

#include "src/division.h"

// PHI_SIZE

void cos_test(){    
    // loop through phi values and print the results
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

