#ifndef MET_H
#define MET_H

#include <iostream>
#include <cmath>
#include "ap_int.h"
#include "ap_fixed.h"

// For testing
#define NTEST 20
#define NPART 10
#define FLOATPI 3.141593
#define DEBUG 0

//
// Input / Output types
//

// pT is uint where 1=1GeV, up to 1024=2^10
//   px, py need to be signed
//   pT^2 needs double precision
#define PT_SIZE 10
typedef ap_uint<PT_SIZE> pt_t;
typedef ap_int<PT_SIZE+1> pxy_t;
#define PT2_SIZE 2*PT_SIZE
typedef ap_uint<PT2_SIZE> pt2_t;

// phi size can be optimzed. For ref, 2pi/(2^8)=0.025
#define PHI_SIZE 8
typedef ap_int<PHI_SIZE> phi_t;


void met_ref(float in_pt[NPART], float in_phi[NPART], float& out_pt, float& out_phi);
void met_hw(pt_t data_pt[NPART], phi_t data_phi[NPART], pt2_t& res_pt2, phi_t& res_phi);






//
// Lookup tables for pt projections to X, Y
//

#define PROJ_TAB_SIZE (1<<(PHI_SIZE-2))
// use table size of 1024/4=256 for now, so no precision is lost

template<class pt_T>
void init_projx_table(pt_T table_out[PROJ_TAB_SIZE]) {
    // Return table of cos(phi) where phi is in (0,pi/2)
    // multiply result by 1=2^(PT-SIZE)
    for (int i = 0; i < PROJ_TAB_SIZE; i++) {
        if(0){
            std::cout << " ---> " << i << "  ";// << table_out[i] << std::endl;
            std::cout << (1<<PT_SIZE) << "  ";
            std::cout << (float(i)/PROJ_TAB_SIZE * FLOATPI/2) << "  ";
            std::cout << cos(float(i)/PROJ_TAB_SIZE * FLOATPI/2) << "  ";
            std::cout << (1<<PT_SIZE) * cos(float(i)/PROJ_TAB_SIZE * FLOATPI/2) << "  ";
            std::cout << std::endl;
        }
        table_out[i] = (1<<PT_SIZE) * cos(float(i)/PROJ_TAB_SIZE * FLOATPI/2);
    }
    return;
}

template<class pt_T, class phi_T,class pxy_T>
void ProjX(pt_T pt, phi_T phi, pxy_T &x){
    // Initialize the lookup tables
#ifdef __HLS_SYN__
    bool initialized = false;
    pt_t cos_table[PROJ_TAB_SIZE];
#else 
    static bool initialized = false;
    static pt_t cos_table[PROJ_TAB_SIZE];
#endif
    if (!initialized) {
        init_projx_table(cos_table);
        initialized = true;
    }
    //map phi to first quadrant value: range [0, 2^(PHI_SIZE-2))
    ap_uint<PHI_SIZE-2> phiQ1 = phi;
    if(phi>=(1<<(PHI_SIZE-2))) phiQ1 = (1<<(PHI_SIZE-2)) -1 - phiQ1; // map 64-128 (0-63) to 63-0
    if(phi<0 && phi>=-(1<<(PHI_SIZE-2))) phiQ1 = (1<<(PHI_SIZE-2)) -1 - phiQ1; // map -64-1 (0-63) to 63-0

    // get x component and flip sign if necessary
    //x = pt * cos_table[phiQ1];
    x = (pt * cos_table[phiQ1]) >> PT_SIZE;
    if( phi>=(1<<(PHI_SIZE-2))
        || phi<-(1<<(PHI_SIZE-2)))
        x = -x;

    return;

    // test projection and flipping 
    if(0){
        std::cout << " ---> " << pt << "  ";// << table_out[i] << std::endl;
        std::cout << phi << "  ";
        std::cout << phiQ1 << "  ";
        std::cout << cos_table[phiQ1] << "  ";
        std::cout << pt * cos_table[phiQ1] << "  ";
        std::cout << ((pt * cos_table[phiQ1])>>(PT_SIZE)) << "  ";
        std::cout << float(cos_table[phiQ1])/pow(2,PT_SIZE) << "  ";
        std::cout << "   -> " << x << "  ";// << table_out[i] << std::endl;
        if( phi>=(1<<(PHI_SIZE-2))
            || phi<-(1<<(PHI_SIZE-2)))
            x = 0-x;
        std::cout << x << "  ";
    }

    // test the phi quadrant assignment
    if(0){
        bool flip;
        for(int i=0;i< (1<<PHI_SIZE); i++){
            phi = i;
            flip = false;
            ap_uint<PHI_SIZE-2> phiQ1 = phi;
            if(phi>=(1<<(PHI_SIZE-2))) phiQ1 = (1<<(PHI_SIZE-2)) -1 - phiQ1; // map 64-128 (0-63) to 63-0
            if(phi<0 && phi>=-(1<<(PHI_SIZE-2))) phiQ1 = (1<<(PHI_SIZE-2)) -1 - phiQ1; // map -64-1 (0-63) to 63-0
            if( phi>=(1<<(PHI_SIZE-2))
                || phi<-(1<<(PHI_SIZE-2)))
                flip = true;
            std::cout << "-> " << phi << "  " << phiQ1 << "  " << flip << std::endl;
        }
    }
}







