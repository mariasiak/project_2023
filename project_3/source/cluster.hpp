#ifndef CLUSTER_H
#define CLUSTER_H

#include "common.h"

#include "data_handler.h"
#include "vec_math.h"

/*
    A module with a cluster struct.
    
    This an hpp file and also contains the implementations of the struct (not just declerations).
*/

struct Cluster {
    Datapoint centroid;
    Bucket    point_indexes; 

    Cluster(){}

    Cluster(Bucket _point_indexes) {
        point_indexes = _point_indexes;
    }

    // calculate new centroid from points of cluster c=sum(p[i])/d (mean of points)
    void calculate_centroid() {
        // c=0 (vector)
        Datapoint new_centroid=VecMath::zero_vec();
        int number_of_points=point_indexes.size();
        for(int i=0;i<number_of_points;i++) {
            // c += p[i]
            new_centroid=VecMath::add(new_centroid,DataHandler::get_data_point_at(point_indexes[i]));
        }
        // c/=number_of_points
        centroid=VecMath::div(new_centroid,number_of_points);        
    }
};

#endif