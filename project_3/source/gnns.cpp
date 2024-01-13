#include "gnns.h"

std::vector<std::vector<int>> GNNS::search_graph;

void GNNS::setup(int LSH_L, int LSH_M, int LSH_k){

	// run LSH
    std::cout<<"\nRunning LSH.\n";
    LSH::setup(LSH_L,LSH_M,LSH_k);
    std::cout<<"Dataset hashed successfully.\n\n";

    // print progress message
    std::cout<<"Generating search graph:"<<std::endl;
    std::cout<<"Points parsed:"<<std::endl;

    // run k-NN LSH queries for the whole dataset to create the GNNS graph
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

    /*TODO REMOVE PRINTS?*/
    // print the graph (point index and indexes of k neighbors)
    for(int i=0;i<MAX_DATA_SIZE;i++) {
    // print datapoint_index : [nn1_index, ..., nnk_index]
        std::cout<<i<<" : [";
        std::vector<int> &lsh_knn_indexes=search_graph[i];
        for(int nni=0;nni<lsh_knn_indexes.size();nni++) {
            std::cout<<lsh_knn_indexes[nni];
            if(nni!=lsh_knn_indexes.size()-1) std::cout<<",";
        }
        std::cout<<"]"<<std::endl;
    }
}

std::vector<int> GNNS::query_KNN(Datapoint query,int N,int E, int R, int T){
	
	std::vector<int> result_indexes;
    std::vector<double> result_distances;

    // R random restarts
    for(int r=1;r<=R;r++) {
        // randomly choose starting point
        int current_point_index=MyRand::U_int(0,MAX_DATA_SIZE);

        // save path through graph, stop if we run on it (avoid circles in search)
        std::vector<int> current_point_path;
        current_point_path.push_back(current_point_index);

        for(int t=1;t<T;t++) {
            // find E expansions
            std::vector<int> expansions_indexes;

            for(int i=0;i<E;i++) {
                int expansion_point_index=search_graph[current_point_index][i];
                expansions_indexes.push_back(expansion_point_index);
                // also add expansion to result indexes
                result_indexes.push_back(expansion_point_index);
                // and save distance from query
                double expansion_point_distance=VecMath::dist(query,DataHandler::get_data_point_at(expansion_point_index));
                result_distances.push_back(expansion_point_distance);
            }

            // set current point index to closest expansion
            int nn_index = NearestNeighbor::run(query,expansions_indexes);
            current_point_index = expansions_indexes[nn_index];


            // check if we have already visited this point in the graph
            bool will_make_circle=false;
            for(int i=0;i<current_point_path.size();i++) {
                if(current_point_index==current_point_path[i]) {
                    will_make_circle=true;
                    break;
                }
            }

            // continuing will result in loops until all greedy steps are made, so break
            if(will_make_circle) {
                break;
            }

            // else, add the point to the path and continue
            current_point_path.push_back(current_point_index);
        }

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