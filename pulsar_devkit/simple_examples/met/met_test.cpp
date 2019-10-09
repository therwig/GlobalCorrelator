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

//#include "lut_test.cpp"

int alg_test() {

    // calculate met for NPART particles
    pt_t in_pt_hw[NPART];
    pt2_t out_pt2_hw;
    phi_t in_phi_hw[NPART], out_phi_hw;
    float in_pt[NPART], in_phi[NPART];
    float out_pt, out_phi;

    //setup random
    std::default_random_engine generator(1776); // seed
    std::uniform_real_distribution<float> pt_dist(10.,100.);
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

    for (int i=0; i<NTEST; ++i) {
        if(DEBUG) std::cout << "\n\n\n\nEvent " << i << std::endl;
        for(int j=0; j<NPART; j++){
            // convert float to hw
            in_pt_hw[j]  = vals[i][j].first;
            in_phi_hw[j] = pow(2,PHI_SIZE)/(2*FLOATPI)*vals[i][j].second;
            // keep test vals as float
            in_pt[j]  = vals[i][j].first;
            in_phi[j] = vals[i][j].second;

            if(DEBUG){
                std::cout << " \t part pt " << in_pt[j];
                std::cout << "\t phi " << in_phi[j];
                std::cout << std::endl;
            }
        }
        out_pt2_hw=0.; out_phi_hw=0.;
        out_pt=0.; out_phi=0.;
        
        met_ref(in_pt, in_phi, out_pt, out_phi);
        if(DEBUG) std::cout << " REF : met(pt = " << out_pt << ", phi = "<< out_phi << ")\n";

        met_hw(in_pt_hw, in_phi_hw, out_pt2_hw, out_phi_hw);
        float phi_rad = float(out_phi_hw) * (2*FLOATPI)/(1<<PHI_SIZE);
        float pt = sqrt(float(out_pt2_hw));
        if(DEBUG) std::cout << "  HW : met(pt = " << pt << ", phi = "<< phi_rad << ")\n";

        //print compact (in nice units)
        if(!DEBUG) std::cout << "Event " << i
                             << " (REF vs HW) met " << out_pt << " vs " << pt 
                             << ", phi "<< out_phi << " vs "<< phi_rad << "\n";

    }

    return 0;
}

int main() {
    // optionally test pieces
    // cos_test();
    // div_test();
    // acos_test();
    // sqrt_acos_test();

    // test the algorithm
    alg_test();
    //return 1;
    return 0;
}
