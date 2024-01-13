#include "data_handler.h"

// init static variables
Dataset DataHandler::data_points;
Dataset DataHandler::data_points_original;
Labels  DataHandler::data_labels;
int     DataHandler::dataset_size;
Dataset DataHandler::test_queries;
Dataset DataHandler::test_queries_original;
int     DataHandler::queries_size;

/* Init Functions */
////////////////////////////////////////////////////////////////////////////////////////

// load data from ubyte mnist dataset
void DataHandler::load_data_mnist(std::string path,int n)
{

    std::ifstream file (path);

    std::cout<<"Loading data from: "<<path<<std::endl;

    if (file.is_open())
    {
        int magic_number=0;
        int number_of_images=0;
        int n_rows=0;
        int n_cols=0;

        file.read((char*)&magic_number,sizeof(magic_number)); 
        magic_number= reverse_int(magic_number);
        file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images= reverse_int(number_of_images);
        file.read((char*)&n_rows,sizeof(n_rows));
        n_rows= reverse_int(n_rows);
        file.read((char*)&n_cols,sizeof(n_cols));
        n_cols= reverse_int(n_cols);


        std::cout<<"magic_number = "<<magic_number<<std::endl;
        std::cout<<"number_of_images = "<<number_of_images<<std::endl;
        std::cout<<"n_rows = "<<n_rows<<std::endl;
        std::cout<<"n_cols = "<<n_cols<<std::endl;

        if(n!=0 && n<number_of_images) {
            number_of_images=n;
        }

        dataset_size=number_of_images;

        for(int i=0;i<number_of_images;++i)
        {

            Datapoint data_point;

            // reading a data point here
            for(int r=0;r<n_rows;++r)
            {
                for(int c=0;c<n_cols;++c)
                {
                    unsigned char temp=0;
                    file.read((char*)&temp,sizeof(temp));
                    data_point.push_back(static_cast<int>(temp));
                }
            }

            data_labels.push_back(-1);
            data_points_original.push_back(data_point);
        }

        file.close();

        std::cout<<"Data loaded successfully."<<std::endl;
    } else {
        std::cout<<"Could not find dataset path: "<<path<<std::endl;
    }

}

// load queries from ubyte mnist dataset
void DataHandler::load_queries_mnist(std::string path,int n)
{

    std::ifstream file (path);

    std::cout<<"Loading queries from: "<<path<<std::endl;

    if (file.is_open())
    {
        int magic_number=0;
        int number_of_images=0;
        int n_rows=0;
        int n_cols=0;

        file.read((char*)&magic_number,sizeof(magic_number)); 
        magic_number= reverse_int(magic_number);
        file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images= reverse_int(number_of_images);
        file.read((char*)&n_rows,sizeof(n_rows));
        n_rows= reverse_int(n_rows);
        file.read((char*)&n_cols,sizeof(n_cols));
        n_cols= reverse_int(n_cols);


        std::cout<<"magic_number = "<<magic_number<<std::endl;
        std::cout<<"number_of_images = "<<number_of_images<<std::endl;
        std::cout<<"n_rows = "<<n_rows<<std::endl;
        std::cout<<"n_cols = "<<n_cols<<std::endl;

        if(n!=0 && n<number_of_images) {
            number_of_images=n;
        }

        queries_size=number_of_images;

        for(int i=0;i<number_of_images;++i)
        {

            Datapoint data_point;

            // reading a data point here
            for(int r=0;r<n_rows;++r)
            {
                for(int c=0;c<n_cols;++c)
                {
                    unsigned char temp=0;
                    file.read((char*)&temp,sizeof(temp));
                    data_point.push_back(static_cast<int>(temp));
                }
            }

            test_queries_original.push_back(data_point);
        }

        file.close();

        std::cout<<"Queries loaded successfully."<<std::endl;
    } else {
        std::cout<<"Could not find queries path: "<<path<<std::endl;
    }

}

// get first n points from mnist csv dataset
void DataHandler::load_data_latent(std::string path, int n) {

    std::cout<<"Loading Database."<<std::endl;

    // set dataset_size
    dataset_size=n;

    // open the CSV file
    std::ifstream csv_file(path);

    // read and process each line in the CSV
    std::string line;
    int line_counter=0;
    while (std::getline(csv_file, line)) { 

        // use a stringstream to split the line into fields on comma
        std::istringstream line_stream(line);
        std::string field;

        // read a data point from the CSV
        Datapoint data_point;
        int field_counter=0;
        while (std::getline(line_stream, field, ',')) {
            int val=atoi(field.c_str());

            /*
            // first value is data label
            if(field_counter==0) data_labels.push_back(val);
            // else it is a pixel
            else 
            */

            data_point.push_back(val);
            
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

// get first n points from mnist csv dataset
void DataHandler::load_queries_latent(std::string path, int n) {

    std::cout<<"Loading Queries."<<std::endl;

    // set queries_size
    queries_size=n;

    // open the CSV file
    std::ifstream csv_file(path);

    // read and process each line in the CSV
    std::string line;
    int line_counter=0;
    while (std::getline(csv_file, line) && line_counter<n+1) { // +1 because first line is titles of fields

        // use a stringstream to split the line into fields on comma
        std::istringstream line_stream(line);
        std::string field;

        // read a data point from the CSV
        Datapoint data_point;
        int field_counter=0;
        while (std::getline(line_stream, field, ',')) {
            int val=atoi(field.c_str());

            /*
            // first value is data label
            if(field_counter==0) data_labels.push_back(val);
            // else it is a pixel
            else 
            */

            data_point.push_back(val);
            //increase field counter
            field_counter++;
        }

        // and add it to the data points
        test_queries.push_back(data_point);

        // increase line counter
        line_counter++;
    }

    // close the CSV file
    csv_file.close();

    std::cout<<"Queries loaded successfully."<<std::endl;
}

// get first n points from mnist csv dataset
void DataHandler::load_data(int n) {

    std::cout<<"Loading Database."<<std::endl;

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
    if(!i_in_range(i,queries_size)) i%=10;
    return test_queries[i];    
}

// get dataset size
int DataHandler::get_dataset_size() {
    return dataset_size;
}

// get queries size
int DataHandler::get_queries_size() {
    return queries_size;
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
        std::cout<<"Index i= "<<i<<"out of range (must be in ["<<0<<","<<dataset_size<<"-1]."<<std::endl;
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

// reverse an integer (used for reading the ubyte dataset file)
int DataHandler::reverse_int (int i) 
{
    unsigned char c1, c2, c3, c4;

    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;

    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}

// Project 3 - handle original data
Datapoint DataHandler::get_data_point_at_o(int i){
    if(!i_in_range(i)) i%=dataset_size;
    return data_points_original[i];
}

Datapoint DataHandler::get_test_query_at_o(int i){
    if(!i_in_range(i,queries_size)) i%=10;
    return test_queries_original[i];    
}

void DataHandler::print_data_point_o(Datapoint point, int ascii){
    for(int y=0;y<HEIGHT_o;y++) {
        for(int x=0;x<LENGTH_o;x++) {
            int val=point[x+y*LENGTH_o];
            
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

void DataHandler::print_data_point_at_o(int i, int ascii){
    if(!i_in_range(i)) i%=dataset_size;
    std::cout<<"Point at "<<i<<": "<<std::endl;
    print_data_point_o(data_points_original[i],ascii);
}