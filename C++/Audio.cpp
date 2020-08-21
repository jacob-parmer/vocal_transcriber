#include "portaudio.h"
#include <stdio.h>
#include <iostream>
#include <sndfile.h>

static const int NUM_CHANNELS = 1;
static const PaSampleFormat PA_SAMPLE_FORMAT = paFloat32;
static const int SAMPLE_SIZE = 4;
static const int SAMPLE_RATE = 44100;
static const int FRAMES_PER_BUFFER = 512;

class Audio {

	private:
		PaStreamParameters inputParameters, outputParameters;

	public:
		Audio();
		virtual ~Audio();
		int record();
		void error(PaError errorCode);
};

Audio::Audio() {

	// Initialize PortAudio and display any errors or warning messages
	PaError errorCode = Pa_Initialize();
	if (errorCode != paNoError) { error(errorCode); }

	// Display dependancy versions
	char buffer [128];
	sf_command(NULL, SFC_GET_LIB_VERSION, buffer, sizeof(buffer));
	std::cout << buffer << "\n";
	std::cout << Pa_GetVersionInfo()->versionText << "\n";

	// Set up PortAudio input and output to default devices
	inputParameters.device = Pa_GetDefaultInputDevice();
	inputParameters.channelCount = NUM_CHANNELS;
	inputParameters.sampleFormat = PA_SAMPLE_FORMAT;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultHighInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	outputParameters.device = Pa_GetDefaultOutputDevice();
	outputParameters.channelCount = NUM_CHANNELS;
	outputParameters.sampleFormat = PA_SAMPLE_FORMAT;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency;

}

Audio::~Audio() {
	PaError errorCode = Pa_Terminate();
	if (errorCode != paNoError) { error(errorCode); }
}

int Audio::record() {

	PaStream *stream = NULL;
	char *sampleBlock = NULL;
	int numBytes = FRAMES_PER_BUFFER * NUM_CHANNELS * SAMPLE_SIZE;
	sampleBlock = (char *) malloc(numBytes);

	// Set up stream
	PaError errorCode = Pa_OpenStream(&stream, &inputParameters, &outputParameters, SAMPLE_RATE,
		       			  FRAMES_PER_BUFFER, paClipOff, NULL, NULL);
	if (errorCode != paNoError) { error(errorCode); }

	// Start stream
	errorCode = Pa_StartStream(stream);
	if (errorCode != paNoError) { error(errorCode); }


	std::cout << "Recording...\n";
	for (int i=0; i<(2*SAMPLE_RATE)/FRAMES_PER_BUFFER; ++i) {
	
		errorCode = Pa_WriteStream(stream, sampleBlock, FRAMES_PER_BUFFER);
		if (errorCode != paNoError) { error(errorCode); }
	
		std::cout << sampleBlock << "\n";

		errorCode = Pa_ReadStream(stream, sampleBlock, FRAMES_PER_BUFFER);
		if (errorCode != paNoError) { error(errorCode); }

	
	}



	// Stop Stream
	errorCode = Pa_StopStream(stream);
	if (errorCode != paNoError) { error(errorCode); }

	// Tear down stream
	errorCode = Pa_CloseStream(stream);
	if (errorCode != paNoError) { error(errorCode); }

	return 0;

}

void Audio::error(PaError errorCode) {
	std::cout << Pa_GetErrorText(errorCode) << "\n";
}

int main() {

	Audio* ad = new Audio();
	int err = ad->record();
	delete ad;

}
