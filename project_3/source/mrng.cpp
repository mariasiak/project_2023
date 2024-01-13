#include "mrng.h"

std::vector<std::vector<int>> MRNG::search_graph;
std::vector<std::vector<int>> MRNG::mrng_search_graph;
int MRNG::starting_point_index;

void MRNG::setup(int LSH_L, int LSH_M, int LSH_k, int I){

	// run LSH
    std::cout<<"\nRunning LSH.\n";
    LSH::setup(LSH_L,LSH_M,LSH_k);
    std::cout<<"Dataset hashed successfully.\n\n";

    // print progress message
    std::cout<<"Generating search graph:"<<std::endl;
    std::cout<<"Points parsed:"<<std::endl;

    // run k-NN LSH queries for the whole dataset to create the nearest search graph
    for(int i=0;i<MAX_DATA_SIZE;i++) {
    	// get ith datapoint
        Datapoint datapoint = DataHandler::get_data_point_at(i);
        // find its k nearest neighbors
        std::vector<int> lsh_knn_indexes = LSH::query_KNN(DataHandler::get_data_point_at(i),LSH_k);
        // add k neighbors to search graph
        search_graph.push_back(lsh_knn_indexes);
        // print progress message
        if(i%1000==0) {
            std::cout<<i<<"/"<<MAX_DATA_SIZE<<std::endl;
        }

    }

    // create mrng search graph
    for(int i=0;i<MAX_DATA_SIZE;i++) {
        // get ith datapoint
        Datapoint datapoint = DataHandler::get_data_point_at(i);
        
        // create list of canditates
        std::vector<int> candidate_indexes;
        for(int j=2;j<=I;j++) {
            candidate_indexes.push_back(search_graph[i][j]);
        }

        // set nearest neighbor index and add to vector
        int nearest_neighbor_index=search_graph[i][1];
        std::vector<int> neighbors;
        neighbors.push_back(nearest_neighbor_index);

        // try to find more neighbors

        /*
            triangle (prt):
                p -> datapoint
                t -> point at nearest index
                r -> point at canditate index
        */

        // for all canditates
        Datapoint p=datapoint;
        for (int candidate_index:candidate_indexes) {
            bool condition=true;
            Datapoint r=DataHandler::get_data_point_at(candidate_index);

            double pr_dist=VecMath::dist(p,r);

            // for all neighbors
            for (int neighbor_index:neighbors){

                // skip same indexes
                if(candidate_index==neighbor_index) {
                    continue;
                }

                Datapoint t=DataHandler::get_data_point_at(neighbor_index);

                double pt_dist=VecMath::dist(p,t);
                double rt_dist=VecMath::dist(r,t);

                // if (pr) longest edge in triangle (prt) 
                if(pr_dist>pt_dist && pr_dist>rt_dist) {
                    condition=false;
                    break;
                }
            }   

            if (condition==true) {
                neighbors.push_back(candidate_index);
            }
        }

        
        // add neighbors to graph
        mrng_search_graph.push_back(neighbors);

    }

    /*TODO REMOVE PRINTS?*/
    // print the mrng graph (point index and indexes of k neighbors)
    for(int i=0;i<MAX_DATA_SIZE;i++) {
    // print datapoint_index : [nn1_index, ..., nnk_index]
        std::cout<<i<<" : {";
        std::vector<int> &nn_indexes=mrng_search_graph[i];
        for(int nni=0;nni<nn_indexes.size();nni++) {
            std::cout<<nn_indexes[nni];
            if(nni!=nn_indexes.size()-1) std::cout<<",";
        }
        std::cout<<"}"<<std::endl;
    }

    // find starting search point (nearest to centroid of dataset)
    
    // c=0 (vector)
    Datapoint centroid=VecMath::zero_vec();
    for(int i=0;i<MAX_DATA_SIZE;i++) {
        // c += p[i]
        centroid=VecMath::add(centroid,DataHandler::get_data_point_at(i));
    }
    // c/=number_of_points
    centroid=VecMath::div(centroid,MAX_DATA_SIZE);  

    // get index of point closest to centroid
    starting_point_index = NearestNeighbor::run(centroid);

    // print starting point index
    std::cout<<"starting_point_index="<<starting_point_index<<std::endl;
}

std::vector<int> MRNG::query_KNN(Datapoint query,int N){
	
	std::vector<int> result_indexes;
    std::vector<double> result_distances;

    // T: number of steps
    int T=50;

    // choose starting point
    int current_point_index=starting_point_index;

    // save at result
    result_indexes.push_back(current_point_index);
    double point_distance=VecMath::dist(query,DataHandler::get_data_point_at(current_point_index));
    result_distances.push_back(point_distance);

    for(int t=1;t<T;t++) {

        // set new current point index to neighbor of previously current point closest to query
        int nn_index = NearestNeighbor::run(query,mrng_search_graph[current_point_index]);
        current_point_index = mrng_search_graph[current_point_index][nn_index]; 

        // update result
        result_indexes.push_back(current_point_index);
        double point_distance=VecMath::dist(query,DataHandler::get_data_point_at(current_point_index));
        result_distances.push_back(point_distance);

        //std::cout<<current_point_index<<" - "<<point_distance<<std::endl;
    }


    // sort the result
    std::vector<int> sorted_result_indexes = VecMath::sort_indexes(result_indexes,result_distances);

    // keep only unique indexes
    std::vector<int> unique_sorted_result_indexes;

    for(int sri:sorted_result_indexes) {
        bool found=false;

        for(int usri:unique_sorted_result_indexes) {
            if (sri==usri) {
                found=true;
            }
        }

        if (found==false) {
            unique_sorted_result_indexes.push_back(sri);
        } 
    }


    // return only the first N elements
    int offset=std::min(N,int(unique_sorted_result_indexes.size()));
    auto vec_start=unique_sorted_result_indexes.begin();
	return std::vector<int>(vec_start, vec_start + offset);
}