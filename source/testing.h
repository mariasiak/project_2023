#ifndef TESTING_H
#define TESTING_H

#include "common.h"
#include "data_handler.h"
#include "vec_math.h"
#include "nearest_neighbor.h"
#include "hash.hpp"
#include "lsh.h"
#include "hypercube.h"
#include "cluster.hpp"
#include "clustering.h"

/*
	A static module with tester mains used during development.
*/

class Testing { 

public:

	static void reverse_range_clustering();
	static void HC_KNN_RS();
	static void LSH_KNN_range();
	static void LSH_range_search();
	static void LSH_KNN();
	static void Clustering_Mac_Queen();
	static void Clustering_testing_k_means_pp();
	static void Clustering_testing_init();
	static void Clustering_testing();
	static void Cluster_testing();
	static void HC_testing();
	static void HC_f_hash();
	static void LSH_testing();
	static void LSH_prototyping_g_struct();
	static void LSH_prototyping_h_g();
	static void LSH_prototyping_hash_function_h();
	static void data_exploration();
	static void nearest_neighbor();
	static void vec_math();
	static void data_handler_test();
};

#endif