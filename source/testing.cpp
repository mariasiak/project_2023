#include "testing.h"

void Testing::reverse_range_clustering() {
    Clustering::setup(4);

    //Clustering::print_centroids();

    //Clustering::run_loyds();
    Clustering::run_reverse_range_lsh();
    //Clustering::run_reverse_range_hc();

    //Clustering::print_centroids();  

    //double ss = Clustering::get_silhouette_score();
}

void Testing::HC_KNN_RS() {
    Hypercube::setup(5);

    Label q_label=1;
    Datapoint query = DataHandler::get_test_query_at(q_label);    

    std::cout<< "Label of query is: "<<q_label<<std::endl;
    std::cout<< "Image of query is: "<<std::endl;
    DataHandler::print_data_point(query); 

    for(int i=0;i<=3;i++) {

        double R=1100;

        std::vector<int> hc_range_indexes = Hypercube::query_range(query,R,i);

        std::cout<< "\nNearest Neighbors found in range: \n";
        std::cout<< "----------------------------------------------\n";

        for(int hc_range_index:hc_range_indexes) {
            std::cout<<"point_index = "<<hc_range_index<<" ";
            std::cout<<"Distance from query: "<<VecMath::dist(query,DataHandler::get_data_point_at(hc_range_index))<<std::endl;
        }    
    }

    for(int i=0;i<=-1;i++) {

        int K=13;

        std::vector<int> hc_nn_indexes = Hypercube::query_KNN(query,K,i);

        std::cout<< "\nK Nearest Neighbors found with HC: \n";
        std::cout<< "----------------------------------------------\n";

        for(int hc_nn_index:hc_nn_indexes) {
            std::cout<<"point_index = "<<hc_nn_index<<" ";
            std::cout<<"Distance from query: "<<VecMath::dist(query,DataHandler::get_data_point_at(hc_nn_index))<<std::endl;
        }    
    }

    for(int i=0;i<=-1;i++) {

        int hc_nn_i = Hypercube::query_NN(query,i);

        std::cout<< "\nNearest Neighbor found with LSH: \n";
        std::cout<< "----------------------------------------------\n";
        std::cout<<"point_index = "<<hc_nn_i<<" ";
        std::cout<<"Distance from query: "<<VecMath::dist(query,DataHandler::get_data_point_at(hc_nn_i))<<std::endl;
    }


}

void Testing::LSH_KNN_range() {
    LSH::setup();

    double R=2000;
    Label q_label=1;
    Datapoint query = DataHandler::get_test_query_at(q_label);    

    std::cout<< "Label of query is: "<<q_label<<std::endl;
    std::cout<< "Image of query is: "<<std::endl;
    DataHandler::print_data_point(query); 


    for(int i=0;i<2;i++) {
        std::vector<int> lsh_rs_indexes = LSH::query_range(query,R);

        std::cout<< "\nNeighbors found in range R with LSH: \n";
        std::cout<< "----------------------------------------------\n";

        for(int lsh_rs_index:lsh_rs_indexes) {
            std::cout<<"point_index = "<<lsh_rs_index<<" ";
            std::cout<<"Distance from query: "<<VecMath::dist(query,DataHandler::get_data_point_at(lsh_rs_index))<<std::endl;
        }  
    }

    for(int i=0;i<2;i++) {

        int K=13;

        std::vector<int> lsh_nn_indexes = LSH::query_KNN(query,K);

        std::cout<< "\nK Nearest Neighbors found with LSH: \n";
        std::cout<< "----------------------------------------------\n";

        for(int lsh_nn_index:lsh_nn_indexes) {
            std::cout<<"point_index = "<<lsh_nn_index<<" ";
            std::cout<<"Distance from query: "<<VecMath::dist(query,DataHandler::get_data_point_at(lsh_nn_index))<<std::endl;
        }    
    }

   
    for(int i=0;i<2;i++) {

        int lsh_nn_i = LSH::query_NN(query);

        std::cout<< "\nNearest Neighbor found with LSH: \n";
        std::cout<< "----------------------------------------------\n";
        std::cout<<"point_index = "<<lsh_nn_i<<" ";
        std::cout<<"Distance from query: "<<VecMath::dist(query,DataHandler::get_data_point_at(lsh_nn_i))<<std::endl;
    }

    int nn_index = NearestNeighbor::run(query);

    std::cout<< "\nNearest Neighbor found with exhaustive search: \n";
    std::cout<<"point_index = "<<nn_index<<" ";
    std::cout<<"Distance from query: "<<VecMath::dist(query,DataHandler::get_data_point_at(nn_index))<<std::endl;

    nn_index = NearestNeighbor::run(query);

    std::cout<< "\nNearest Neighbor found with exhaustive search: \n";
    std::cout<<"point_index = "<<nn_index<<" ";
    std::cout<<"Distance from query: "<<VecMath::dist(query,DataHandler::get_data_point_at(nn_index))<<std::endl;

}