template<class pt_T>
void init_projy_table(pt_T table_out[PROJ_TAB_SIZE]) {
    // Return table of sin(phi) where phi is in (0,pi/2)
    // multiply result by 1=2^(PT-SIZE)
    for (int i = 0; i < PROJ_TAB_SIZE; i++) {
        table_out[i] = (1<<PT_SIZE) * sin(float(i)/PROJ_TAB_SIZE * FLOATPI/2);
    }
    return;
}

template<class pt_T, class phi_T,class pxy_T>
void ProjY(pt_T pt, phi_T phi, pxy_T &y){
    // Initialize the lookup tables
#ifdef __HLS_SYN__
    bool initialized = false;
    pt_t sin_table[PROJ_TAB_SIZE];
#else 
    static bool initialized = false;
    static pt_t sin_table[PROJ_TAB_SIZE];
#endif
    if (!initialized) {
        init_projy_table(sin_table);
        initialized = true;
    }
    //map phi to first quadrant value: range [0, 2^(PHI_SIZE-2))
    ap_uint<PHI_SIZE-2> phiQ1 = phi;
    if(phi>=(1<<(PHI_SIZE-2))) phiQ1 = (1<<(PHI_SIZE-2)) -1 - phiQ1; // map 64-128 (0-63) to 63-0
    if(phi<0 && phi>=-(1<<(PHI_SIZE-2))) phiQ1 = (1<<(PHI_SIZE-2)) -1 - phiQ1; // map -64-1 (0-63) to 63-0

    // get y component and flip sign if necessary
    y = (pt * sin_table[phiQ1]) >> PT_SIZE;
    if( phi<0 ) y = -y;

    return;
}





//
// This is not the best way, I think, since numbers 513-1023 all map to 1 !
//   TODO - come back to this
//
#define INV_TAB_SIZE (1<<(PT_SIZE))
// return the inverse of a 'pt_size' bits (1024) number
template<class pt_T>
void init_inv_table(pt_T table_out[INV_TAB_SIZE]) {
    // multiply result by 1=2^(PT-SIZE)
    table_out[0]=(1<<PT_SIZE)-1;
    for (int i = 1; i < INV_TAB_SIZE; i++) {
        table_out[i] = (1<<PT_SIZE) / float(i);
    }
    return;
}

