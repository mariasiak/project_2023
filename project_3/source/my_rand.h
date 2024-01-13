#ifndef MY_RAND_H
#define MY_RAND_H

#include "common.h"

/*
	A static module with tester mains used during development.
*/

class MyRand { 

public:

	static void setup();
	static float N(float mean=0.0, float sigma=1.0);
	static int U_int(int min, int max);
	static float U_float(float min, float max);

private:

	static unsigned seed; 
	static std::default_random_engine generator;

};

#endif