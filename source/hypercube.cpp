#include "hypercube.h"


std::vector<HashFunction_f> Hypercube::f_functions;
std::vector<HashFunction_h> Hypercube::h_functions;
HashTable Hypercube::hypercube_buckets;

int Hypercube::d;
int Hypercube::w;

// pass point through all fs and hs to get bucket in binary, then convert to int
int Hypercube::get_bucket_of_point(Datapoint p) {
	std::vector<int> bin;
	for(int i=0; i<d; i++){
		int h_val=h_functions[i].h(p);
		int f_val=f_functions[i].f(h_val);
		bin.push_back(f_val);
		//std::cout<<"h_val = "<<h_val<<" f_val="<<f_val<<std::endl;
	}

	//for(int i=0;i<d;i++)
	//	std::cout<<bin[i]<<" ";
	//std::cout<<" -> "<<bin_to_int(bin)<<std::endl;

	return bin_to_int(bin);
}

// 7 -> 00111
std::vector<int> Hypercube::int_to_bin(int n){
    std::vector<int> bin;
    
    if (n == 0) {
        bin.push_back(0);
    }
    
    while (n > 0) {
        bin.push_back(n % 2);
        n /= 2;
    }
    
    // fill zeros to get d-dimensions
    for(int i=bin.size();i<d;i++) {
    	bin.push_back(0);
    }

    // reverse the vector to get the binary representation
    std::reverse(bin.begin(), bin.end());
    
    return bin;
}

// 00101 -> 1*2^0 + 0*2^1 + 1*2^2 + 0 + ... = 5
int Hypercube::bin_to_int(std::vector<int> b){
	int result=0;
	int pow=1;

	for(int i=1;i<=d;i++){
		result+=b[d-i]*pow;
		pow*=2;
	}

	return result;
}

// find binary numbers that have hamming distance one from given binary number, by changing each digit
std::vector<std::vector<int>> Hypercube::hamming_dist_one(std::vector<int> b) {
	std::vector<std::vector<int>> hamming_neighbors;

	for(int i=0;i<b.size();i++){
		std::vector<int> hamming_neighbor=b;

		// swap 1 with 0 and vice versa at position i
		hamming_neighbor[i]=1-b[i];

		hamming_neighbors.push_back(hamming_neighbor);
	}

	return hamming_neighbors;
}

void Hypercube::setup(int dd, int ww){

	// set d, w
	d=dd; w=ww;

	// use random w if w==0
	if (w==0) w=MyRand::U_int(2,6);

	// use random d in [floor(logn)-1,floor(logn)-3] if d is 0
	if (d==0) {
		int floorlogn = floor(log10(DataHandler::get_dataset_size()));
		d=MyRand::U_int(floorlogn-3,floorlogn-1);
	}

	// create the h and f hash functions
	for (int i=0;i<d;i++) {
        h_functions.push_back(HashFunction_h(w));
        f_functions.push_back(HashFunction_f());
    }
	
	//setup up the hypercube hash table (add 2^d buckets)
	for(int i=0;i<pow(2,d);i++){
		hypercube_buckets.push_back(Bucket());
	}

	// fill the buckets of the hypercube with the data points
	for(int i=0;i<DataHandler::get_dataset_size();i++){
		// find bucket of point
		int bucket_index = get_bucket_of_point(DataHandler::get_data_point_at(i));
		// std::cout<<"bucket_index: "<<bucket_index<<std::endl;
		// put index of point in bucket
		hypercube_buckets[bucket_index].push_back(i);
	}

}

