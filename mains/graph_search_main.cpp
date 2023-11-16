#include "../source/common.h"
#include "../source/testing.h"

#include "../source/my_rand.h"
#include "../source/data_handler.h"

int main(int argc, char* argv[]) {

    // setup random seeding
    MyRand::setup();

    // setup input variables with deafault values
    std::string input_file="", query_file="", output_file="";
    int k = 50, E = 30, R=1, N = 1, I=20, m=1;

    // read input variables
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

          // -d <input_file>
        if (arg == "-d" && i + 1 < argc) {
            input_file = argv[i + 1];
            i++;
        } // -q <query_file> 
        else if (arg == "-q" && i + 1 < argc) {
            query_file = argv[i + 1];
            i++;
        } // -k <int> : number of nearest neighbors in graph 
        else if (arg == "-k" && i + 1 < argc) {
            k = std::stoi(argv[i + 1]);
            i++;
        } // -E <int> : number of expansions 
        else if (arg == "-E" && i + 1 < argc) {
            E = std::stoi(argv[i + 1]);
            i++;
        } // -R <int> : number of random resets 
        else if (arg == "-R" && i + 1 < argc) {
            R = std::stoi(argv[i + 1]);
            i++;
        } // -N <int> : number of nearest neighbors 
        else if (arg == "-N" && i + 1 < argc) {
            N = std::stoi(argv[i + 1]);
            i++;
        } // -I <int_only_for_search_on_graph> : number for candidate tank
        else if (arg == "-I" && i + 1 < argc) {
            I = std::stoi(argv[i + 1]);
            i++;
        } // -m <1_for_gnns_2_for_mrng> 
        else if (arg == "-m" && i + 1 < argc) {
            m = std::stoi(argv[i + 1]);
            i++;
        } // -o <output_file>  
        else if (arg == "-o" && i + 1 < argc) {
            output_file = argv[i + 1];
            i++;
        }
    }

    // print input variables
    std::cout << "Input variables:" << std::endl;
    std::cout << "----------------" << std::endl;
    std::cout << "Input File: " << input_file << std::endl;
    std::cout << "Query File: " << query_file << std::endl;
    std::cout << "k: " << k << std::endl;
    std::cout << "E: " << E << std::endl;
    std::cout << "R: " << R << std::endl;
    std::cout << "N: " << N << std::endl;
    std::cout << "I: " << I << std::endl;
    std::cout << "m: " << m << std::endl;
    std::cout << "Output File: " << output_file << std::endl;


    // used to rerun algorithm if user enters y after first run
    std::string ans="";
    do {
        // no input file given, ask from user
        if(input_file.size()==0 || ans=="y") {
            std::cout<<"Type path to input file: ";
            std::cin >> input_file;
        }

        // no output file given, ask from user
        if(query_file.size()==0 || ans=="y") {
            std::cout<<"Type path to query file: ";
            std::cin >> query_file;
        }

        // no query file given, ask from user
        if(output_file.size()==0 || ans=="y") {
            std::cout<<"Type path to output file: ";
            std::cin >> output_file;
        }

        // print input variables
        std::cout << "Input variables:" << std::endl;
        std::cout << "----------------" << std::endl;
        std::cout << "Input File: " << input_file << std::endl;
        std::cout << "Query File: " << query_file << std::endl;
        std::cout << "k: " << k << std::endl;
        std::cout << "E: " << E << std::endl;
        std::cout << "R: " << R << std::endl;
        std::cout << "N: " << N << std::endl;
        std::cout << "I: " << I << std::endl;
        std::cout << "m: " << m << std::endl;
        std::cout << "Output File: " << output_file << std::endl;

        // load data and queries
        DataHandler::load_data_mnist(input_file,MAX_DATA_SIZE);
        DataHandler::load_queries_mnist(query_file,MAX_QUERY_SIZE);

        // setup LSH
        int LSH_L=2, LSH_M=50, LSH_k=k;

        std::cout<<"\nRunning LSH.\n";
        LSH::setup(LSH_L,LSH_M,LSH_k);
        std::cout<<"Dataset hashed successfully.\n\n";

        // create and open outpout file for writing
        std::ofstream out_stream(output_file);

        // graph will be stored here
        std::vector<std::vector<int>> search_graph;

        // run k-NN LSH queries for the whole dataset to create the GNNS graph

        std::cout<<"Generating search graph:"<<std::endl;
        std::cout<<"Points parsed:"<<std::endl;

        for(int i=0;i<MAX_DATA_SIZE;i++) {
            Datapoint datapoint = DataHandler::get_data_point_at(i);
            /* TODO: use proper k here, +1 because closest neighbor is always the data point itself */
            int TODO_k=k;
            /* TODO fix LSH */
            //std::vector<int> lsh_knn_indexes = LSH::query_KNN(DataHandler::get_data_point_at(0),TODO_k);
            std::vector<int> lsh_knn_indexes = LSH::query_KNN(DataHandler::get_data_point_at(i),TODO_k);

            // remove index of data point used as query
            //lsh_knn_indexes.erase(std::remove(lsh_knn_indexes.begin(), lsh_knn_indexes.end(), i), lsh_knn_indexes.end());

            if(i%1000==0 && i>0) {
                std::cout<<i<<"/"<<MAX_DATA_SIZE<<std::endl;
            }

            search_graph.push_back(lsh_knn_indexes);
        }

        for(int i=0;i<MAX_DATA_SIZE;i++) {
        //if(i%1000==0) {
        // print datapoint_index : [nn1_index, ..., nnk_index]
            std::cout<<i<<" : [";
            std::vector<int> &lsh_knn_indexes=search_graph[i];
            for(int nni=0;nni<lsh_knn_indexes.size();nni++) {
                std::cout<<lsh_knn_indexes[nni];
                if(nni!=lsh_knn_indexes.size()-1) std::cout<<",";
            }
            std::cout<<"]"<<std::endl;
        //}
        }

        // GNNS Algorithm

        // T : number of greedy steps
        int T=20;

        Datapoint query = DataHandler::get_test_query_at(0);

        std::vector<int> result_indexes;
        std::vector<double> result_distances;

        // R random restarts
        for(int r=1;r<R;r++) {
            // randomly choose starting point
            int current_point_index=MyRand::U_int(0,MAX_DATA_SIZE);

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
                current_point_index = NearestNeighbor::run(query,expansions_indexes);
            }

        }

        // sort the result
        std::vector<int> sorted_result_indexes = VecMath::sort_indexes(result_indexes,result_distances);

        // print top N results
        for(int i=0;i<result_indexes.size();i++) {
            int nni = result_indexes[i];
            std::cout<<nni<<" - "<<VecMath::dist(query,DataHandler::get_data_point_at(nni));
        }

        /*

        // run through all queries
        int number_of_queries=DataHandler::get_queries_size();
        for(int i=0;i<number_of_queries;i++) {

            // run algorithms and print some messages
            Datapoint query = DataHandler::get_test_query_at(i);

            // run range search
            std::vector<int> lsh_rs_indexes = LSH::query_range(query,radius);

            std::cout<< "\nNeighbors found in range R with LSH: \n";
            std::cout<< "----------------------------------------------\n";

            for(int lsh_rs_index:lsh_rs_indexes) {
                std::cout<<"point_index = "<<lsh_rs_index<<" ";
                std::cout<<"Distance from query: "<<VecMath::dist(query,DataHandler::get_data_point_at(lsh_rs_index))<<std::endl;
            }  

            // clock and run lsh KNN
            auto start_time_lsh = std::chrono::high_resolution_clock::now();
            std::vector<int> lsh_nn_indexes = LSH::query_KNN(query,number_of_nearest);
            auto stop_time_lsh = std::chrono::high_resolution_clock::now();
            auto duration_lsh = std::chrono::duration_cast<std::chrono::microseconds>(stop_time_lsh - start_time_lsh);

            std::cout<< "\nK Nearest Neighbors found with LSH: \n";
            std::cout<< "----------------------------------------------\n";

            for(int lsh_nn_index:lsh_nn_indexes) {
                std::cout<<"point_index = "<<lsh_nn_index<<" ";
                std::cout<<"Distance from query: "<<VecMath::dist(query,DataHandler::get_data_point_at(lsh_nn_index))<<std::endl;
            }    

            // clock and run exact KNN
            auto start_time_true = std::chrono::high_resolution_clock::now();        
            std::vector<int> exact_nn_indexes = NearestNeighbor::query_KNN(query,number_of_nearest);
            auto stop_time_true = std::chrono::high_resolution_clock::now();
            auto duration_true = std::chrono::duration_cast<std::chrono::microseconds>(stop_time_true - start_time_true);

            std::cout<< "\nK Nearest Neighbors found with exhaustive search: \n";
            std::cout<< "----------------------------------------------\n";

            for(int exact_nn_index:exact_nn_indexes) {
                std::cout<<"point_index = "<<exact_nn_index<<" ";
                std::cout<<"Distance from query: "<<VecMath::dist(query,DataHandler::get_data_point_at(exact_nn_index))<<std::endl;
            }    

            // write output file
            out_stream<<"Query: "<<i<<std::endl;
            for(int j=1;j<=number_of_nearest;j++) {
                out_stream<<"Nearest neighbor-"<<j<<": "<<lsh_nn_indexes[j-1]<<std::endl;
                out_stream<<"distanceLSH: "<<VecMath::dist(query,DataHandler::get_data_point_at(lsh_nn_indexes[j-1]))<<std::endl;
                out_stream<<"distanceTrue: "<<VecMath::dist(query,DataHandler::get_data_point_at(exact_nn_indexes[j-1]))<<std::endl;
            }

            out_stream<<"tLSH: "<<duration_lsh.count()<<std::endl;
            out_stream<<"tTrue: "<<duration_true.count()<<std::endl;
            out_stream<<"R-near neighbors: "<<std::endl;

            for(int lsh_rs_index:lsh_rs_indexes) {
                out_stream<<lsh_rs_index<<std::endl;
            }               

        }
        */

        out_stream.close();

        std::cout<<"Run again for different input, output, query files? (y)"<<std::endl;
        std::cin >> ans;

    } while(ans=="y");

    return 0;
}
