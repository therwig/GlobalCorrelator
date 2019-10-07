/*
Example using division LUT
*/
#include <vector>
#include <cstdio>
#include <utility>
#include <random>

#include "ap_int.h"
#include "ap_fixed.h"
#include "src/met.h"
#include "lut_test.cpp"

int alg_test() {

    // calculate met for NPART particles
    pt_t in_pt_hw[NPART], out_pt_hw;
    phi_t in_phi_hw[NPART], out_phi_hw;
    float in_pt[NPART], in_phi[NPART];
    float out_pt, out_phi;

    //setup random
    std::default_random_engine generator(1776); // seed
    std::uniform_real_distribution<float> pt_dist(0.,100.);
    std::uniform_real_distribution<float> phi_dist(-FLOATPI,FLOATPI);

    //fill test data
    std::vector<std::vector<std::pair<float,float> > > vals;
    vals.resize(NTEST);
    for(int i=0; i<NTEST; i++){
        vals[i].resize(NPART);
        for(int j=0; j<NPART; j++){
            vals[i][j].first  = pt_dist(generator);
            vals[i][j].second = phi_dist(generator);
            if(0){
                std::cout << vals[i][j].first  << "  ";
                std::cout << vals[i][j].second << "\n";
            }
        }
    }

    float pt_hw;
    float phi_hw;
    for (int i=0; i<NTEST; ++i) {

        for(int j=0; j<NPART; j++){
            // convert float to hw
            in_pt_hw[j]  = pow(2,PT_DEC)*vals[i][j].first;
            in_phi_hw[j] = pow(2,PHI_SIZE)/(2*FLOATPI)*vals[i][j].second;
            // keep test vals as float
            in_pt[j]  = vals[i][j].first;
            in_phi[j] = vals[i][j].second;
        }
        out_pt_hw=0.; out_phi_hw=0.;
        out_pt=0.; out_phi=0.;
        
        met_ref(in_pt, in_phi, out_pt, out_phi);
        std::cout << " REF : met(pt = " << out_pt << ", phi = "<< out_phi << ")\n";

        met_hw(in_pt_hw, in_phi_hw, out_pt_hw, out_phi_hw);
        std::cout << "  HW : met(pt = " << out_pt_hw << ", phi = "<< out_phi_hw << ")\n";

    }

    return 0;
}

int main() {
    cos_test();
    return 0;
    alg_test();
    return 0;
}