void Testing::LSH_range_search() {
    LSH::setup();

    double R=2000;
    Label q_label=1;
    Datapoint query = DataHandler::get_test_query_at(q_label);    

    std::cout<< "Label of query is: "<<q_label<<std::endl;
    std::cout<< "Image of query is: "<<std::endl;
    DataHandler::print_data_point(query); 

    std::vector<int> lsh_rs_indexes = LSH::query_range(query,R);

    std::cout<< "\nNeighbors found in range R with LSH: \n";
    std::cout<< "----------------------------------------------\n";

    for(int lsh_rs_index:lsh_rs_indexes) {
        DataHandler::print_point_and_label_at(lsh_rs_index);
        std::cout<<"Distance from query: "<<VecMath::dist(query,DataHandler::get_data_point_at(lsh_rs_index))<<std::endl;
    }  
}

void Testing::LSH_KNN() {
    LSH::setup();

    int K=13;
    Label q_label=1;
    Datapoint query = DataHandler::get_test_query_at(q_label);    

    std::cout<< "Label of query is: "<<q_label<<std::endl;
    std::cout<< "Image of query is: "<<std::endl;
    DataHandler::print_data_point(query); 

    std::vector<int> lsh_nn_indexes = LSH::query_KNN(query,K);

    std::cout<< "\nK Nearest Neighbors found with LSH: \n";
    std::cout<< "----------------------------------------------\n";

    for(int lsh_nn_index:lsh_nn_indexes) {
        DataHandler::print_point_and_label_at(lsh_nn_index);
    }

    lsh_nn_indexes = LSH::query_KNN(query,K);

    std::cout<< "\nK Nearest Neighbors found with LSH: \n";
    std::cout<< "----------------------------------------------\n";

    for(int lsh_nn_index:lsh_nn_indexes) {
        DataHandler::print_point_and_label_at(lsh_nn_index);
    }
}

void Testing::Clustering_Mac_Queen() {
    Clustering::setup(20);

    Clustering::print_centroids();

    Clustering::run_loyds();

    Clustering::print_centroids();

    double ss = Clustering::get_silhouette_score();
}

void Testing::Clustering_testing_k_means_pp() {
    Clustering::setup(10);

    Clustering::print_centroids();
}

void Testing::Clustering_testing_init() {
    int k=10; // number of clusters
    std::vector<Cluster> clusters;

    for(int i=0;i<k;i++){
        clusters.push_back(Cluster());
    }


    // assign centroids
    Dataset centroids; // put centroids in a dataset to use NN on
    for(int i=0;i<k;i++){
        clusters[i].centroid=DataHandler::get_test_query_at(i);
        centroids.push_back(clusters[i].centroid);
        DataHandler::print_data_point(clusters[i].centroid);
    }

    // assign points to nearest centroid's cluster
    std::map<int, int> point_index_to_cluster_map;
    for(int i=0;i<DataHandler::get_dataset_size(); i++) {
        point_index_to_cluster_map[i]=NearestNeighbor::run(DataHandler::get_data_point_at(i),centroids);
    }

    // clear previous clusters
    for(int i=0;i<k;i++) {
        clusters[i].point_indexes.clear();
    }

    // assign points to clusters
    for(int i=0;i<DataHandler::get_dataset_size(); i++) {
        int ci=point_index_to_cluster_map[i];
        clusters[ci].point_indexes.push_back(i);
    }

    // re-calculate centroids
    for(int i=0;i<k;i++){
        clusters[i].calculate_centroid();
        DataHandler::print_data_point(clusters[i].centroid);
    }      

    // print labels of points of clusters
    std::vector<std::vector<int>> labels;
    for(int i=0;i<k;i++){
        std::cout<<"Cluster "<<i<<":"<<std::endl;
        labels.push_back(std::vector<int>());        
        for(int j=0;j<clusters[i].point_indexes.size();j++) {
            //std::cout<<DataHandler::get_label_at(clusters[i].point_indexes[j])<<", ";
            labels[i].push_back(DataHandler::get_label_at(clusters[i].point_indexes[j]));
        }

        // print occurances

        // create an array to store the counts for each value (0 to 9)
        int counts[10] = {0};  // initialize all counts to zero

        // count occurrences of each element in the vector
        for (int value : labels[i]) {
            if (value >= 0 && value <= 9) {
                counts[value]++;
            }
        }

        // Print the occurrences
        for (int i = 0; i < 10; i++) {
            std::cout << "Value " << i << " occurs " << counts[i] << " times." << std::endl;
        }

        std::cout<<std::endl;
    }

}

