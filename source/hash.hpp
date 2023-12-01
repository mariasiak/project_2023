#ifndef HASH_H
#define HASH_H

#include "common.h"

#include "vec_math.h"
#include "my_rand.h"

/*
	A module with h and g hash function structs for lsh
    and f hash function for hypercube.
    
    This an hpp file and also contains the implementations of the structs (not just declerations).
*/

/*
    The base hash function h(p) for LSH:

    h(p) = floor((p*v+t)/w) -> in Z

    v: d dimensional vector in Normal distribuion (0,1)^d
    p: any given d dimensional point
    t: uniformaly and randomly chosen disturbance in [0,w)
    w: parameter in [2,6]
*/

struct HashFunction_h {
    std::vector<double> v;   // in N(0,1)^DIMENSIONS
    double w;                // set parameter in [2,6]
    double t;                // in U[0,w]

    // hash function constructor
    HashFunction_h(double ww) {
        // set w - no check for range [2,6]! 
        w = ww;

        // generate v in N(0,1)
        for(int i=0;i<DIMENSIONS;i++) {
            v.push_back(std::abs(MyRand::N())); /*TODO just using abs for now*/
        }

        // generate t in U_float[0,w]
        t = MyRand::U_float(0.0001,w);
    }

    // hash a data point p
    int h(Datapoint p) {
        // h(p) = floor((p*v+t)/w) -> in Z
        return std::floor((VecMath::ip(p,v)+t)/w);
    }

};

/*
    g construction or amplification: 
        - choose k randomly chosen h functions from H
        - g(p) = sum(i from 1 to k) ri*hi(p) % M (M is size of table = L?, ri random natural)
*/

struct HashFunction_g {
    std::vector<HashFunction_h> h_hash_functions; // will hold k random h_hash_functions
    std::vector<int> ris; // will hold k ris to create the inner product of g using the h_hash_functions
    int N; // maximum number for random generation of ris
    int k; // k in [4,10] of h functions used for a g 
    int M; // M is number of buckets (should be large)
    int w; // w in [2,6] used in all h functions
    
    // hash function constructor
    HashFunction_g(int kk, int MM, int ww) {
        // set k - no check for range [4,10]! 
        k = kk;

        // set M
        M = MM;

        // set w
        w=ww;

        // add h functions to h_hash_functions
        for (int i=0;i<k;i++) {
            h_hash_functions.push_back(HashFunction_h(w));
        }

        // generate ris in U_int[1,N]
        N=123456;
        for (int i=0;i<k;i++) {
            ris.push_back(MyRand::U_int(1,N));
        }
    }

    // hash a data point p
    int g(Datapoint p) {
        // calculate h(p) for all h functions in h_hash_functions
        std::vector<int> calculated_h_hash_functions;
        for (int i=0;i<k;i++) {
            calculated_h_hash_functions.push_back(h_hash_functions[i].h(p));
        } 
        // g(p) = sum(i from 1 to k) ri*hi(p) % M = R ip(modM) H
        return VecMath::ip_mod_M(ris,calculated_h_hash_functions,M); 
    }

};

/*
    f hash function N ->(randomly)-> {0,1}:
        - f(x) -> {0,1}
        - if x has been mapped to 0 or 1 return f(x) in {0,1}
        - if x has not been mapped, flip a coin on {0,1}, map x to result
*/

struct HashFunction_f {
    std::map<int, int> hash_map;

    // hash an x
    int f(int x) {
        // check if the x has been mapped already
        if (hash_map.find(x) != hash_map.end()) {
            // if x has already been mapped return f(x)
            return hash_map[x];
        } else {
            // if x has not been mapped flip a coin, save the pair, return coin result
            int coin = MyRand::U_int(0,1);
            hash_map[x] = coin;
            return coin;
        }
    }

};

#endif