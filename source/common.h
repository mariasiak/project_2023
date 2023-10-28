#ifndef COMMON_H
#define COMMON_H


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <sstream>
#include <dirent.h>
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>
#include <map>


#define data_file_path "dataset/mnist_train.csv"
#define LENGTH     		28
#define HEIGHT 	   		28
#define DIMENSIONS 		784 // 28 * 28 = 784

#define MIN_VAL			0
#define MAX_VAL			255

#define MAX_DATA_SIZE	5000
#define MAX_QUERY_SIZE	10

typedef std::vector<int> 	   Datapoint;
typedef std::vector<Datapoint> Dataset;
typedef int 				   Label;
typedef std::vector<Label> 	   Labels;

typedef std::vector<int> 	Bucket;
typedef std::vector<Bucket> HashTable;

#endif