void Testing::Clustering_testing() {
    int k=10; // number of clusters
    std::vector<Cluster> clusters;

    for(int i=0;i<k;i++){
        clusters.push_back(Cluster());
    }

    // assign points to clusters
    for(int i=0;i<DataHandler::get_dataset_size(); i++) {
        clusters[i%k].point_indexes.push_back(i);
    }

    Dataset centroids; // put centroids in a dataset to use NN on
    for(int i=0;i<k;i++){
        clusters[i].calculate_centroid();
        centroids.push_back(clusters[i].centroid);
        DataHandler::print_data_point(clusters[i].centroid);
    }

    // re-assign points to nearest clusters
    std::map<int, int> point_index_to_cluster_map;
    for(int i=0;i<DataHandler::get_dataset_size(); i++) {
        point_index_to_cluster_map[i]=NearestNeighbor::run(DataHandler::get_data_point_at(i),centroids);
    }

    // clear previous clusters
    for(int i=0;i<k;i++) {
        clusters[i].point_indexes.clear();
    }

    // assign points to clusters
    for(int i=0;i<DataHandler::get_dataset_size(); i++) {
        int ci=point_index_to_cluster_map[i];
        clusters[ci].point_indexes.push_back(i);
    }

    // re-calculate centroids
    for(int i=0;i<k;i++){
        clusters[i].calculate_centroid();
        DataHandler::print_data_point(clusters[i].centroid);
    }    
}

void Testing::Cluster_testing() {

    Bucket pis1, pis2;

    // push first 10 points for c1
    for(int i=0;i<10;i++) pis1.push_back(i);

    // push first 10 points with label 5 for p2
    int count=0;
    for(int i=0;i<DataHandler::get_dataset_size();i++) {
        //if(DataHandler::get_label_at(i)==5) {
            pis2.push_back(i);
            count++;
            if(count==10) break;
        //}
    }

    Cluster c1(pis1),c2(pis2);

    for(int i=0;i<10;i++) DataHandler::print_point_and_label_at(c1.point_indexes[i]);
    for(int i=0;i<10;i++) DataHandler::print_point_and_label_at(c2.point_indexes[i]);

    c1.calculate_centroid();
    c2.calculate_centroid();

    DataHandler::print_data_point(c1.centroid);
    DataHandler::print_data_point(c2.centroid);

}

void Testing::HC_testing() {
    auto start_time_setup = std::chrono::high_resolution_clock::now();
    Hypercube::setup(5);
    auto stop_time_setup = std::chrono::high_resolution_clock::now();
    auto duration_setup = std::chrono::duration_cast<std::chrono::microseconds>(stop_time_setup - start_time_setup);

    Label q_label=3;
    Datapoint query = DataHandler::get_test_query_at(q_label);    

    std::cout<< "Label of query is: "<<q_label<<std::endl;
    std::cout<< "Image of query is: "<<std::endl;
    DataHandler::print_data_point(query); 

    for(int i=0;i<2;i++) {
        auto start_time_hc_nn = std::chrono::high_resolution_clock::now();
        int hc_nn_index = Hypercube::query_NN(query);
        auto stop_time_hc_nn = std::chrono::high_resolution_clock::now();
        auto duration_hc_nn = std::chrono::duration_cast<std::chrono::microseconds>(stop_time_hc_nn - start_time_hc_nn);

        std::cout<< "\nNearest Neighbor found with HCs: \n";
        std::cout<< "----------------------------------------------\n";
        DataHandler::print_point_and_label_at(hc_nn_index);   
        std::cout<< "-- distance = "<<VecMath::dist(query,DataHandler::get_data_point_at(hc_nn_index))<<"\n";
        std::cout<< "-- duration = "<<duration_hc_nn.count()<<"\n";
        std::cout<< "-- setup duration = "<<duration_setup.count()<<"\n";
        std::cout<< "-- total duration = "<<duration_setup.count()+duration_hc_nn.count()<<"\n\n";
    }

    auto start_time_nn = std::chrono::high_resolution_clock::now();
    int nn_index = NearestNeighbor::run(query);
    auto stop_time_nn = std::chrono::high_resolution_clock::now();
    auto duration_nn = std::chrono::duration_cast<std::chrono::microseconds>(stop_time_nn - start_time_nn);
    
    std::cout<< "\nNearest Neighbor found with exhaustive search: \n";
    std::cout<< "----------------------------------------------\n";
    DataHandler::print_point_and_label_at(nn_index);   
    std::cout<< "-- distance = "<<VecMath::dist(query,DataHandler::get_data_point_at(nn_index))<<"\n";
    std::cout<< "-- duration = "<<duration_nn.count()<<"\n\n";
}

