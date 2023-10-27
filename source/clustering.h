#ifndef CLUSTERING_H
#define CLUSTERING_H

#include "common.h"

#include "cluster.hpp"
#include "my_rand.h"
#include "nearest_neighbor.h"
#include "lsh.h"
#include "hypercube.h"

class Clustering {

public:
	static void setup(int k);
	static void print_centroids();
	static void run_loyds();
	static void run_reverse_range_lsh();
	static void run_reverse_range_hc();
	static double get_silhouette_score();

private:
	static int k;
	static std::vector<Cluster> clusters;

	// functions
	static void k_means_pp_init();
	static double loyd_step();
};

#endif
