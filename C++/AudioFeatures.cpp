#include "AudioFeatures.h"

AudioFeatures::AudioFeatures(int channelCount, int sampleRate, int framesPerBuffer)
	: Audio(channelCount, sampleRate, framesPerBuffer) { };

AudioFeatures::~AudioFeatures() { };

/* From data, determine the likely pitch at each sample using the pYIN algorithm.
 * This algorithm uses the source at: https://code.soundsoftware.ac.uk/projects/pyin,
 * which was developed by Matthias Mauch and Simon Dixon at Queen Mary, University of London.
 *
 *
 *
 *
 */
void AudioFeatures::processPitches() {
	Yin *y = new Yin(this->framesPerBuffer, this->sampleRate);
	
	// Existing pYIN code requires doubles.
	// Unfortunately, doubles are not a supported type from PortAudio, so we just make a copy
	// in memory of the data vector, but with doubles instead of floats to be able to process.
	std::vector<double> dataAsDoubles(data.begin(), data.end());

	double* ptr = dataAsDoubles.data();

	for (int i=0; i<dataAsDoubles.size(); i++) {
		Yin::YinOutput output = y->process(ptr);
		std::cout << output.f0 << "\n";
		ptr = ptr + 1;
	}
	delete y;
	return;
}
