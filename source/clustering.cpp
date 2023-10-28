#include "clustering.h"

int Clustering::k;
std::vector<Cluster> Clustering::clusters;

void Clustering::setup(int kk){
    // set number of clusters
    k=kk; 
    
    // push back empty clusters in clusters vector
    for(int i=0;i<k;i++){
        clusters.push_back(Cluster());
    }

    // initialise clusters with k-means++
    k_means_pp_init();   
}

// print centroid pictures
void Clustering::print_centroids(){
    for(int i=0;i<k;i++){
        DataHandler::print_data_point(clusters[i].centroid);
    } 
}

// perform a number of loyd's steps
void Clustering::run_loyds(){
    int max_iterations=30;

    double mean_distance=-1;
    for(int iter=0;iter<max_iterations;iter++) {
        double new_mean_distance=loyd_step();
        std::cout<<"mean_distance is: "<<new_mean_distance<<std::endl;

        // centroids start dancing around fixed points
        if(mean_distance!=-1 && mean_distance<new_mean_distance) break;

        mean_distance=new_mean_distance;
    }  
}

void Clustering::run_reverse_range_lsh(int LSH_L, int LSH_k){
    
    // get daset size and setup lsh
    int data_size=DataHandler::get_dataset_size();
    int M=data_size/10;
    LSH::setup(LSH_L,M,LSH_k);

    // put centroids in a dataset to find nearest pair   
    Dataset centroids; 
    for(int i=0;i<k;i++){
        centroids.push_back(clusters[i].centroid);
    }

    // do while clusters are moving around
    int max_iterations=10;

    double mean_distance=-1;

    for(int iter=0;iter<max_iterations;iter++){

        // clear previous clusters
        for(int i=0;i<k;i++) {
            clusters[i].point_indexes.clear();
        }

        // vector to store final assignment to clusters
        std::vector<int> final_clusters;
        for(int i=0;i<data_size;i++) {
            final_clusters.push_back(-1);
        }

        // find minimum distance between centroids to use as starting range
        double min_distance;
        bool first_run=true;

        for(int i=0;i<k;i++) {
            for(int j=i+1;j<k;j++) {
                double distance=VecMath::dist(centroids[i],centroids[j]);
                if(first_run==true || distance<min_distance) {
                    min_distance=distance;
                    first_run=false;
                }
            }
        }

        // count how many points where assigned per range to stop increasing range
        int points_assigned_count=0;
        
        // starting range will be minimum distance between two centroids, halved
        double range=min_distance/2;
        int range_increase_rate=2;
        
        while(points_assigned_count<data_size) {
            // create a vector with possible clusters for points
            std::vector<std::vector<int>> data_point_clusters;
            for(int i=0;i<data_size;i++) {
                data_point_clusters.push_back(std::vector<int>());
            }

            // run range search for each centroid and mark results in data_point_clusters
            for(int i=0;i<k;i++) {
                std::vector<int> points_indexes_near_centroid_i = LSH::query_range(centroids[i],range);    
                for(int point_index:points_indexes_near_centroid_i) {
                    data_point_clusters[point_index].push_back(i);
                }
            }    

            // from possible clusters, assign point to nearest
            for(int i=0;i<data_size;i++) {
                Datapoint current_point=DataHandler::get_data_point_at(i);

                int possible_clusters_count=data_point_clusters[i].size();

                // maybe next range increase will get the point
                if(possible_clusters_count==0) continue;

                // point already assigned to cluster
                if(final_clusters[i]!=-1) continue;

                // find closest cluster from clusters in range
                bool first_run = true;
                double min_distance;
                int min_index;

                for(int j=0;j<possible_clusters_count;j++) {
                    int cluster_index=data_point_clusters[i][j];
                    double distance=VecMath::dist(centroids[cluster_index],current_point);
                    if(first_run==true || distance<min_distance) {
                        min_distance=distance;
                        min_index=cluster_index;
                        first_run=false;
                    }
                }

                // assign cluster index to final clusters
                final_clusters[i]=min_index;
                points_assigned_count++;
            }

            range*=range_increase_rate;

            //std::cout<<"points_assigned_count: "<<points_assigned_count<<std::endl;
        }

        // put point indexes to assigned clusters
        for(int i=0;i<data_size;i++) {
            int point_cluster=final_clusters[i];
            clusters[point_cluster].point_indexes.push_back(i);
        }

        // save old centroids to check convergence
        Dataset old_centroids; 

        // update centroids with mac queen method (re-calculate centroids as mean of assigned points)
        for(int i=0;i<k;i++){
            old_centroids.push_back(clusters[i].centroid);
            clusters[i].calculate_centroid();
        }   

        // update centroids dataset to compute mean distances
        centroids.clear();  
        for(int i=0;i<k;i++){
            centroids.push_back(clusters[i].centroid);
        }

        // calculate mean distance of new and old centroids
        double new_mean_distance = VecMath::mean_distance_datasets(old_centroids,centroids);

        // centroids start dancing around fixed points
        if(mean_distance!=-1 && mean_distance<new_mean_distance) break;

        mean_distance=new_mean_distance;

        std::cout<<"mean_distance = "<<mean_distance<<std::endl;
    }
}

