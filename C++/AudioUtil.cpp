#include "AudioUtil.h"

#include "portaudio.h"
#include <stdio.h>
#include <iostream>
#include <sndfile.h>
#include <cstring>
#include <vector>

static const int SAMPLE_SIZE = 4;

AudioUtil::~AudioUtil() { };

int AudioUtil::record(int seconds) {

	int numBytes = framesPerBuffer * inputParameters.channelCount * SAMPLE_SIZE;
	float *sampleBlock = (float *) malloc(numBytes);

	memset(sampleBlock, 0.0f, numBytes);

	Audio::startStream();

	std::cout << "Recording...\n";
	for (int i=0; i<(seconds*sampleRate)/framesPerBuffer; ++i) {
		PaError errorCode = Pa_ReadStream(stream, sampleBlock, framesPerBuffer);
		if (errorCode != paNoError) { error(errorCode); return -1;}
		
		std::cout << *sampleBlock << "\n";
		data.push_back(*sampleBlock);
	}	

	Audio::stopStream();

	return 0;
}

int AudioUtil::writeWAV(const char* filename) {

	int readcount;
	memset (&sfinfo, 0, sizeof(sfinfo));

	sfinfo.format = (SF_FORMAT_WAV | SF_FORMAT_FLOAT);
	sfinfo.channels = inputParameters.channelCount;
	sfinfo.samplerate = sampleRate;

	if (! (outfile = sf_open(filename, SFM_WRITE, &sfinfo))) {
		std::cout << "Not able to open output file " << filename << ".\n";
		puts(sf_strerror(NULL));
	}

	//sf_write_float(outfile, data, sizeof(data));

	//std::cout << sizeof(data) << "\n";
	sf_close(outfile);

	return 0;
}

