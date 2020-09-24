#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#include "portaudio.h"

#include <iostream>
#include <sndfile.h>
#include <vector>
#include <cstring>

class AudioUtil;
class AudioFeatures;

class Audio {

	public:
		Audio(int channelCount, int sampleRate, int framesPerBuffer);
		virtual ~Audio();
		
		void startStream();
		void readStream(bool &stop);
		void stopStream();

		bool dataIsEmpty();
		void dataFlush();
		
		std::vector<float> getAudioData();
		void setAudioData(std::vector<float>);

		void error(PaError errorCode);

	
	protected:
		PaStreamParameters inputParameters, outputParameters;
		std::vector<float> data;
		SF_INFO sfinfo;
		const int sampleRate;
		const int framesPerBuffer;
		PaStream *stream = NULL;

};

#endif
