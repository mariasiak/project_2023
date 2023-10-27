#ifndef HYPERCUBE_H
#define HYPERCUBE_H

#include "common.h"
#include "hash.hpp"
#include "data_handler.h"

/*

	dimensionality reduction:
		- choose d in [floor(logn)-1,floor(logn)-3]
		- project from DIMENSIONS to d
		- hope that properties of vicinity are retain in lesser dimensions
		- how?

	hypercube idea:
		- create 2^d buckets each with binary representation
		- so the buckets are 00..00, 00..01, 00..10, 00..11, ... , 11..11
		- create family of functions fi that randomly map integer values to {0,1}
		- choose d of these functions
		- use them to map values to {0,1}^d
		- how?

	h,f mapping:
		- choose datapoint p
		- parse it through d hi (from LSH) hash functions to get:
			{h1(p), h2(p), ... , hd(p)} in N^d
		- parse these values, each through fi, to get:
			{f1(h1(p)), f2(h2(p)), ... , fd(hd(p))} in {0,1}^d
		- so in summary:
			p -> {f1(h1(p)), f2(h2(p)), ... , fd(hd(p))} in {0,1}^d
		- do this for all datapoints in data set to fill buckets of hypercube

	querying:
		- to get results on a query, parse the query through:
			q -> {f1(h1(q)), f2(h2(q)), ... , fd(hd(q))} in {0,1}^d
		- search the resulting bucket and nearby buckets
		- nearby buckets are found with HAMMING distance
	
*/


class Hypercube { 

public:

	static void setup(int d=10,int w=3);
	static int query_NN(Datapoint query,int max_depth=1);
	static std::vector<int> query_KNN(Datapoint query,int K,int max_depth=1);
	static std::vector<int> query_range(Datapoint query,double R,int max_depth=1);

private:

	static std::vector<HashFunction_f> f_functions;
	static std::vector<HashFunction_h> h_functions;
	static HashTable hypercube_buckets;

	static int d; // number of reduced dimensions
	static int w; // window for h functions

	// helper functions
	static int get_bucket_of_point(Datapoint p);
	static std::vector<int> int_to_bin(int n);
	static int bin_to_int(std::vector<int> b);
	static std::vector<std::vector<int>> hamming_dist_one(std::vector<int> b);
	static std::vector<int> get_next_buckets(std::vector<int> buckets_to_be_checked, std::vector<bool> buckets_checked,std::vector<bool>& already_pushed);


};

#endif