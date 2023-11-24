#ifndef MRNG_H
#define MRNG_H

#include "../source/common.h"

#include "../source/lsh.h"
#include "../source/my_rand.h"
#include "../source/data_handler.h"
#include "../source/nearest_neighbor.h"

class MRNG { 

public:

	static void setup(int LSH_L=10, int LSH_M=100, int LSH_k=50, int I=20);
	static std::vector<int> query_KNN(Datapoint query,int N=1);

private:

	static std::vector<std::vector<int>> search_graph;
	static std::vector<std::vector<int>> mrng_search_graph;
	static int starting_point_index;
};

#endif
