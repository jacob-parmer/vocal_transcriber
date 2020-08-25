#include "Audio.h"

#include "portaudio.h"
#include <stdio.h>
#include <iostream>
#include <sndfile.h>
#include <cstring>
#include <vector>

class AudioUtil : public Audio {

	public:
		AudioUtil(int channelCount, int sampleRate, int framesPerBuffer)
			: Audio{channelCount, sampleRate, framesPerBuffer} {};
		~AudioUtil();
		int record(int seconds);
		int writeWAV(const char* filename);
	
	private:
		std::vector<float> data;
		SNDFILE *infile, *outfile;
		SF_INFO sfinfo;
};