void Testing::HC_f_hash() {

    HashFunction_f f_test1,f_test2,f_test3;

    int test_values[]={5,123,43,23,5,123,43,23};

    std::cout<<"f_test1:"<<std::endl;
    for(int i=0;i<8;i++)
        std::cout<<f_test1.f(test_values[i])<<std::endl;

    std::cout<<"f_test2:"<<std::endl;
    for(int i=0;i<8;i++)
        std::cout<<f_test2.f(test_values[i])<<std::endl;

    std::cout<<"f_test3:"<<std::endl;
    for(int i=0;i<8;i++)
        std::cout<<f_test3.f(test_values[i])<<std::endl;
}

void Testing::LSH_testing() {

    auto start_time_setup = std::chrono::high_resolution_clock::now();
    LSH::setup();
    auto stop_time_setup = std::chrono::high_resolution_clock::now();
    auto duration_setup = std::chrono::duration_cast<std::chrono::microseconds>(stop_time_setup - start_time_setup);

    Label q_label=3;
    Datapoint query = DataHandler::get_test_query_at(q_label);    

    std::cout<< "Label of query is: "<<q_label<<std::endl;
    std::cout<< "Image of query is: "<<std::endl;
    DataHandler::print_data_point(query); 

    auto start_time_lsh_nn = std::chrono::high_resolution_clock::now();
    int lsh_nn_index = LSH::query_NN(query);
    auto stop_time_lsh_nn = std::chrono::high_resolution_clock::now();
    auto duration_lsh_nn = std::chrono::duration_cast<std::chrono::microseconds>(stop_time_lsh_nn - start_time_lsh_nn);

    std::cout<< "\nNearest Neighbor found with LSH: \n";
    std::cout<< "----------------------------------------------\n";
    DataHandler::print_point_and_label_at(lsh_nn_index);   
    std::cout<< "-- distance = "<<VecMath::dist(query,DataHandler::get_data_point_at(lsh_nn_index))<<"\n";
    std::cout<< "-- duration = "<<duration_lsh_nn.count()<<"\n";
    std::cout<< "-- setup duration = "<<duration_setup.count()<<"\n";
    std::cout<< "-- total duration = "<<duration_setup.count()+duration_lsh_nn.count()<<"\n\n";
    
    auto start_time_nn = std::chrono::high_resolution_clock::now();
    int nn_index = NearestNeighbor::run(query);
    auto stop_time_nn = std::chrono::high_resolution_clock::now();
    auto duration_nn = std::chrono::duration_cast<std::chrono::microseconds>(stop_time_nn - start_time_nn);
    
    std::cout<< "\nNearest Neighbor found with exhaustive search: \n";
    std::cout<< "----------------------------------------------\n";
    DataHandler::print_point_and_label_at(nn_index);   
    std::cout<< "-- distance = "<<VecMath::dist(query,DataHandler::get_data_point_at(nn_index))<<"\n";
    std::cout<< "-- duration = "<<duration_nn.count()<<"\n\n";

}

void Testing::LSH_prototyping_g_struct() {
    // create a g
    HashFunction_g my_g(5,10,3);
    
    // get and print g values for first 100 points
    for (int i=0;i<115;i++) {
        // get and print calculated g
        std::cout<<"calculated g is: "<<my_g.g(DataHandler::get_data_point_at(3123))<<std::endl;
    }

}