std::vector<int> Hypercube::get_next_buckets(std::vector<int> buckets_to_be_checked, std::vector<bool> buckets_checked,std::vector<bool>& already_pushed){
		// update buckets to be checked with hamming neighbors of lastly checked buckets
		std::vector<int> new_buckets_to_be_checked;

		//std::cout<<"next buckets to be checked: \n";
		// for all lastly checked buckets
		for(int j=0;j<buckets_to_be_checked.size();j++) {
			// find hamming neighbors of binary index
    		int current_bucket_index=buckets_to_be_checked[j];
    		std::vector<std::vector<int>> hamming_neighbors = Hypercube::hamming_dist_one(int_to_bin(current_bucket_index));			

    		// push back all hamming neighbors, that have not already been checked, as integers
    		for(int i=0;i<hamming_neighbors.size();i++) {
    			int bucket_index = bin_to_int(hamming_neighbors[i]);
    			if(buckets_checked[bucket_index]==false && already_pushed[bucket_index]==false) {
    				new_buckets_to_be_checked.push_back(bucket_index);
    				//VecMath::print_vec(int_to_bin(bucket_index));
    				already_pushed[bucket_index]=true;
    			}
    		}
    	}

    	// update buckets to be checked
    	return new_buckets_to_be_checked;
}

// find nearest neighbor using hypercube, max depth is how many buckets away the search will be extended to
int Hypercube::query_NN(Datapoint query,int max_depth){

	// setup result
	bool first_run=true;
	int result_index;
	double result_distance;

	// setup buckets checked
	std::vector<bool> buckets_checked;
	for(int i=0;i<hypercube_buckets.size();i++) {
		buckets_checked.push_back(false);
	}

	/*
		setup buckets pushed
		this vector is used for debugging as buckets_checked already does the job
	*/
	std::vector<bool> already_pushed;
	for(int i=0;i<hypercube_buckets.size();i++) {
		already_pushed.push_back(false);
	}

	// setup buckets to be checked
	std::vector<int> buckets_to_be_checked;
	int bucket_of_query=get_bucket_of_point(query);
	buckets_to_be_checked.push_back(bucket_of_query);
	already_pushed[bucket_of_query]=true;

	//std::cout<<"first bucket to be checked: \n";
	//VecMath::print_vec(int_to_bin(bucket_of_query));

	// run query through all buckets from depth=0 to max_depth
	for(int depth=0;depth<=max_depth;depth++){


		// search for K nearest neighbour in buckets to be checked
		for(int j=0;j<buckets_to_be_checked.size();j++) {

			// get current bucket
			int current_bucket_index=buckets_to_be_checked[j];
			int size_of_bucket=hypercube_buckets[current_bucket_index].size();

			// skip bucket if it is checked
			if(buckets_checked[current_bucket_index]==true) {
				continue;
			}

			// search bucket for best neighbor
			for(int i=0;i<size_of_bucket;i++){
			
				int datapoint_index=hypercube_buckets[current_bucket_index][i];
				
				double datapoint_distance=VecMath::dist(query,DataHandler::get_data_point_at(datapoint_index));
				
				if(first_run || datapoint_distance<result_distance) {
					first_run=false;
					result_index=datapoint_index;
					result_distance=datapoint_distance;
				}

				// mark checked bucket
				buckets_checked[current_bucket_index]=true;
			}

		}

		// if max_depth reached, there is no need to find more neighbors
		if(depth==max_depth) {
			continue;
		}

    	// update buckets to be checked
    	buckets_to_be_checked=get_next_buckets(buckets_to_be_checked, buckets_checked, already_pushed);
	}

	return result_index;

}

