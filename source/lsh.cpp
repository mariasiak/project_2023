#include "lsh.h"

std::vector<HashTable> LSH::hash_tables;
std::vector<HashFunction_g> LSH::g_functions;

int LSH::L;
int LSH::M;
int LSH::k;
int LSH::w;

void LSH::setup(int LL, int MM, int kk, int ww){

	// set parameters
	L=LL; M=MM; k=kk; w=ww;

	// use random k and w if k==0 or w==0 respectively
	if (k==0) k=MyRand::U_int(4,10);
	if (w==0) w=MyRand::U_int(2,6);

	// create the g hash functions
	for (int i=0;i<L;i++) {
        g_functions.push_back(HashFunction_g(k,M,w));
    }
    
    // fill all the buckets of all the hash tables with points
    for(int i=0;i<L;i++){
    	
    	//setup up hash table
    	hash_tables.push_back(HashTable());
		for(int j=0;j<M;j++){
			hash_tables[i].push_back(Bucket());
		}

    	for(int j=0;j<DataHandler::get_dataset_size();j++){
    		/*
    			- i: the hash table index
    			- j: the data point index inside the database
				- hash_tables[i]: the ith hash table
				- g_functions[i]: the g hash function of the ith hash table
				- DataHandler::get_data_point_at(j): the jth data point
				- g_functions[i].g(DataHandler::get_data_point_at(j)): 
					the bucket index of the ith hash table that the jth data point should be placed at

				=> the next line of code places the index j of the data point at the proper bucket
				   of the ith hash table by using the ith hash function
    		*/
    		hash_tables[i][g_functions[i].g(DataHandler::get_data_point_at(j))].push_back(j);
    	}
    }
}

int LSH::query_NN(Datapoint query){

	// setup result
	bool first_run=true;
	int result_index;
	double result_distance;

	// run query through all hash tables
	for(int i=0;i<L;i++){
		// search each bucket for closest point
		int query_bucket_index=g_functions[i].g(query);
		for(int datapoint_index:hash_tables[i][query_bucket_index]) {
			double datapoint_distance=VecMath::dist(query,DataHandler::get_data_point_at(datapoint_index));

			// update result
			if(first_run || datapoint_distance<result_distance) {
				first_run=false;
				result_index=datapoint_index;
				result_distance=datapoint_distance;
			}
		}
	}

	return result_index;
}

// find K nearest neighbors using LSH
std::vector<int> LSH::query_KNN(Datapoint query,int K){
	// setup result
	std::vector<int> result_indexes;
	std::vector<double> result_distances;
	int worst_neighbor_index=0;

	// run query through all hash tables
	for(int i=0;i<L;i++){
		// search each bucket for closest point
		int query_bucket_index=g_functions[i].g(query);
		for(int datapoint_index:hash_tables[i][query_bucket_index]) {
			double datapoint_distance=VecMath::dist(query,DataHandler::get_data_point_at(datapoint_index));
			int neighbors_found=result_indexes.size();

			// push first K points found
			if(neighbors_found<K) {
				result_indexes.push_back(datapoint_index);
				result_distances.push_back(datapoint_distance);
				
				// update worst (furthest) neighbor index
				if(datapoint_distance>result_distances[worst_neighbor_index]) {
					worst_neighbor_index=neighbors_found;
				}
			} else if(datapoint_distance<result_distances[worst_neighbor_index]) {
				// replace worse (furthest) neighbor with new neighbor found
				result_indexes[worst_neighbor_index]=datapoint_index;
				result_distances[worst_neighbor_index]=datapoint_distance;

				// update worse (furthest) neighbor index
				auto max_distance_iterator = std::max_element(result_distances.begin(), result_distances.end());
				worst_neighbor_index = std::distance(result_distances.begin(), max_distance_iterator);
			}
		}
	}

	// sort the result before returing it
	return VecMath::sort_indexes(result_indexes,result_distances);
}

// perform range search using LSH
std::vector<int> LSH::query_range(Datapoint query,double R){
	// setup result
	std::vector<int> result_indexes;
	std::vector<double> result_distances;

	// run query through all hash tables
	for(int i=0;i<L;i++){
		// search each bucket for points in range
		int query_bucket_index=g_functions[i].g(query);
		for(int datapoint_index:hash_tables[i][query_bucket_index]) {
			double datapoint_distance=VecMath::dist(query,DataHandler::get_data_point_at(datapoint_index));

			if(datapoint_distance<R) {
				result_indexes.push_back(datapoint_index);
				result_distances.push_back(datapoint_distance);
			}
		}
	}

	// sort the result before returing it
	return VecMath::sort_indexes(result_indexes,result_distances);
}

void LSH::print_bucket_info() {
	for(int i=0;i<hash_tables.size();i++) {
		for(int j=0;j<hash_tables[i].size();j++){
			std::cout<<"Bucket "<<i<<","<<j<<" has size: "<<hash_tables[i][j].size()<<std::endl;
		}
	}
}