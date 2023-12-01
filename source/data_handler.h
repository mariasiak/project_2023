#ifndef DATA_HANDLING_H
#define DATA_HANDLING_H

#include "common.h"

/*
	A static module to handle the dataset.

	Provides getters, prints and handles data loading and accessing.
*/

class DataHandler { 

public:

	// init Functions
	static void load_data(int n);
	static void load_data_mnist(std::string path,int n=0);
	static void load_queries_mnist(std::string path,int n=0);
	static void load_test_queries();

	// getter Functions
	static Datapoint get_data_point_at(int i);
	static Label get_label_at(int i);
	static Datapoint get_test_query_at(int i);
	static int get_dataset_size();
	static int get_queries_size();

	// print functions ( ascii: 0 -> symbols (-+,#), 1 -> values (0 to 255) )
	static void print_data_point(Datapoint point, int ascii=0);
	static void print_data_point_at(int i, int ascii=0);
	static void print_label_at(int i);
	static void print_point_and_label_at(int i, int ascii=0);

private:

	// members
	static Dataset data_points;
	static Labels  data_labels;
	static int     dataset_size;
	static Dataset test_queries;
	static int 	   queries_size;

	// helper Functions 
	static bool i_in_range(int i,int size=dataset_size);
	static int reverse_int (int i);
};

#endif