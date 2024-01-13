#include "my_rand.h"

// init static variables
unsigned MyRand::seed; 
std::default_random_engine MyRand::generator;

// setup seed and generator
void MyRand::setup() {
	seed = std::chrono::system_clock::now().time_since_epoch().count();
	generator.seed(seed);
}

// return a number in normal distribution with given mean and sigma
float MyRand::N(float mean, float sigma) {
    std::normal_distribution<float> normal_distribution(mean,sigma);	
	return normal_distribution(generator);
}

// return a number in uniform distribution between min and max
int MyRand::U_int(int min, int max) {
    std::uniform_int_distribution<int> uniform_distribution(min,max);	
	return uniform_distribution(generator);
}

// return a number in uniform distribution between min and max
float MyRand::U_float(float min, float max) {
    std::uniform_real_distribution<float> uniform_distribution(min,max);	
	return uniform_distribution(generator);
}