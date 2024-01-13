#include "../source/common.h"
#include "../source/testing.h"

#include "../source/my_rand.h"
#include "../source/data_handler.h"

int main(int argc, char* argv[]) {

    // setup random seeding
    MyRand::setup();

    // setup input variables
    std::string input_file, configuration_file, output_file, input_file_latent;
    bool complete=false;
    std::string method;
    int number_of_clusters=5, LSH_L=3, LSH_k=4, HC_M=10, HC_k=3, HC_probes=2;

    // read input variables
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-i" && i + 1 < argc) {
            input_file = argv[i + 1];
            i++;
        } else if (arg == "-c" && i + 1 < argc) {
            configuration_file = argv[i + 1];
            i++;
        } else if (arg == "-o" && i + 1 < argc) {
            output_file = argv[i + 1];
            i++;
        } else if (arg == "-complete") {
            complete=true;
        } else if (arg == "-m" && i + 1 < argc) {
            method = argv[i + 1];
            i++;
        // Project 3 - added latent input and query files as input
        } else if (arg == "-dl" && i + 1 < argc) {
            input_file_latent = argv[i + 1];
            i++;
        }
    }

    // read values from configuration file
    
    std::ifstream configuration_stream(configuration_file);
    if (!configuration_stream) {
        std::cout << "Failed to open configuration_file: "<<configuration_file << std::endl;
        return 0;
    }

    std::string line;

    while (std::getline(configuration_stream, line)) {
        
        std::istringstream line_stream(line);
        std::string key;
        
        if (line_stream >> key) {
            
            if (key == "number_of_clusters:") {
                line_stream >> number_of_clusters;
            } else if (key == "number_of_vector_hash_tables:") {
                line_stream >> LSH_L;
            } else if (key == "number_of_vector_hash_functions:") {
                line_stream >> LSH_k;
            } else if (key == "max_number_M_hypercube:") {
                line_stream >> HC_M;
            } else if (key == "number_of_hypercube_dimensions:") {
                line_stream >> HC_k;
            } else if (key == "number_of_probes:") {
                line_stream >> HC_probes;
            }

        }
        
    }

    configuration_stream.close();

    // print input variables
    std::cout << "Input File: " << input_file << std::endl;
    // Project 3 - updated message for input and query file
    std::cout << "Input File Latent: " << input_file_latent << std::endl;
    std::cout << "Configuration File: " << configuration_file << std::endl;
    std::cout << "Output File: " << output_file << std::endl;
    std::cout << "Complete: " << complete << std::endl;
    std::cout << "Method: " << method << std::endl;
    std::cout << "number_of_clusters: " << number_of_clusters << std::endl;
    std::cout << "LSH_L: " << LSH_L << std::endl;
    std::cout << "LSH_k: " << LSH_k << std::endl;
    std::cout << "HC_M: " << HC_M << std::endl;
    std::cout << "HC_k: " << HC_k << std::endl;
    std::cout << "HC_probes: " << HC_probes << std::endl;


    // Project 3 - loads with original dimensions
    DataHandler::load_data_mnist(input_file,MAX_DATA_SIZE);

    // Project 3 - loads data with latent dimensions
    DataHandler::load_data_latent(input_file_latent,MAX_DATA_SIZE);    

    // setup clustering (runs k-means++ initialisation)
    Clustering::setup(number_of_clusters);

    // print inital centroids
    //Clustering::print_centroids();


    // time and run clustering
    auto start_time_clustering = std::chrono::high_resolution_clock::now();
    if(method=="Hypercube") {
        Clustering::run_reverse_range_hc(HC_k,HC_M,HC_probes);
    } else if (method=="Classic") {
        Clustering::run_loyds();
    } else if (method=="LSH") {
        Clustering::run_reverse_range_lsh(LSH_L,LSH_k);
    } else {
        std::cout<<"Wrong input for method. Must be Classic or LSH or Hypercube";
        return 0;
    }
    auto stop_time_clustering = std::chrono::high_resolution_clock::now();
    auto duration_clustering = std::chrono::duration_cast<std::chrono::microseconds>(stop_time_clustering - start_time_clustering);

    // print centroids after clustering
    //Clustering::print_centroids();  

    // get silhouette score
    std::vector<double> silhouette_scores = Clustering::get_silhouette_scores();

    // write output file
    std::ofstream out_stream(output_file);

    // get clusters
    std::vector<Cluster> clusters = Clustering::get_clusters();

    out_stream<<"Algorithm: "<<method<<std::endl;
    for(int i=0;i<number_of_clusters;i++){
        out_stream<<"CLUSTER-"<<i+1<<" {size: "<<clusters[i].point_indexes.size()<<", centroid: [";
        for(int j=0;j<clusters[i].centroid.size();j++) {
           out_stream<<clusters[i].centroid[j];
            if(j!=clusters[i].centroid.size()-1) {
                out_stream<<", ";
            }
        }
        out_stream<<"]}"<<std::endl;
    }
    
    out_stream<<"clustering_time: "<<duration_clustering.count()/1000000<<std::endl;
    
    out_stream<<"Silhouette: [";
    for(int i=0;i<silhouette_scores.size();i++) {
        out_stream<<silhouette_scores[i];
        if(i!=silhouette_scores.size()-1) {
            out_stream<<", ";
        }
    }
    out_stream<<"]"<<std::endl;

    if(complete==true) {
        for(int i=0;i<number_of_clusters;i++){
            out_stream<<"CLUSTER-"<<i+1<<" {[";
            for(int j=0;j<clusters[i].centroid.size();j++) {
               out_stream<<clusters[i].centroid[j];
                if(j!=clusters[i].centroid.size()-1) {
                    out_stream<<", ";
                }
            }
            out_stream<<"], ";
            for(int j=0;j<clusters[i].point_indexes.size();j++) {
               out_stream<<clusters[i].point_indexes[j];
                if(j!=clusters[i].point_indexes.size()-1) {
                    out_stream<<", ";
                }
            }
            out_stream<<"}"<<std::endl;
        }  
    }

    return 0;
}