void Clustering::run_reverse_range_hc(int HC_k,int HC_M,int HC_probes){
    // get daset size and setup HC
    int data_size=DataHandler::get_dataset_size();
    Hypercube::setup(HC_k);

    // put centroids in a dataset to find nearest pair   
    Dataset centroids; 
    for(int i=0;i<k;i++){
        centroids.push_back(clusters[i].centroid);
    }

    // do while clusters are moving around
    int max_iterations=10;

    double mean_distance=-1;

    for(int iter=0;iter<max_iterations;iter++){

        // clear previous clusters
        for(int i=0;i<k;i++) {
            clusters[i].point_indexes.clear();
        }

        // vector to store final assignment to clusters
        std::vector<int> final_clusters;
        for(int i=0;i<data_size;i++) {
            final_clusters.push_back(-1);
        }

        // find minimum distance between centroids to use as starting range
        double min_distance;
        bool first_run=true;

        for(int i=0;i<k;i++) {
            for(int j=i+1;j<k;j++) {
                double distance=VecMath::dist(centroids[i],centroids[j]);
                if(first_run==true || distance<min_distance) {
                    min_distance=distance;
                    first_run=false;
                }
            }
        }

        // count how many points where assigned per range to stop increasing range
        int points_assigned_count=0;
        
        // starting range will be minimum distance between two centroids, halved
        double range=min_distance/2;
        int range_increase_rate=2;
        
        while(points_assigned_count<data_size) {

            //std::cout<<points_assigned_count<<std::endl;
            // create a vector with possible clusters for points
            std::vector<std::vector<int>> data_point_clusters;
            for(int i=0;i<data_size;i++) {
                data_point_clusters.push_back(std::vector<int>());
            }

            // run range search for each centroid and mark results in data_point_clusters
            for(int i=0;i<k;i++) {
                std::vector<int> points_indexes_near_centroid_i = Hypercube::query_range(centroids[i],range,HC_k);    
                for(int point_index:points_indexes_near_centroid_i) {
                    data_point_clusters[point_index].push_back(i);
                }
            }    

            // from possible clusters, assign point to nearest
            for(int i=0;i<data_size;i++) {
                Datapoint current_point=DataHandler::get_data_point_at(i);

                int possible_clusters_count=data_point_clusters[i].size();

                // maybe next range increase will get the point
                if(possible_clusters_count==0) continue;

                // point already assigned to cluster
                if(final_clusters[i]!=-1) continue;

                // find closest cluster from clusters in range
                bool first_run = true;
                double min_distance;
                int min_index;

                for(int j=0;j<possible_clusters_count;j++) {
                    int cluster_index=data_point_clusters[i][j];
                    double distance=VecMath::dist(centroids[cluster_index],current_point);
                    if(first_run==true || distance<min_distance) {
                        min_distance=distance;
                        min_index=cluster_index;
                        first_run=false;
                    }
                }

                // assign cluster index to final clusters
                final_clusters[i]=min_index;
                points_assigned_count++;
            }

            range*=range_increase_rate;

            //std::cout<<"points_assigned_count: "<<points_assigned_count<<std::endl;
        }

        // put point indexes to assigned clusters
        for(int i=0;i<data_size;i++) {
            int point_cluster=final_clusters[i];
            clusters[point_cluster].point_indexes.push_back(i);
        }

        // save old centroids to check convergence
        Dataset old_centroids; 

        // update centroids with mac queen method (re-calculate centroids as mean of assigned points)
        for(int i=0;i<k;i++){
            old_centroids.push_back(clusters[i].centroid);
            clusters[i].calculate_centroid();
        }   

        // update centroids dataset to compute mean distances
        centroids.clear();  
        for(int i=0;i<k;i++){
            centroids.push_back(clusters[i].centroid);
        }

        // calculate mean distance of new and old centroids
        double new_mean_distance = VecMath::mean_distance_datasets(old_centroids,centroids);

        // centroids start dancing around fixed points
        if(mean_distance!=-1 && mean_distance<new_mean_distance) break;

        mean_distance=new_mean_distance;

        std::cout<<"mean_distance = "<<mean_distance<<std::endl;
    }
}

