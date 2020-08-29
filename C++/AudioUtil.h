#include "Audio.h"

#include <thread>
#include <chrono>

class AudioUtil : public Audio {

	public:
		AudioUtil(int channelCount, int sampleRate, int framesPerBuffer);
		~AudioUtil();
		int record(int seconds);
		int writeWAV(const char* filename);
		int readWAV(const char* filename);

	private:
		SNDFILE *infile, *outfile;
		
};
