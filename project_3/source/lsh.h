#ifndef LSH_H
#define LSH_H

#include "common.h"

#include "hash.hpp"
#include "my_rand.h"
#include "data_handler.h"
#include "vec_math.h"

/*
	Key idea:
		- Create many hash tables
		- Use a hash function with a good chance to put points that are near each other at same bucket
		- Check bucket of query at many tables and get near point with high chance

	The hard part:
		- Find a hash function that respects geometry and has requested properties
		- LSH Family of hash functions H: 
			+ h in H if:
				=> close points -> high chance of same h value (same bucket)
				=> far points -> small chance of same h value (same bucket)
		- Hash function choice depends on metric of the problem
		- Best choice for eucliean metric is projection on line (buckets will be line parts on line)

	ANN:
		- let query q
		- hash q at each table
		- get items in bucket of each table
		- with good chance, one of the items is a good ANN of q

	H: projection on line
	h: parameters that define the line of projection
	L: number of hash buckets ( = 5 or 6 up to 30)
	
	each hash function will be made up of a combination of h functions
	g: hash function using a number of h functions (so L in number)
	k: number of h functions used for a g (= 4 to 10)

	preproccessing:
		L*n*[copmplexity of g] time to put all points in the hash tables.

	g construction or amplification: 
		- choose k randomly chosen h functions from H
		- g(p) = sum(i from 1 to k) ri*hi(p) % M (M is size of table = L?, ri random natural)
			+ M 

	h(p) = floor((p*v+t)/w) -> in Z

	v: d dimensional vector in Normal distribuion (0,1)^d
	p: any given d dimensional point
	t: uniformaly and randomly chosen disturbance in [0,w)
	w: parameter in [2,6]

*/


class LSH { 

public:

	static void setup(int L=13, int M=1998, int k=7, int w=3);
	static int query_NN(Datapoint query);
	static std::vector<int> query_KNN(Datapoint query,int K);
	static std::vector<int> query_range(Datapoint query,double R);
	static void print_bucket_info();

private:

	static std::vector<HashTable> hash_tables;
	static std::vector<HashFunction_g> g_functions;

	static int L; // number of hash tables (5 or 6 up to 30)
	static int M; // number of buckets in each hash table (should be large)
	static int k; // number of h hash functions to create a g hash function (sould be from 4 to 10)
	static int w; // window of h hash functions (should be from 2 to 6)

};

#endif