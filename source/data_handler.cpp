#include "data_handler.h"

// init static variables
Dataset DataHandler::data_points;
Labels  DataHandler::data_labels;
int     DataHandler::dataset_size;
Dataset DataHandler::test_queries(10);

/* Init Functions */
////////////////////////////////////////////////////////////////////////////////////////

// get first n points from mnist dataset
void DataHandler::load_data(int n) {

    std::cout<<"Loading Database."<<std::endl;

    load_test_queries();

    // set dataset_size
    dataset_size=n;

    // open the CSV file
    std::ifstream csv_file(data_file_path);

    // read and process each line in the CSV
    std::string line;
    int line_counter=0;
    while (std::getline(csv_file, line) && line_counter<n+1) { // +1 because first line is titles of fields

        // skip first line (titles of fields)
        if(line_counter==0) {
            line_counter++;
            continue;
        }

        // use a stringstream to split the line into fields on comma
        std::istringstream line_stream(line);
        std::string field;

        // read a data point from the CSV
        Datapoint data_point;
        int field_counter=0;
        while (std::getline(line_stream, field, ',')) {
            int val=atoi(field.c_str());
            // first value is data label
            if(field_counter==0) data_labels.push_back(val);
            // else it is a pixel
            else data_point.push_back(val);
            //increase field counter
            field_counter++;
        }

        // and add it to the data points
        data_points.push_back(data_point);

        // increase line counter
        line_counter++;
    }

    // close the CSV file
    csv_file.close();

    std::cout<<"Database loaded successfully."<<std::endl;
}

/* Getter Functions */
////////////////////////////////////////////////////////////////////////////////////////

// get a data point from i
Datapoint DataHandler::get_data_point_at(int i) {
    if(!i_in_range(i)) i%=dataset_size;
    return data_points[i];
}

// get a label from i
Label DataHandler::get_label_at(int i) {
    if(!i_in_range(i)) i%=dataset_size;
    return data_labels[i];
}

// get a test query of given number
Datapoint DataHandler::get_test_query_at(int i) {
    if(!i_in_range(i,10)) i%=10;
    return test_queries[i];    
}

// get dataset size
int DataHandler::get_dataset_size() {
    return dataset_size;
}


/* Print Functions */
////////////////////////////////////////////////////////////////////////////////////////

// print a data point (as an ASCII image)
void DataHandler::print_data_point(Datapoint point, int ascii) {
	for(int y=0;y<HEIGHT;y++) {
		for(int x=0;x<LENGTH;x++) {
			int val=point[x+y*LENGTH];
            
            switch(ascii){
                // ascii: 0 -> symbols (-+,#)
                case 0:
                    if(val==0) std::cout<<" ";
                    else if(val<MAX_VAL/6) std::cout<<"·";
                    else if(val<MAX_VAL/3) std::cout<<"-";
                    else if(val<2*MAX_VAL/3) std::cout<<"+";
                    else std::cout<<"#";
                    break;
                // ascii: 1 -> values (0 to 255)
                case 1:
                    printf("%3d ",val);
                    break;
            }
            
		}
        // print an extra line if values are printed for readability
        if(ascii==1) std::cout<<std::endl;
		std::cout<<std::endl;		
	}
}

// print a data point (as an ASCII image), from given i
void DataHandler::print_data_point_at(int i, int ascii) {
    if(!i_in_range(i)) i%=dataset_size;
    std::cout<<"Point at "<<i<<": "<<std::endl;
    print_data_point(data_points[i],ascii);
}

// print a data point label, from given i
void DataHandler::print_label_at(int i) {
    if(!i_in_range(i)) i%=dataset_size;
    std::cout<<"Label at "<<i<<": "<<data_labels[i]<<std::endl;
}

// print a data point (as an ASCII image) and label, from given i
void DataHandler::print_point_and_label_at(int i, int ascii) {
    if(!i_in_range(i)) i%=dataset_size;
    print_label_at(i);
    print_data_point_at(i,ascii);
}

/* Helper Functions */
////////////////////////////////////////////////////////////////////////////////////////

// check if index is in range
bool DataHandler::i_in_range(int i,int size) {
    if(i<0 || i>=size) {
        std::cout<<"Index out of range (must be in ["<<0<<","<<dataset_size<<"-1]."<<std::endl;
        return false;
    }
    return true;
}

// load test queries from test_queries file
void DataHandler::load_test_queries() {
    // set the directory path
    std::string directoryPath = "test_queries";

    // open the directory using opendir
    DIR* dir = opendir(directoryPath.c_str());
    struct dirent* query_file;

    // read each query file from the directory
    while ((query_file = readdir(dir))) {
    
        // get label (value drawn) of query file
        int label = query_file->d_name[4]-'0';

        // set query file path
        std::string query_file_path = std::string("test_queries/")+std::string(query_file->d_name);

        // open the query file
        std::ifstream query_file(query_file_path);

        // read the line of the query
        std::string line;
        std::getline(query_file, line); 

        // use a stringstream to split the query into fields on comma
        std::istringstream line_stream(line);
        std::string field;

        // read the fields from the query file
        Datapoint query_point;
        int field_counter=0;
        while (std::getline(line_stream, field, ',')) {
            int val=atoi(field.c_str());
            // first value is data label sp skip it
            if(field_counter!=0) query_point.push_back(val);
            //increase field counter
            field_counter++;
        }

        // and add the query data point to the test queries
        test_queries[label]=query_point;


        // close the CSV file
        query_file.close();

    }

    closedir(dir);
}
