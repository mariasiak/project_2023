#include "../source/common.h"
#include "../source/testing.h"

#include "../source/my_rand.h"
#include "../source/data_handler.h"

int main(int argc, char* argv[]) {

    // setup random seeding
    MyRand::setup();

    // setup input variables
    std::string input_file, query_file, output_file;
    int k = 4, L = 5, number_of_nearest = 1;
    double radius = 10000;

    // read input variables
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-d" && i + 1 < argc) {
            input_file = argv[i + 1];
            i++;
        } else if (arg == "-q" && i + 1 < argc) {
            query_file = argv[i + 1];
            i++;
        } else if (arg == "-k" && i + 1 < argc) {
            k = std::stoi(argv[i + 1]);
            i++;
        } else if (arg == "-L" && i + 1 < argc) {
            L = std::stoi(argv[i + 1]);
            i++;
        } else if (arg == "-o" && i + 1 < argc) {
            output_file = argv[i + 1];
            i++;
        } else if (arg == "-N" && i + 1 < argc) {
            number_of_nearest = std::stoi(argv[i + 1]);
            i++;
        } else if (arg == "-R" && i + 1 < argc) {
            radius = std::stod(argv[i + 1]);
            i++;
        }
    }

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
        std::cout << "Input File: " << input_file << std::endl;
        std::cout << "Query File: " << query_file << std::endl;
        std::cout << "k: " << k << std::endl;
        std::cout << "L: " << L << std::endl;
        std::cout << "Output File: " << output_file << std::endl;
        std::cout << "Number of Nearest: " << number_of_nearest << std::endl;
        std::cout << "Radius: " << radius << std::endl;

        // load data and queries
        DataHandler::load_data_mnist(input_file,MAX_DATA_SIZE);
        DataHandler::load_queries_mnist(query_file,MAX_QUERY_SIZE);

        // setup LSH
        int M=1000;
        LSH::setup(L,M,k);

        std::ofstream out_stream(output_file);


        double tAverageApproximate=0.0;
        double tAverageTrue=0.0;
        double MAF=0.0;
        double AAF=0.0;

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

            tAverageApproximate+=duration_lsh.count()/number_of_queries;
            tAverageTrue+=duration_true.count()/number_of_queries;

            double AF = VecMath::dist(query,DataHandler::get_data_point_at(lsh_nn_indexes[0]))/VecMath::dist(query,DataHandler::get_data_point_at(exact_nn_indexes[0]));

            if(AF>MAF) {
                MAF=AF;
            }

            AAF+=AF/number_of_queries;

        }

        out_stream<<"tAverageApproximate: "<< tAverageApproximate <<std::endl;
        out_stream<<"tAverageTrue: "<< tAverageTrue <<std::endl;
        out_stream<<"MAF: "<< MAF <<std::endl;
        out_stream<<"AAF: "<< AAF <<std::endl;

        out_stream.close();

        std::cout<<"Run again for different input, output, query files? (y)"<<std::endl;
        std::cin >> ans;

    } while(ans=="y");

    return 0;
}
