
#include "common.h"
#include "testing.h"

#include "my_rand.h"
#include "data_handler.h"

void setup() {
    int n=4000;
    std::string path="dataset/train-images.idx3-ubyte";
    DataHandler::load_data_mnist(path,n);
    DataHandler::load_test_queries();
    //DataHandler::load_data(n);
    MyRand::setup();
}

int main() {

    setup();



    Testing::reverse_range_clustering();
    //Testing::HC_KNN_RS();
    //Testing::LSH_KNN_range();
    //Testing::LSH_range_search();
    //Testing::LSH_KNN();
    //Testing::Clustering_Mac_Queen();
    //Testing::Clustering_testing_k_means_pp();
    //Testing::Clustering_testing_init();
    //Testing::Clustering_testing();
    //Testing::Cluster_testing();
    //Testing::HC_testing();
    //Testing::HC_f_hash();
    //Testing::LSH_testing();
    //Testing::LSH_prototyping_g_struct();
    //Testing::LSH_prototyping_h_g();
    //Testing::LSH_prototyping_hash_function_h();
    //Testing::data_exploration();
    //Testing::nearest_neighbor();
    //Testing::vec_math();
    //Testing::data_handler_test();

    return 0;
}
