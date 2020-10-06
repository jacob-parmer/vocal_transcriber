#include "SignalProcessing.h"

/* Returns the zero crossing rate of a frame. The formula for this was found in the paper:
 * "Separation of voiced and unvoiced using Zero Crossing Rate and Energy of the speech signal"
 * by authors Bachu R.G., Kopparthi S., Adapa B., and Barkana B.D.
 *
 * PARAMS: in (float*) - pointer to the first element in a frame
 * 	   frameSize (int) - size of the frame
 *
 * RETURNS: zcr (double) - Rate at which signal changes from positive to negative or from 
 * 			   negative to positive.
 *
 */
double Signal::zeroCrossingRate(float* in, int frameSize) {
	
	double zcr = 0;
	for (int n=0; n<(frameSize - 1); n++) {
		int sgn1 = sgn(*(in + 1));
		int sgn2 = sgn(*in);

		double zc = std::abs(sgn1 - sgn2);
		zc = zc / (2 * frameSize);
		zcr += zc;
		
		in += 1; 
	}	

	return zcr;
}

/*
 *
 *
 *
 *
 *
 */
double Signal::meanSquaredError(float* in, int frameSize) {
	
	return 0.0;
}

/*
 *
 *
 *
 */
void Signal::FFT(std::vector<float>& data) {

	return;

}