#define ATAN_SIZE (PHI_SIZE-3)
#define ATAN_TAB_SIZE (1<<ATAN_SIZE)
// Get arctan of a number in (0,1), represented as integers 0 to 2^(pt_size)=1024
// for a 1-1 mapping, we can get away with an eight of the bits used for full phi
template<class phi_T>
void init_atan_table(phi_T table_out[ATAN_TAB_SIZE]) {
    // multiply result by 1=2^(PT-SIZE)
    table_out[0]=0;
    for (int i = 1; i < ATAN_TAB_SIZE; i++) {
        table_out[i] = atan(float(i)/ATAN_TAB_SIZE) * (1<<(PHI_SIZE-3)) / (FLOATPI/4);
        if(0){
            std::cout << " -> ";
            std::cout << i << "  ";
            std::cout << float(i)/ATAN_TAB_SIZE << "  ";
            std::cout << atan(float(i)/ATAN_TAB_SIZE) << "  ";
            std::cout << atan(float(i)/ATAN_TAB_SIZE) * (1<<(PHI_SIZE-3)) / (FLOATPI/4) << "  ";
            std::cout << table_out[i] << "  ";
            std::cout << std::endl;
        }
    }
    return;
}

template<class pxy_T, class phi_T>
void PhiFromXY(pxy_T px, pxy_T py, phi_T &phi){
    // Initialize the lookup tables
#ifdef __HLS_SYN__
    bool initialized = false;
    pt_t inv_table[INV_TAB_SIZE];
    pt_t atan_table[INV_TAB_SIZE];
#else 
    static bool initialized = false;
    static pt_t inv_table[INV_TAB_SIZE];
    static pt_t atan_table[INV_TAB_SIZE];
#endif
    if (!initialized) {
        init_inv_table(inv_table);
        init_atan_table(atan_table);
        initialized = true;
    }

    if(px==0 && py==0){ phi = 0; return; }

    // TODO check if these cases are handled properly...
    //if(px==0 && py >0){ phi = 0; return; }

    // get q1 coordinates
    pt_t x =  px; //px>=0 ? px : -px;
    pt_t y =  py; //py>=0 ? py : -py;
    if(px<0) x = -px;
    if(py<0) y = -py;
    // transform so a<b
    pt_t a = x; //x<y ? x : y;
    pt_t b = y; //x<y ? y : x;
    if(a>b){ a = y; b = x; }

    // get a/b <= 1
    pt_t inv_b = inv_table[b];
    pt_t a_over_b = a * inv_b; // x 2^(PT_BITS)
    ap_uint<ATAN_SIZE> atan_index = a_over_b >> (PT_SIZE-ATAN_SIZE); // keep only most significant bits
    phi = atan_table[atan_index];

    // rotate from (0,pi/4) to full quad1
    if(y>x) phi = (1<<(PHI_SIZE-2)) - phi; //phi = pi/2 - phi
    // other quadrants
    if( px < 0 && py > 0 ) phi = (1<<(PHI_SIZE-1)) - phi;    // Q2 phi = pi - phi
    if( px > 0 && py < 0 ) phi = -phi;                       // Q4 phi = -phi
    if( px < 0 && py < 0 ) phi = -((1<<(PHI_SIZE-1)) - phi); // Q3 composition of both

    // TODO - CHECK rotate phi back
    // TODO - CHECK px==0, py==0 cases

    if(0){
        std::cout << " ---> ";
        std::cout << a << "  ";
        std::cout << b << "  ";
        std::cout << inv_b << "  ";
        std::cout << a*inv_b << "  ";
        std::cout << atan_index << "  ";
        std::cout << phi << "  ";
        std::cout << " ---> ";
        std::cout << 1./float(b) << "  ";
        std::cout << (1<<PT_SIZE)/float(b) << "  ";
        std::cout << float(a)/float(b) << "  ";
        std::cout << atan(float(a)/float(b)) << "  ";
        std::cout << atan(float(a)/float(b))*(1<<(PHI_SIZE-3))/(FLOATPI/4) << "  ";
        std::cout << std::endl;
    }

    return;
}



// Some general comments...
// division = multiplication and bit shift
// if a, b uint<16>, then a in (0,2^16-1) and 1==2^16
// then 1/b=2^16/b and a/b=a*(2^16/b)
// can convert to decimal by shifting 16 bits
// a/b = a*(2^16/b) >> 16














#endif
