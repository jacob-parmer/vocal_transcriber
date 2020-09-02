#include "Audio.h"

#include "portaudio.h"

static const PaSampleFormat SAMPLE_FORMAT = paFloat32;
static const int SAMPLE_SIZE = 4; // 32 bit samples, 4 bytes

/* Declaration of Audio object. This object is supposed to handle lower-level ideas of audio
 * handling, such as the handling of streams and floating point data representative of the audio.
 *
 * PARAMS: channelCount (int) - Number of channels to be passed. For a microphone, this will only
 * 				be 1.
 *
 * 	   sampleRate (int) - Number of samples of the incoming signal to grab per second.
 * 	   		      Common sampleRates are 44100 (CD's) and 48000 (Modern recordings) 
 *
 *	   framesPerBuffer (int) - Number of frames to store in each buffer when read/writing.
 *
 * OUTPUTS: Version #'s to stdout
 * 	    inputParameters and outputParameters for PortAudio stream handling
 * 	    sfinfo information for libsndfile
 */
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

/* Destructor, called to terminate PortAudio session when done with Audio object.  */
Audio::~Audio() {
	PaError errorCode = Pa_Terminate();
	if (errorCode != paNoError) { error(errorCode); }
}

/* Begins PortAudio stream. Must be called after Audio object has been created so that PortAudio
 * information has been initialized.
 *
 * OUTPUTS: Initialized PaStream object. (PaStream is really just a void* that accepts data from
 * 	    default device.)
 */
void Audio::startStream() {
	PaError errorCode = Pa_OpenStream(&stream, &inputParameters, &outputParameters,
					  sampleRate, framesPerBuffer, paClipOff, NULL, NULL);
	if (errorCode != paNoError) { error(errorCode);}

	errorCode = Pa_StartStream(stream);
	if (errorCode != paNoError) { error(errorCode);}
}

/* Reads PortAudio information from input default device. This information is stored in the form
 * of a vector of 32-bit floats named "data".
 *
 * PARAMS: &stop (bool address) - Parameter by reference, tells stream when to stop taking input.
 * 
 * OUTPUTS: data (vector<float>) - 32float data of audio signal. If runnning for any significant
 * 				   amount of time, this should be a large vector!
 *
 */
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

/* Stops and tears down PortAudio stream. Should be called in correspondence with
 * Audio::startStream(). */
void Audio::stopStream() {
	// Stop stream
	PaError errorCode = Pa_StopStream(stream);
	if (errorCode != paNoError) { error(errorCode); }

	// Tear down stream
	errorCode = Pa_CloseStream(stream);
	if (errorCode != paNoError) { error(errorCode); }
}

/* Returns true if data vector is empty, returns false if data vector has elements inside it. */
bool Audio::dataIsEmpty() {
	return (data.size() == 0);
}

/* Empties data variable for re-use with a different Audio signal. */
void Audio::dataFlush() {
	data.clear();
	data.resize(0);
	data.reserve(sampleRate); // reserves 1 second worth of memory for new data
}

/* Returns the data found in this Audio object.  */
std::vector<float> Audio::getAudioData() { return this->data; }

/*
 *
 *
 */
void Audio::writeDataToCSV() {
	std::ofstream myFile("data.csv");
	for (int i=0; i<data.size(); i++) {
		myFile << this->data.at(i) << "\n";
	}
}
/* Sets the Audio object's data to input value. */
void Audio::setAudioData(std::vector<float> data)  { this->data = data; }

/* Error handling. Called to display errors if something goes wrong with a PortAudio session.
 *
 * PARAMS: errorCode (PaError) - errorCode as listed in PortAudio by given PaError struct. 
 * 				 See PortAudio documentation for more information.
 *
 * OUTPUTS: Pa_GetErrorText(), provided textual description for the given errorCode.
 *
 */
void Audio::error(PaError errorCode) {
	std::cout << Pa_GetErrorText(errorCode) << "\n";
}

