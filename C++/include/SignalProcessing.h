#ifndef SIGNAL_H_INCLUDED
#define SIGNAL_H_INCLUDED


#include <cmath>
#include <complex>
#include <vector>
#include <valarray>
const double PI = 3.141592653589793238460;

class Signal {
	public:

	static double zeroCrossingRate(float* in, int frameSize);
	static double meanSquaredError(float* in, int frameSize);	
	static void FFT(std::vector<float>& data); 

};

template <typename T> int sgn(T val);
#endif
