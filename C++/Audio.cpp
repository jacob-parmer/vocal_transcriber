#include "Audio.h"

#include "portaudio.h"
#include <stdio.h>
#include <iostream>
#include <sndfile.h>
#include <cstring>
#include <vector>

static const PaSampleFormat SAMPLE_FORMAT = paFloat32;

Audio::Audio( int channelCount, int sampleRate, int framesPerBuffer) :
			sampleRate(sampleRate),
			framesPerBuffer(framesPerBuffer)
{

	// Display dependancy versions
	char buffer [128];
	sf_command(NULL, SFC_GET_LIB_VERSION, buffer, sizeof(buffer));
	std::cout << "libsndfile version:" << buffer << "\n";
	std::cout << "PortAudio version:" << Pa_GetVersionInfo()->versionText << "\n";

	// Initialize PortAudio and display any errors or warning messages
	PaError errorCode = Pa_Initialize();
	if (errorCode != paNoError) { error(errorCode); }

	// Set up PortAudio input and output to default devices
	inputParameters.device = Pa_GetDefaultInputDevice();
	inputParameters.channelCount = channelCount;
	inputParameters.sampleFormat = SAMPLE_FORMAT;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultHighInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	outputParameters.device = Pa_GetDefaultOutputDevice();
	outputParameters.channelCount = channelCount;
	outputParameters.sampleFormat = SAMPLE_FORMAT;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency;

}

Audio::~Audio() {
	PaError errorCode = Pa_Terminate();
	if (errorCode != paNoError) { error(errorCode); }
}

void Audio::startStream() {
	// Set up stream
	PaError errorCode = Pa_OpenStream(&stream, &inputParameters, &outputParameters,
					  sampleRate, framesPerBuffer, paClipOff, NULL, NULL);
	if (errorCode != paNoError) { error(errorCode);}

	// Start stream
	errorCode = Pa_StartStream(stream);
	if (errorCode != paNoError) { error(errorCode);}
}

void Audio::stopStream() {
	// Stop stream
	PaError errorCode = Pa_StopStream(stream);
	if (errorCode != paNoError) { error(errorCode); }

	// Tear down stream
	errorCode = Pa_CloseStream(stream);
	if (errorCode != paNoError) { error(errorCode); }
}

PaStreamParameters Audio::getInputParameters() {
	return inputParameters;
}

PaStreamParameters Audio::getOutputParameters() {
	return inputParameters;
}

const int Audio::getSampleRate() {
	return sampleRate;
}

const int Audio::getFramesPerBuffer() {
	return framesPerBuffer;
}

void Audio::error(PaError errorCode) {
	std::cout << Pa_GetErrorText(errorCode) << "\n";
}

