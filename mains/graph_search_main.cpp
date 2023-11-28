#include "../source/common.h"
#include "../source/testing.h"

#include "../source/my_rand.h"
#include "../source/data_handler.h"

#include "../source/gnns.h"
#include "../source/mrng.h"

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
        } // -I <int_only_for_search_on_graph> : size of candidate tank
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


        // create and open outpout file for writing
        std::ofstream out_stream(output_file);

        // write output file
        if (m==1) {
            out_stream<<"GNNS Results"<<std::endl;
        } 
        else if (m==2) {
            out_stream<<"MRNG Results"<<std::endl;
        }


        // setup LSH variables
        int LSH_L=10, LSH_M=100, LSH_k=k;

        // setup GNNS or MRNG
        if (m==1) {
            GNNS::setup(LSH_L, LSH_M, LSH_k);
        }
        else if (m==2) {
            MRNG::setup(LSH_L, LSH_M, LSH_k, I);
        }

        double tAverageApproximate=0.0;
        double tAverageTrue=0.0;
        double MAF=0.0;
        double AAF=0.0;

        // run through all queries
        int number_of_queries=DataHandler::get_queries_size();
        for(int i=0;i<number_of_queries;i++) {

            // run algorithms and print some messages
            Datapoint query = DataHandler::get_test_query_at(i);

            // clock and run GNNS or MRNG KNN
            auto start_time_graph_search = std::chrono::high_resolution_clock::now();

            std::vector<int> graph_search_nn_indexes;
            if(m==1) {
                int T=20; // T : number of greedy steps
                graph_search_nn_indexes = GNNS::query_KNN(query,N,E,R,T);
            }
            else if (m==2) {
                graph_search_nn_indexes = MRNG::query_KNN(query,N);   
            }

            auto stop_time_graph_search = std::chrono::high_resolution_clock::now();
            auto duration_graph_search = std::chrono::duration_cast<std::chrono::microseconds>(stop_time_graph_search - start_time_graph_search);

            std::cout<< "\nK Nearest Neighbors found with Graph Search: \n";
            std::cout<< "----------------------------------------------\n";

            for(int graph_search_nn_index:graph_search_nn_indexes) {
                std::cout<<"point_index = "<<graph_search_nn_index<<" ";
                std::cout<<"Distance from query: "<<VecMath::dist(query,DataHandler::get_data_point_at(graph_search_nn_index))<<std::endl;
            }    

            // clock and run exact KNN
            auto start_time_true = std::chrono::high_resolution_clock::now();        

            std::vector<int> exact_nn_indexes = NearestNeighbor::query_KNN(query,N);

            auto stop_time_true = std::chrono::high_resolution_clock::now();
            auto duration_true = std::chrono::duration_cast<std::chrono::microseconds>(stop_time_true - start_time_true);

            std::cout<< "\nK Nearest Neighbors found with exhaustive search: \n";
            std::cout<< "----------------------------------------------\n";

            for(int exact_nn_index:exact_nn_indexes) {
                std::cout<<"point_index = "<<exact_nn_index<<" ";
                std::cout<<"Distance from query: "<<VecMath::dist(query,DataHandler::get_data_point_at(exact_nn_index))<<std::endl;
            }    

            out_stream<<"Query: "<<i<<std::endl;
            for(int j=1;j<=N;j++) {
                out_stream<<"Nearest neighbor-"<<j<<": "<<graph_search_nn_indexes[j-1]<<std::endl;
                out_stream<<"distanceApproxmiate: "<<VecMath::dist(query,DataHandler::get_data_point_at(graph_search_nn_indexes[j-1]))<<std::endl;
                out_stream<<"distanceTrue: "<<VecMath::dist(query,DataHandler::get_data_point_at(exact_nn_indexes[j-1]))<<std::endl;
            }

            tAverageApproximate+=duration_graph_search.count()/number_of_queries;
            tAverageTrue+=duration_true.count()/number_of_queries;

            double AF = VecMath::dist(query,DataHandler::get_data_point_at(graph_search_nn_indexes[0]))/VecMath::dist(query,DataHandler::get_data_point_at(exact_nn_indexes[0]));

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