// find K nearest neighbors using hypercube, max depth is how many buckets away the search will be extended to
std::vector<int> Hypercube::query_KNN(Datapoint query,int K,int max_depth){

	// setup result
	std::vector<int> result_indexes;
	std::vector<double> result_distances;
	int worst_neighbor_index=0;

	// setup buckets checked
	std::vector<bool> buckets_checked;
	for(int i=0;i<hypercube_buckets.size();i++) {
		buckets_checked.push_back(false);
	}

	/*
	 setup buckets pushed
	 this vector is used for debugging as buckets_checked already does the job
	*/
	std::vector<bool> already_pushed;
	for(int i=0;i<hypercube_buckets.size();i++) {
		already_pushed.push_back(false);
	}

	// setup buckets to be checked
	std::vector<int> buckets_to_be_checked;
	int bucket_of_query=get_bucket_of_point(query);
	buckets_to_be_checked.push_back(bucket_of_query);
	already_pushed[bucket_of_query]=true;

	//std::cout<<"first bucket to be checked: \n";
	//VecMath::print_vec(int_to_bin(bucket_of_query));

	// run query through all buckets from depth=0 to max_depth
	for(int depth=0;depth<=max_depth;depth++){


		// search for K nearest neighbour in buckets to be checked
		for(int j=0;j<buckets_to_be_checked.size();j++) {

			// get current bucket
			int current_bucket_index=buckets_to_be_checked[j];
			int size_of_bucket=hypercube_buckets[current_bucket_index].size();

			// skip bucket if it is checked
			if(buckets_checked[current_bucket_index]==true) {
				continue;
			}

			// search bucket for neighbors
			for(int i=0;i<size_of_bucket;i++){
			
				int datapoint_index=hypercube_buckets[current_bucket_index][i];
				
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

				// mark checked bucket
				buckets_checked[current_bucket_index]=true;
			}

		}

		// if max_depth reached, there is no need to find more neighbors
		if(depth==max_depth) {
			continue;
		}

    	// update buckets to be checked
    	buckets_to_be_checked=get_next_buckets(buckets_to_be_checked, buckets_checked, already_pushed);
	}

	// sort the result before returing it
	return VecMath::sort_indexes(result_indexes,result_distances);

	return result_indexes;
}

// find K nearest neighbors using hypercube, max depth is how many buckets away the search will be extended to
std::vector<int> Hypercube::query_range(Datapoint query,double R,int max_depth){

	// setup result
	std::vector<int> result_indexes;
	std::vector<double> result_distances;
	int worst_neighbor_index=0;

	// setup buckets checked
	std::vector<bool> buckets_checked;
	for(int i=0;i<hypercube_buckets.size();i++) {
		buckets_checked.push_back(false);
	}

	/*
	 setup buckets pushed
	 this vector is used for debugging as buckets_checked already does the job
	*/
	std::vector<bool> already_pushed;
	for(int i=0;i<hypercube_buckets.size();i++) {
		already_pushed.push_back(false);
	}

	// setup buckets to be checked
	std::vector<int> buckets_to_be_checked;
	int bucket_of_query=get_bucket_of_point(query);
	buckets_to_be_checked.push_back(bucket_of_query);
	already_pushed[bucket_of_query]=true;

	//std::cout<<"first bucket to be checked: \n";
	//VecMath::print_vec(int_to_bin(bucket_of_query));

	// run query through all buckets from depth=0 to max_depth
	for(int depth=0;depth<=max_depth;depth++){


		// search for K nearest neighbour in buckets to be checked
		for(int j=0;j<buckets_to_be_checked.size();j++) {

			// get current bucket
			int current_bucket_index=buckets_to_be_checked[j];
			int size_of_bucket=hypercube_buckets[current_bucket_index].size();

			// skip bucket if it is checked
			if(buckets_checked[current_bucket_index]==true) {
				continue;
			}

			// search bucket for neighbors in range
			for(int i=0;i<size_of_bucket;i++){
			
				int datapoint_index=hypercube_buckets[current_bucket_index][i];
				
				double datapoint_distance=VecMath::dist(query,DataHandler::get_data_point_at(datapoint_index));

				if(datapoint_distance<R) {
					result_indexes.push_back(datapoint_index);
					result_distances.push_back(datapoint_distance);
				}

				// mark checked bucket
				buckets_checked[current_bucket_index]=true;
			}

		}

		// if max_depth reached, there is no need to find more neighbors
		if(depth==max_depth) {
			continue;
		}

    	// update buckets to be checked
    	buckets_to_be_checked=get_next_buckets(buckets_to_be_checked, buckets_checked, already_pushed);
	}

	// sort the result before returing it
	return VecMath::sort_indexes(result_indexes,result_distances);

	return result_indexes;
}