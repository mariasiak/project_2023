#ifndef VEC_MATH_H
#define VEC_MATH_H

#include "common.h"

/*
	A static module with vector math inside the specification of the project
*/

class VecMath { 

public:
	static Datapoint add(Datapoint v1, Datapoint v2, bool bounds=false);
	static Datapoint sub(Datapoint v1, Datapoint v2);
	static Datapoint mul(Datapoint v1, Datapoint v2);
	static Datapoint div(Datapoint v1, int l);
	static int ip(Datapoint v1, Datapoint v2);
	static double ip(Datapoint v1, std::vector<double> v2);
	static double ip(std::vector<double> v1, std::vector<double> v2);
	static int ip_mod_M(std::vector<int> v1, std::vector<int> v2, int M);
	static double dist(Datapoint v1, Datapoint v2, int metric=2);
	static double dist_o(Datapoint v1, Datapoint v2, int metric=2);
	static Datapoint zero_vec(int d=DIMENSIONS);
	static double mean_distance_datasets(Dataset d1, Dataset d2);
	static std::vector<int> sort_indexes(std::vector<int> indexes,std::vector<double> distances);
	static void print_vec(std::vector<int> v);
};

#endif