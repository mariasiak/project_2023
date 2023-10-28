#include "nearest_neighbor.h"

// find K nearest neighbors using LSH
std::vector<int> NearestNeighbor::query_KNN(Datapoint query,int K){
	// setup result
	int size=DataHandler::get_dataset_size();
	std::vector<int> result_indexes;
	std::vector<double> result_distances;
	int worst_neighbor_index=0;

	for(int i=0;i<size;i++){
	
		int datapoint_index=i;
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

	// sort the result before returing it
	return VecMath::sort_indexes(result_indexes,result_distances);
}


int NearestNeighbor::run(Datapoint query) {
	
	int size=DataHandler::get_dataset_size();
	int nn_index=0;
	double distance=VecMath::dist(query,DataHandler::get_data_point_at(0));

	for (int i=1; i<size; i++) {
		double temp_distance=VecMath::dist(query,DataHandler::get_data_point_at(i));
		if(temp_distance<distance) {
			distance=temp_distance;
			nn_index=i;
		}
	}

	return nn_index;
}

int NearestNeighbor::run(Datapoint query,Dataset points) {
	int size=points.size();
	int nn_index=0;
	double distance=VecMath::dist(query,points[0]);

	for (int i=1; i<size; i++) {
		double temp_distance=VecMath::dist(query,points[i]);
		if(temp_distance<distance) {
			distance=temp_distance;
			nn_index=i;
		}
	}

	return nn_index;	
}

// int k defines an index to skip
int NearestNeighbor::run(Datapoint query,Dataset points,int k) {
	int size=points.size();
	int nn_index=0;
	double distance=VecMath::dist(query,points[0]);

	if(k==0) {
		nn_index=1;
		distance=VecMath::dist(query,points[1]);
	}

	for (int i=1; i<size; i++) {
		if(i==k) continue;
		double temp_distance=VecMath::dist(query,points[i]);
		if(temp_distance<distance) {
			distance=temp_distance;
			nn_index=i;
		}
	}

	return nn_index;	
}

// find nearest neighbor in databes given indexes instead of points
int NearestNeighbor::run(Datapoint query,std::vector<int> point_indexes) {
	int size=point_indexes.size();
	int nn_index=0;
	double distance=VecMath::dist(query,DataHandler::get_data_point_at(point_indexes[0]));

	for (int i=1; i<size; i++) {
		double temp_distance=VecMath::dist(query,DataHandler::get_data_point_at(point_indexes[i]));
		if(temp_distance<distance) {
			distance=temp_distance;
			nn_index=i;
		}
	}

	return nn_index;	
}