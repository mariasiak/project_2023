#include "nearest_neighbor.h"

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