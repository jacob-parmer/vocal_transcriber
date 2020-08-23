#include "portaudio.h"
#include <stdio.h>
#include <iostream>
#include <sndfile.h>
#include <cstring>

static const int NUM_CHANNELS = 2;
static const PaSampleFormat SAMPLE_FORMAT = paFloat32;
static const int SAMPLE_SIZE = 4;
static const int SAMPLE_RATE = 44100;
static const int FRAMES_PER_BUFFER = 512;

class Audio {

	private:
		PaStreamParameters inputParameters, outputParameters;
		SF_INFO fileParameters;
	public:
	Audio();
		virtual ~Audio();
		int record(int seconds);
		void writeWAV(float *value, const char* filename);
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
	inputParameters.sampleFormat = SAMPLE_FORMAT;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultHighInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	outputParameters.device = Pa_GetDefaultOutputDevice();
	outputParameters.channelCount = NUM_CHANNELS;
	outputParameters.sampleFormat = SAMPLE_FORMAT;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency;

}

Audio::~Audio() {
	PaError errorCode = Pa_Terminate();
	if (errorCode != paNoError) { error(errorCode); }
}

int Audio::record(int seconds) {

	PaStream *stream = NULL;
	float *sampleBlock = NULL;
	int numBytes = FRAMES_PER_BUFFER * NUM_CHANNELS * SAMPLE_SIZE;
	sampleBlock = (float *) malloc(numBytes);

	memset(sampleBlock, 0.0f, numBytes);
	
	// Set up stream
	PaError errorCode = Pa_OpenStream(&stream, &inputParameters, &outputParameters, SAMPLE_RATE,
		       			  FRAMES_PER_BUFFER, paClipOff, NULL, NULL);
	if (errorCode != paNoError) { error(errorCode); }

	// Start stream
	errorCode = Pa_StartStream(stream);
	if (errorCode != paNoError) { error(errorCode); }

	std::cout << "Recording...\n";
	for (int i=0; i<(seconds*SAMPLE_RATE)/FRAMES_PER_BUFFER; ++i) {
		errorCode = Pa_ReadStream(stream, sampleBlock, FRAMES_PER_BUFFER);
		if (errorCode != paNoError) { error(errorCode); }

		std::cout << *sampleBlock << "\n";
	}

	float *value{ static_cast<float*>(stream) };
	std::cout << "Finished recording.\n";

	Audio::writeWAV(value, "recording.wav");

	// Stop Stream
	errorCode = Pa_StopStream(stream);
	if (errorCode != paNoError) { error(errorCode); }

	// Tear down stream
	errorCode = Pa_CloseStream(stream);
	if (errorCode != paNoError) { error(errorCode); }

	return 0;

}

void Audio::writeWAV(float *value, const char* filename) {

	memset (&fileParameters, 0, sizeof(fileParameters));
	
	SNDFILE *file; 	
       
	if (! (file = sf_open(filename, SFM_WRITE, &fileParameters))) {
		printf("Not able to open output file %s.\n", filename);	
	};




	sf_close(file);
}

void Audio::error(PaError errorCode) {
	std::cout << Pa_GetErrorText(errorCode) << "\n";
}

int main() {

	int seconds = 5;
	Audio* ad = new Audio();
	int err = ad->record(seconds);
	delete ad;

}
