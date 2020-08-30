#include "Audio.h"

#include "pyin/Yin.h"
#include "pyin/YinUtil.h"

struct Pitch {
	double f0;
	std::string name;
	double rms;
};

class AudioFeatures : public Audio {

	public:
		AudioFeatures(int channelCount, int sampleRate, int framesPerBuffer);
		~AudioFeatures();

		void processPitches();


	private:
		std::vector<Pitch> pitches;		

};
