#include <cmath>

class Signal {
	public:

	double zeroCrossingRate(float* in, int frameSize);
	
	private:
	bool floatSignChange(float curr, float next);

};