// testing out LSH h functions with struct and g
void Testing::LSH_prototyping_h_g() {

    // create a vector with hash functions h
    std::vector<HashFunction_h> hashes;

    // generate and push back k randomly chosen h functions

    // set seed for random
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // create a random number generator
    std::default_random_engine generator(seed);
    // create a uniform distribution in the range [2, 6] for w 
    std::uniform_real_distribution<double> uniform_distribution(2,6);
    // choose a k (= 4 to 10 )
    int k=10;
    // add h functions to vector hashes
    for (int i=0;i<k;i++) {
        hashes.push_back(uniform_distribution(generator));
    }

    // load a point
    int n=10000;

    DataHandler::load_data(n);

    Label q_label=3;
    Datapoint p = DataHandler::get_test_query_at(q_label);

    // save values of hashes in a vector
    std::vector<int> calculated_hs;
    for (int i=0;i<k;i++) {
        calculated_hs.push_back(hashes[i].h(p));
    } 

    // get and print values from all hashes
    std::cout<<"1st try:"<<std::endl;
    for (int i=0;i<k;i++) {
        std::cout<<hashes[i].h(p)<<std::endl;
    }

    // and one more time to check same p gets same values
    std::cout<<std::endl<<"2nd try:"<<std::endl;
    for (int i=0;i<k;i++) {
        std::cout<<hashes[i].h(p)<<std::endl;
    }

    // create a vector with ri s
    std::vector<int> ris;
    // create a uniform distribution in the range [0, N] for ri s
    int N=12345;
    std::uniform_real_distribution<double> u_d_N(0,N);
    // fill ris
    for (int i=0;i<k;i++) {
        ris.push_back(u_d_N(generator));
    }
    // choose M
    int M=10;

    // g(p) = sum(i from 1 to k) ri*hi(p) % M
    int g=VecMath::ip(ris,calculated_hs)%M;

    // print calculated g
    std::cout<<std::endl<<"calculated g is:"<<std::endl;
    std::cout<<g<<std::endl;

}

// testing out LSH h function
void Testing::LSH_prototyping_hash_function_h() {


    // create a random number generator
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    // create a normal distribution with mean 0 and standard deviation 1
    std::normal_distribution<double> n_distribution(0.0, 1.0);

    // generate a v
    std::vector<double> normaly_distributed_v;
    for(int i=0;i<DIMENSIONS;i++) normaly_distributed_v.push_back(n_distribution(generator));

    // choose a w
    int w = 3;

    // generate a t
    // create a uniform distribution in the range [0, w]
    /*TODO handle negative values*/
    std::uniform_real_distribution<double> u_distribution(0.0, w);
    double t = u_distribution(generator);

    // load a p
    int n=10000;

    DataHandler::load_data(n);

    Label q_label=3;
    Datapoint p = DataHandler::get_test_query_at(q_label);    


    // print what we got
    std::cout<<"v is: \n";
    for(int i=0;i<10;i++) std::cout<<normaly_distributed_v[i]<<" ";
    std::cout<<" ..."<<std::endl;

    std::cout<<"w is: "<<w<<"\n";

    std::cout<<"t is: "<<t<<"\n";        

    std::cout<<"p is: \n";        
    DataHandler::print_data_point(p);

    // calculate h

    int h=std::floor((VecMath::ip(p,normaly_distributed_v)+t)/w);
    std::cout<<"h is "<<h<<std::endl;

}

// does not work without labels!
// calculate frequency of appearing numbers
void Testing::data_exploration() {
    

    int size=DataHandler::get_dataset_size();

    std::cout<<"Dataset size: "<<size<<std::endl;

    // init counters vector to all 0s
    std::vector<int> counters;
    for(int i=0;i<10;i++) {
        counters.push_back(0);
    }

    // count how many times each number occurs
    for(int i=0;i<size;i++) {
        counters[DataHandler::get_label_at(i)]++;
    }
   
    // find max occurances index
    int max_i=0;
    for(int i=1;i<10;i++) {
        if(counters[i]>counters[max_i]){
            max_i=i;
        }
    }    
    int max_val=counters[max_i];

    // print values as bars
    int bar_size=25;
    for(int i=0; i<10; i++){
        std::cout<<i<<" | "<<counters[i]<<" | ";
        for(int b=0; b<counters[i]*bar_size/max_val; b++) {
            std::cout<<"#";
        }
        std::cout<<std::endl;
    }
}  

