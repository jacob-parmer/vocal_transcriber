#include "AudioFeatures.h"

AudioFeatures::AudioFeatures(int channelCount, int sampleRate, int framesPerBuffer)
	: Audio(channelCount, sampleRate, framesPerBuffer) { };

AudioFeatures::~AudioFeatures() { };

/* From data, determine the likely pitch at each sample using the pYIN algorithm.
 * This algorithm uses the source at: https://code.soundsoftware.ac.uk/projects/pyin,
 * which was developed by Matthias Mauch and Simon Dixon at Queen Mary, University of London.
 *
 * OUTPUTS: pitches (std::vector<Pitch>) - The pitch at the given datapoint, as determined by
 * 					   the YIN algorithm's detected fundamental frequency
 * 	   
 */
/*
void AudioFeatures::processPitches() {
	Yin *y = new Yin(this->framesPerBuffer, this->sampleRate);
	
	// Existing pYIN code requires doubles.
	// Unfortunately, doubles are not a supported type from PortAudio, so we just make a copy
	// in memory of the data vector, but with doubles instead of floats to be able to process.
	std::vector<double> dataAsDoubles(data.begin(), data.end());
	double* ptr = dataAsDoubles.data();

	int dataSize = dataAsDoubles.size();

	for (int j=0; j<dataSize; j++) {
		std::cout << dataAsDoubles.at(j) << "\n";
	}

	std::cout << dataSize;

	double threshold = 0.02;
	for (int i=0; i<dataSize; i++) {
		Yin::YinOutput output = y->process(ptr);
		if (isnan(output.f0)) { continue; }
		Pitch p;
		std::cout << output.f0 << "\n";
		if (output.rms > threshold) {
			p.f0 = output.f0;
			p.name = AudioFeatures::getNameFromF0(output.f0);
			p.rms = output.rms;
			pitches.push_back(p);
		} else {
			p.f0 = 0;
			p.name = "REST";
			p.rms = 0;
			pitches.push_back(p);
		}
		ptr = ptr + 1;

	}

	std::cout << "Here: " << pitches.size() << "\n";
	for (Pitch i : pitches) {
		std::cout << i.name << "\n";
	}

	delete y;
}
*/

/* Given fundamental frequency of a signal, determine the melodic note name in 12-tone equal
 * temperament tuning.
 *
 * PARAMS: f0 (double) - fundamental frequency of the signal
 *
 * RETURNS: note (std::string) - name of the note, e.g. A4.
 *
 */
std::string AudioFeatures::getNameFromF0(double f0) {

	/* formula for frequency to midi, further information found here:
	 * https://newt.phys.unsw.edu.au/jw/notes.html */
	int midi = (int) round(12 * log2(std::abs(f0)/440) + 69);

	// note names (C, D, E, etc.) + Octave (1, 2, 3, etc.)
	std::string note = NOTE_NAME.at(midi % 12) + std::to_string(int(midi / 12) - 1);
	
	return note;	

}


