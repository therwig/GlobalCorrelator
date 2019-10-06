#include <math.h>
#include <cmath>
#include <algorithm>
#include "ap_int.h"
#include "ap_fixed.h"
#include "src/met.h"

void met_ref(float in_pt[NPART], float in_phi[NPART], float& out_pt, float& out_phi){
    double met_x=0.;
    double met_y=0.;
    for(int i=0;i<NPART;i++){
        met_x -= in_pt[i] * cos(in_phi[i]);
        met_y -= in_pt[i] * sin(in_phi[i]);
    }
    out_pt = sqrt(pow(met_x,2)+pow(met_y,2));
    if(out_pt<1e-10) out_pt = 1e-10; // guard divide by zero
    out_phi = met_y>=0 ? acos(met_x/out_pt) : -acos(met_x/out_pt);
    return;
}