// nearest neighbor test
void Testing::nearest_neighbor() {
    

    Label q_label=3;
    Datapoint query = DataHandler::get_test_query_at(q_label); 

    std::cout<< "Label of query is: "<<q_label<<std::endl;
    std::cout<< "Image of query is: "<<std::endl;
    DataHandler::print_data_point(query);  

    int nn_index = NearestNeighbor::run(query);

    std::cout<< "\nNearest Neighbor found: \n";
    std::cout<< "----------------------------------------------\n";
    DataHandler::print_point_and_label_at(nn_index);   

}

// vec math testing
void Testing::vec_math() {

    int i1=3, i2=7;
    Datapoint p1=DataHandler::get_test_query_at(i1), p2=DataHandler::get_test_query_at(i2);

    std::cout<< "\nPoint 1:\n";
    std::cout<< "----------------------------------------------\n";    
    DataHandler::print_data_point(p1);    

    std::cout<< "\nPoint 2:\n";
    std::cout<< "----------------------------------------------\n";    
    DataHandler::print_data_point(p2);   

    std::cout<< "\nPoint 1 + Point 2:\n";
    std::cout<< "----------------------------------------------\n";  
    Datapoint sum=VecMath::add(p1,p2);
    DataHandler::print_data_point(sum);

    std::cout<< "\nPoint 1 - Point 2:\n";
    std::cout<< "----------------------------------------------\n";  
    Datapoint dif1=VecMath::sub(p1,p2);
    DataHandler::print_data_point(dif1);

    std::cout<< "\nPoint 2 - Point 1:\n";
    std::cout<< "----------------------------------------------\n";  
    Datapoint dif2=VecMath::sub(p2,p1);
    DataHandler::print_data_point(dif2);

    std::cout<< "\nPoint 1 * Point 2:\n";
    std::cout<< "----------------------------------------------\n";  
    Datapoint prod=VecMath::mul(p1,p2);
    DataHandler::print_data_point(prod);

    std::cout<< "\nPoint 1 ip Point 2:\n";
    std::cout<< "----------------------------------------------\n";  
    int inner_prod=VecMath::ip(p1,p2);
    std::cout<<"inner product = "<<inner_prod<<std::endl;

    std::cout<< "\nPoint 1 dist Point 2:\n";
    std::cout<< "----------------------------------------------\n";  
    double distance=VecMath::dist(p1,p2);
    std::cout<<"distance = "<<distance<<std::endl;

}

// trying loading, gets, prints of DataHandler module
void Testing::data_handler_test() {
	

    int index=5;
    int ascii=0;


    std::cout<< "\nTesting retrived items with Getter Functions: \n";
    std::cout<< "----------------------------------------------\n";
 	
 	Datapoint point = DataHandler::get_data_point_at(index);
    Label label = DataHandler::get_label_at(index);
    
    std::cout<< "Retrieved Label at "<<index<<" is: "<<label<<std::endl;
    std::cout<< "Retrieved Image at "<<index<<": "<<std::endl;
    DataHandler::print_data_point(point,ascii);

    std::cout<< "\nTesting print_data_point_at: \n";
    std::cout<< "----------------------------------------------\n";
    DataHandler::print_data_point_at(index,ascii);

    std::cout<< "\nTesting print_label_at: \n";
    std::cout<< "----------------------------------------------\n";
    DataHandler::print_label_at(index);

    std::cout<< "\nTesting print_point_and_label_at\n";
    std::cout<< "----------------------------------------------\n";
    DataHandler::print_point_and_label_at(index,ascii);

    index=2;
    std::cout<< "\nTesting print_point_and_label_at\n";
    std::cout<< "----------------------------------------------\n";
    DataHandler::print_point_and_label_at(index,ascii);

    index=23;
    std::cout<< "\nTesting print_point_and_label_at\n";
    std::cout<< "----------------------------------------------\n";
    DataHandler::print_point_and_label_at(index,ascii);



    // test a search of all pictures for a given label number

    int search_label=5;
    std::cout<< "\nSearching all "<<search_label<<"s in dataset:\n";
    std::cout<< "----------------------------------------------\n";

    for(int i=0;i<DataHandler::get_dataset_size();i++) {
    	if(DataHandler::get_label_at(i)==search_label)
    		DataHandler::print_point_and_label_at(i,ascii);
    }

}