/*
	k-means++ intitialisation:
		- select 1st centroid in a uniform random manner from dataset
		- select next centroid with from dataset probability:
			-> P = dist(nearest_centroid)^2 / sum("squared distances of all points from their nearest centroid")
*/

void Clustering::k_means_pp_init() {
    std::cout<<"Calculating centroids using K-Means++:"<<std::endl;

    // initialise dataset size and saved indexes of centroids
    int dataset_size = DataHandler::get_dataset_size();
    std::vector<int> centroid_indexes;

    // choose first centroid at random from database and set it for 1st cluster
    centroid_indexes.push_back(MyRand::U_int(0,dataset_size-1));
    clusters[0].centroid = DataHandler::get_data_point_at(centroid_indexes[0]);

    // print an update message
    std::cout<<"Centroid "<<0<<" at index: "<<centroid_indexes[0]<<std::endl;

    // choose next centroids using the K-Means++ method
    for(int i=1;i<k;i++) {

        // save squared distances from nearest centroid
        std::vector<double> squared_distances;
        double squared_distances_sum=0;

        for(int j=0;j<dataset_size;j++) {

            // check if point is already a centroid
            bool is_centroid=false;
            for(int centroid_index:centroid_indexes) {
                if(centroid_index==j) {
                    is_centroid=true;
                }
            }

            if(is_centroid) {
                squared_distances.push_back(-1);
                continue;
            }

            // find nearest centroid index
            Datapoint jth_point=DataHandler::get_data_point_at(j);
            Datapoint nearest_centroid = DataHandler::get_data_point_at(NearestNeighbor::run(jth_point,centroid_indexes));

            // calculatea distance from it, divide by 1000 to reduce size of numbers (probability stays the same)
            int distance_from_nearest_centroid = VecMath::dist(jth_point,nearest_centroid)/1000;

            // push back its square
            squared_distances.push_back(distance_from_nearest_centroid*distance_from_nearest_centroid);

            // add to sum
            squared_distances_sum+=distance_from_nearest_centroid;
        }

        // randomly select a point to become the next centroid
        double random_roll = MyRand::U_float(0,squared_distances_sum);
        double temp_sum=0;
        int next_centroid_index;
        for(int j=0;j<dataset_size;j++) {
            // skip already assigned centroids
            if(squared_distances[j]==-1) continue;

            temp_sum+=squared_distances[j];
            if(temp_sum>random_roll) {
                next_centroid_index=j;
                break;
            }
        }

        // push back new centroid
        centroid_indexes.push_back(next_centroid_index);

        // set centroid for next cluster
        clusters[i].centroid = DataHandler::get_data_point_at(centroid_indexes[i]);

        // print an update message
        std::cout<<"Centroid "<<i<<" at index: "<<centroid_indexes[i]<<std::endl;
    }

}

