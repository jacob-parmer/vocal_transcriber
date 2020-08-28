#include "Audio.h"

#include "portaudio.h"
#include <vector>
#include <cstring>

class AudioUtil : public Audio {

	public:
		AudioUtil(int channelCount, int sampleRate, int framesPerBuffer);
		~AudioUtil();
		int record(int seconds);
		int writeWAV(const char* filename);
		int readWAV(const char* filename);
		bool dataIsEmpty();
		void dataFlush();

	private:
		std::vector<float> data;
		SNDFILE *infile, *outfile;
		SF_INFO sfinfo;
		int initInfo();
};
