#include "SignalProcessing.h"

/* Returns the zero crossing rate of a frame. More information about zero-crossing rate can be
 * found here: https://en.wikipedia.org/wiki/Zero-crossing_rate
 *
 * PARAMS: in (float*) - pointer to the first element in a frame
 * 	   frameSize (int) - size of the frame
 *
 * RETURNS: zcr (double) - Rate at which signal changes from positive to negative or from 
 * 			   negative to positive.
 *
 */
double Signal::zeroCrossingRate(float* in, int frameSize) {
	
	int numOfZeroCrosses = 0;
	for (int t=1; t<frameSize-1; t++) {
		bool zerocross = Signal::floatSignChange(in[0], in[1]);
		if (zerocross) { numOfZeroCrosses++; }
		in++;
	}	

	return (numOfZeroCrosses / (frameSize - 1.0));
}



/* For float values, checks if the values of the current float and the next float have different
 * signs. This is useful for checking the zeroCrossingRate.
 *
 * PARAMS: curr (float) - current float item
 * 	   next (float) - next float item
 *
 * RETURNS: If sign has changed, return true. If not, return false. *
 *
 */
bool Signal::floatSignChange(float curr, float next) {
	return (std::signbit(curr) != std::signbit(next)); 
}
