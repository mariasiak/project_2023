#include "../source/common.h"
#include "../source/testing.h"

#include "../source/my_rand.h"
#include "../source/data_handler.h"

int main(int argc, char* argv[]) {

    // setup random seeding
    MyRand::setup();

    // setup input variables
    std::string input_file, query_file, output_file, input_file_latent, query_file_latent;
    int k = 14, M=10, probes = 2, number_of_nearest = 1;
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
        } else if (arg == "-M" && i + 1 < argc) {
            M = std::stoi(argv[i + 1]);
            i++;
        } else if (arg == "-probes" && i + 1 < argc) {
            probes = std::stoi(argv[i + 1]);
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
        // Project 3 - added latent input and query files as input
        } else if (arg == "-dl" && i + 1 < argc) {
            input_file_latent = argv[i + 1];
            i++;
        } else if (arg == "-ql" && i + 1 < argc) {
            query_file_latent = argv[i + 1];
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
        // Project 3 - updated message for input and query files
        std::cout << "Input File Latent: " << input_file_latent << std::endl;
        std::cout << "Query File Latent: " << query_file_latent << std::endl;   
        std::cout << "k: " << k << std::endl;
        std::cout << "M: " << M << std::endl;
        std::cout << "Probes: " << probes << std::endl;
        std::cout << "Output File: " << output_file << std::endl;
        std::cout << "Number of Nearest: " << number_of_nearest << std::endl;
        std::cout << "Radius: " << radius << std::endl;

        // Project 3 - loads with original dimensions
        DataHandler::load_data_mnist(input_file,MAX_DATA_SIZE);
        DataHandler::load_queries_mnist(query_file,MAX_QUERY_SIZE);

        // Project 3 - loads data with latent dimensions
        DataHandler::load_data_latent(input_file_latent,MAX_DATA_SIZE);    
        DataHandler::load_queries_latent(query_file_latent,MAX_QUERY_SIZE);

        // setup Hypercube
        Hypercube::setup(k);

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
            Datapoint query_o = DataHandler::get_test_query_at_o(i);

            std::vector<int> hc_rs_indexes;
            if(radius>0) {
                // run range search with Hypercube
                hc_rs_indexes = Hypercube::query_range(query,radius,probes);

                std::cout<< "\nNeighbors found in range R with Hypercube: \n";
                std::cout<< "----------------------------------------------\n";

                for(int hc_rs_index:hc_rs_indexes) {
                    std::cout<<"point_index = "<<hc_rs_index<<" ";
                    // Project 3 - distances calculated on original dimensions
                    std::cout<<"Distance from query: "<<VecMath::dist_o(query_o,DataHandler::get_data_point_at_o(hc_rs_index))<<std::endl;
                }  
            }

            // clock and run Hypercube KNN
            auto start_time_hc = std::chrono::high_resolution_clock::now();
            std::vector<int> hc_nn_indexes = Hypercube::query_KNN(query,number_of_nearest,probes);
            auto stop_time_hc = std::chrono::high_resolution_clock::now();
            auto duration_hc = std::chrono::duration_cast<std::chrono::microseconds>(stop_time_hc - start_time_hc);

            std::cout<< "\nK Nearest Neighbors found with Hypercube: \n";
            std::cout<< "----------------------------------------------\n";

            for(int hc_nn_index:hc_nn_indexes) {
                std::cout<<"point_index = "<<hc_nn_index<<" ";
                // Project 3 - distances calculated on original dimensions
                std::cout<<"Distance from query: "<<VecMath::dist_o(query_o,DataHandler::get_data_point_at_o(hc_nn_index))<<std::endl;
            }    

            // clock and run exact KNN
            auto start_time_true = std::chrono::high_resolution_clock::now();        
            std::vector<int> exact_nn_indexes = NearestNeighbor::query_KNN_o(query_o,number_of_nearest);
            auto stop_time_true = std::chrono::high_resolution_clock::now();
            auto duration_true = std::chrono::duration_cast<std::chrono::microseconds>(stop_time_true - start_time_true);

            std::cout<< "\nK Nearest Neighbors found with exhaustive search: \n";
            std::cout<< "----------------------------------------------\n";

            for(int exact_nn_index:exact_nn_indexes) {
                std::cout<<"point_index = "<<exact_nn_index<<" ";
                // Project 3 - distances calculated on original dimensions
                std::cout<<"Distance from query: "<<VecMath::dist_o(query_o,DataHandler::get_data_point_at_o(exact_nn_index))<<std::endl;
            }    

            // write output file
            out_stream<<"Query: "<<i<<std::endl;
            for(int j=1;j<=number_of_nearest;j++) {
                out_stream<<"Nearest neighbor-"<<j<<": "<<hc_nn_indexes[j-1]<<std::endl;
                // Project 3 - distances calculated on original dimensions
                out_stream<<"distanceHC: "<<VecMath::dist_o(query_o,DataHandler::get_data_point_at_o(hc_nn_indexes[j-1]))<<std::endl;
                out_stream<<"distanceTrue: "<<VecMath::dist_o(query_o,DataHandler::get_data_point_at_o(exact_nn_indexes[j-1]))<<std::endl;
            }

            out_stream<<"tCube: "<<duration_hc.count()<<std::endl;
            out_stream<<"tTrue: "<<duration_true.count()<<std::endl;
            out_stream<<"R-near neighbors: "<<std::endl;

            for(int hc_rs_index:hc_rs_indexes) {
                out_stream<<hc_rs_index<<std::endl;
            }               

            tAverageApproximate+=duration_hc.count()/number_of_queries;
            tAverageTrue+=duration_true.count()/number_of_queries;


            // Project 3 - distances calculated on original dimensions
            double AF = VecMath::dist_o(query_o,DataHandler::get_data_point_at_o(hc_nn_indexes[0]))/VecMath::dist_o(query_o,DataHandler::get_data_point_at_o(exact_nn_indexes[0]));

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