// assign points to exact nearest centroid
double Clustering::loyd_step() {

    // put centroids in a dataset to use NN on    
    Dataset centroids; 
    for(int i=0;i<k;i++){
        centroids.push_back(clusters[i].centroid);
    }

    // clear previous clusters
    for(int i=0;i<k;i++) {
        clusters[i].point_indexes.clear();
    }

    // assign points to nearest centroid's cluster
    for(int i=0;i<DataHandler::get_dataset_size(); i++) {
        // find index of closest cluster
        int closest_cluster_index=NearestNeighbor::run(DataHandler::get_data_point_at(i),centroids);
        // push index of point to that cluster
        clusters[closest_cluster_index].point_indexes.push_back(i);
    }

    // save old centroids to check convergence
    Dataset old_centroids; 

    // update centroids with mac queen method (re-calculate centroids as mean of assigned points)
 	for(int i=0;i<k;i++){
        old_centroids.push_back(clusters[i].centroid);
        clusters[i].calculate_centroid();
    } 	

    // update centroids dataset to compute mean distances
    centroids.clear();  
    for(int i=0;i<k;i++){
        centroids.push_back(clusters[i].centroid);
    }

    // calculate mean distance of new and old centroids
    double mean_distance = VecMath::mean_distance_datasets(old_centroids,centroids);

    return mean_distance;
}

/*
	Silhouette Score:
		- ~-1: bad, ~0: ok, ~1 good
		- for each point in a cluster calculate ai = mean distance from other points in cluster (O(cluster_size^2))
		- for each point in a cluster calculate bi = mean distance from other points in nearest cluster (O(cluster_size^2))
		- s(i) = (bi-ai)/max(ai,bi)
		- average out si for cluster
		- average out si_cluster for all clusters		
*/

std::vector<double> Clustering::get_silhouette_scores() {

    std::cout<<"Calculating Silhouette Score:"<<std::endl;

    // put centroids in a dataset to use NN on
    Dataset centroids; 
    for(int i=0;i<clusters.size();i++){
        centroids.push_back(clusters[i].centroid);
    }

    // initialise silouhette scores and number of clusters
    std::vector<double> silouhette_scores;
    double total_silouhette_score=0.0;
    int num_of_clusters=clusters.size();

    // run silouhette calculations for all clusters
    for(int k=0;k<num_of_clusters;k++) {

        // initialise silhouette score for curent cluster
        double cluster_silouhette_score=0.0;
        int cluster_size = clusters[k].point_indexes.size();

        // for all data points of cluster
        for(int i=0;i<cluster_size;i++) {
            // get datapoint and initialise ai
            double ai=0;
            Datapoint ith_point=DataHandler::get_data_point_at(clusters[k].point_indexes[i]);

            // caculate ai by finding distances of ith point from all other points within cluster
            for(int j=0;j<cluster_size;j++) {
                if(i!=j) {
                    // for each combination of points in cluster calculate distance
                    Datapoint jth_point=DataHandler::get_data_point_at(clusters[k].point_indexes[j]);
                    ai+=VecMath::dist(ith_point,jth_point);
                }
            }
            
            // average it by dividing with cluster_size
            ai/=cluster_size;

            // find nearest cluster that is not the one we are currently in
            int nearest_cluster_index = NearestNeighbor::run(ith_point,centroids,k);

            // caculate bi by finding distances of ith point from all points of nearest cluster
            double bi=0;
            int nearest_cluster_size=clusters[nearest_cluster_index].point_indexes.size();
            for(int j=0;j<nearest_cluster_size;j++) {
                // for each combination of points calculate distance
                Datapoint jth_point=DataHandler::get_data_point_at(clusters[nearest_cluster_index].point_indexes[j]);
                bi+=VecMath::dist(ith_point,jth_point);
            }

            // average it by dividing with nearest_cluster_size
            bi/=nearest_cluster_size;

            // calculate si for ith point and add to total cluster score
            double si = (bi - ai) / std::max(ai, bi);
            cluster_silouhette_score+=si/cluster_size;
        }

        std::cout<<"Silhouette Score of cluster "<<k<<" is:"<<cluster_silouhette_score<<std::endl;
        total_silouhette_score+=cluster_silouhette_score/num_of_clusters;
        silouhette_scores.push_back(cluster_silouhette_score);
    }

    std::cout<<"Total Silouhette score is: "<<total_silouhette_score<<std::endl;
    silouhette_scores.push_back(total_silouhette_score);

    return silouhette_scores;
}

std::vector<Cluster> Clustering::get_clusters() {
    return clusters;
}