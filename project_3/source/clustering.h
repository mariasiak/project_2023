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
	static void run_reverse_range_lsh(int LSH_L=5, int LSH_k=10);
	static void run_reverse_range_hc(int HC_k=4,int HC_M=10,int HC_probes=3);
	static std::vector<double> get_silhouette_scores();
	static std::vector<Cluster> get_clusters();

private:
	static int k;
	static std::vector<Cluster> clusters;

	// functions
	static void k_means_pp_init();
	static double loyd_step();
};

#endif
