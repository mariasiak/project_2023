#ifndef GNNS_H
#define GNNS_H

#include "../source/common.h"

#include "../source/lsh.h"
#include "../source/my_rand.h"
#include "../source/data_handler.h"
#include "../source/nearest_neighbor.h"

class GNNS { 

public:

	static void setup(int LSH_L=10, int LSH_M=100, int LSH_k=50);
	static std::vector<int> query_KNN(Datapoint query,int N=1, int E=30, int R=1,int T=20);

private:

	static std::vector<std::vector<int>> search_graph;
};

#endif
