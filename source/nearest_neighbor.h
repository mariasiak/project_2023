#ifndef NEAREST_NEIGHBOR_H
#define NEAREST_NEIGHBOR_H

#include "common.h"
#include "data_handler.h"
#include "vec_math.h"

/*
	A static module with a brute force nearest neighbor implementation.
*/

class NearestNeighbor { 

public:
	static int run(Datapoint query);
	static int run(Datapoint query,Dataset& points);
	static int run(Datapoint query,Dataset& points,int k);
	static int run(Datapoint query,std::vector<int>& point_indexes);
	static std::vector<int> query_KNN(Datapoint query,int K);
};

#endif