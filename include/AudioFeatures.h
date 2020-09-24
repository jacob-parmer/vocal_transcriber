#include "Audio.h"
#include "SignalProcessing.h"

#include "pyin/Yin.h"
#include "pyin/YinUtil.h"
#include <math.h>
#include <map>

struct Pitch {
	double f0;
	std::string name;
	double rms;
};

static const std::map<int, std::string> NOTE_NAME = {
			{ 0, "C"},
			{ 1, "C#"},
			{ 2, "D"},
			{ 3, "D#"},
			{ 4, "E"},
			{ 5, "F"},
			{ 6, "F#"},
			{ 7, "G"},
			{ 8, "G#"},
			{ 9, "A"},
			{ 10, "A#"},
			{ 11, "B"} }; 

class AudioFeatures : public Audio {

	public:
		AudioFeatures(int channelCount, int sampleRate, int framesPerBuffer);
		~AudioFeatures();

		void processPitches();


	private:
		std::vector<Pitch> pitches;		
		std::string getNameFromF0(double f0);
};


