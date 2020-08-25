#include "portaudio.h"
#include <stdio.h>
#include <iostream>
#include <sndfile.h>
#include <cstring>
#include <vector>

class AudioUtil;

class Audio {

	public:
		Audio(int channelCount, int sampleRate, int framesPerBuffer);
		virtual ~Audio();
		void startStream();
		void stopStream();
		PaStreamParameters getInputParameters();
		PaStreamParameters getOutputParameters();
		const int getSampleRate();
		const int getFramesPerBuffer();
		void error(PaError errorCode);
	
	protected:
		PaStreamParameters inputParameters, outputParameters;
		const int sampleRate;
		const int framesPerBuffer;
		PaStream *stream = NULL;

};
