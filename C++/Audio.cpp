#include "Audio.h"

#include "portaudio.h"

static const PaSampleFormat SAMPLE_FORMAT = paFloat32;
static const int SAMPLE_SIZE = 4; // 32 bit samples, 4 bytes

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

	// Set up libsndfile parameters in variable sfinfo
	memset(&sfinfo, 0, sizeof(sfinfo));

	sfinfo.format = (SF_FORMAT_WAV | SF_FORMAT_FLOAT);
	sfinfo.channels = channelCount;
	sfinfo.samplerate = sampleRate;
}

Audio::~Audio() {
	PaError errorCode = Pa_Terminate();
	if (errorCode != paNoError) { error(errorCode); }
}

void Audio::startStream() {
	PaError errorCode = Pa_OpenStream(&stream, &inputParameters, &outputParameters,
					  sampleRate, framesPerBuffer, paClipOff, NULL, NULL);
	if (errorCode != paNoError) { error(errorCode);}

	errorCode = Pa_StartStream(stream);
	if (errorCode != paNoError) { error(errorCode);}
}

void Audio::readStream(bool &stop) {
	int sizeOfBuffer = framesPerBuffer * inputParameters.channelCount * SAMPLE_SIZE;
	float *buffer = (float *) malloc(sizeOfBuffer);

	memset(buffer, 0.0f, sizeOfBuffer);

	while (!stop) {
		PaError errorCode = Pa_ReadStream(stream, buffer, framesPerBuffer);
		if (errorCode != paNoError) { error(errorCode); }

		// loops through stored values in buffer
		float* ptr = buffer;
		for (int i=0; i<framesPerBuffer; ++i) {
			data.push_back(*ptr);
			ptr = ptr + 1;
		}
	}
}

void Audio::stopStream() {
	// Stop stream
	PaError errorCode = Pa_StopStream(stream);
	if (errorCode != paNoError) { error(errorCode); }

	// Tear down stream
	errorCode = Pa_CloseStream(stream);
	if (errorCode != paNoError) { error(errorCode); }
}

bool Audio::dataIsEmpty() {
	return (data.size() == 0);
}

void Audio::dataFlush() {
	data.clear();
	data.resize(0);
}

void Audio::error(PaError errorCode) {
	std::cout << Pa_GetErrorText(errorCode) << "\n";
}

