#include "vec_math.h"

// add vectors respecting v[d] in [MIN_VALUE,MAX_VALUE] if bounds == true
Datapoint VecMath::add(Datapoint v1, Datapoint v2, bool bounds){
	Datapoint result;
	for(int d=0;d<DIMENSIONS;d++) {
		if (bounds) {
			result.push_back(std::min(MAX_VAL,v1[d]+v2[d]));
		} else {
			result.push_back(v1[d]+v2[d]);
		}
	}
	return result;
}

// add vectors respecting v[d] in [MIN_VALUE,MAX_VALUE]
Datapoint VecMath::sub(Datapoint v1, Datapoint v2){
	Datapoint result;
	for(int d=0;d<DIMENSIONS;d++) {
		result.push_back(std::max(MIN_VAL,v1[d]-v2[d]));
	}
	return result;
}

// multiply vectors by field respecting v[d] in [MIN_VALUE,MAX_VALUE]
Datapoint VecMath::mul(Datapoint v1, Datapoint v2){
	Datapoint result;
	for(int d=0;d<DIMENSIONS;d++) {
		result.push_back(v1[d]*v2[d]/MAX_VAL);
	}
	return result;
}

// divide all fields of vector with l
Datapoint VecMath::div(Datapoint v1, int l){
	Datapoint result=v1;
	for(int d=0;d<DIMENSIONS;d++) {
		result[d]/=l;
	}
	return result;
}

// return inner product of two vectors
int VecMath::ip(Datapoint v1, Datapoint v2){
	int result=0;
	for(int d=0;d<DIMENSIONS;d++) {
		result+=v1[d]*v2[d];
	}
	return result;
}

// return inner product of two vectors (second is double)
double VecMath::ip(Datapoint v1, std::vector<double> v2) {
	double result=0;
	for(int d=0;d<DIMENSIONS;d++) {
		result+=v1[d]*v2[d];
	}
	return result;	
}

// return inner product of two vectors (both double)
double VecMath::ip(std::vector<double> v1, std::vector<double> v2) {
	double result=0;
	for(int d=0;d<DIMENSIONS;d++) {
		result+=v1[d]*v2[d];
	}
	return result;	
}

// return inner product of two vectors mod M (do mod M at each adittion to ease calculations)
int VecMath::ip_mod_M(std::vector<int> v1, std::vector<int> v2, int M) {
	int result=0;
	for(int d=0;d<DIMENSIONS;d++) {
		result+=v1[d]*v2[d];
		result%=M;
	}

	// make sure result is in [0,M)
	while(result<0) result+=M;

	return result;		
}


// return distance with given metric of two vectors (default is euclidean)
double VecMath::dist(Datapoint v1, Datapoint v2, int metric){
	double result=0.0;

	for(int d=0;d<DIMENSIONS;d++) {
		result+=std::pow(std::abs(v1[d]-v2[d]),metric);
	}

	return pow(result,1.0/metric);
}

// return a d-dimensional vector filled with 0s
Datapoint VecMath::zero_vec(int d){
	Datapoint result;
	for(int i=0;i<d;i++) {
		result.push_back(0);
	}
	return result;
}

// calculate the mean distance of p1[i] and p2[i] for i in [0,dataset_size]
double VecMath::mean_distance_datasets(Dataset d1, Dataset d2){
	int dataset_size=d1.size();

	double mean_distance = 0.0;
	for(int i=0;i<dataset_size;i++){
		mean_distance+=VecMath::dist(d1[i],d2[i])/dataset_size;
	}

	return mean_distance;
}

// sort indexes based on distances
std::vector<int> VecMath::sort_indexes(std::vector<int> indexes,std::vector<double> distances){
	// sort the result before returing it
	std::vector<int> sorted_indexes;

    // initialize indexes with 0,1,2,...
    for (int i=0; i<distances.size(); i++) {
        sorted_indexes.push_back(i);
    }

	// sort indexes based on result distances
	std::sort(sorted_indexes.begin(), sorted_indexes.end(), 
		[&distances](double a, double b) {
        	return distances[a] < distances[b];
    	});

    // Rearrange 'data' in place based on the sorted indexes
    std::vector<int> sorted_result_indexes;
    for (int i=0; i<sorted_indexes.size(); i++) {
        sorted_result_indexes.push_back(indexes[sorted_indexes[i]]);
    }	

    return sorted_result_indexes;
}

void VecMath::print_vec(std::vector<int> v) {
	for(int i=0;i<v.size();i++) {
		std::cout<<v[i]<<" ";
	}
	std::cout<<std::endl;
}