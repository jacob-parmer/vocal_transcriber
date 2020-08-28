#include "AudioUtil.h"

static const int SAMPLE_SIZE = 4;

AudioUtil::AudioUtil(int channelCount, int sampleRate, int framesPerBuffer)
	: Audio(channelCount, sampleRate, framesPerBuffer) {

	// initialize information required for libsndfile library
	memset(&sfinfo, 0, sizeof(sfinfo));

	sfinfo.format = (SF_FORMAT_WAV | SF_FORMAT_FLOAT);
	sfinfo.channels = channelCount;
	sfinfo.samplerate = sampleRate;

};
AudioUtil::~AudioUtil() { };

/* Records audio data from default input device to data vector. 
 *
 * PARAMS: seconds (int) - number of seconds recording should last
 *
 * OUTPUTS: data (vector<float>) - large vector of 32 bit float values representing input audio
 *
 * RETURNS: -1 if a failure happens during stream read, 0 if not.
 *
 */
int AudioUtil::record(int seconds) {

	int numBytes = framesPerBuffer * inputParameters.channelCount * SAMPLE_SIZE;
	float *sampleBlock = (float *) malloc(numBytes);

	memset(sampleBlock, 0.0f, numBytes);

	Audio::startStream();
	
	std::cout << "Recording...\n";
	for (int i=0; i<(seconds*sampleRate)/framesPerBuffer; ++i) {
		PaError errorCode = Pa_ReadStream(stream, sampleBlock, framesPerBuffer);
		if (errorCode != paNoError) { error(errorCode); return -1;}
		
		float* ptr = sampleBlock;
		for (int j = 0; j  < framesPerBuffer; ++j) {
			data.push_back(*ptr);
			ptr = ptr + 1;
		}
	}
	std::cout << "Finished recording.\n";
	
	Audio::stopStream();

	return 0;
}

/* Writes audio data to WAV using libsndfile. 
 *
 * PARAMS: filename (const char*) - name of the file to write to. This will overwrite
 *				    if the file already exists.
 *
 * OUTPUTS: wav file with data loaded into AudioUtil object. 
 *
 * RETURNS: -1 if an error occurred, 0 if not.
 *
 */
int AudioUtil::writeWAV(const char* filename) {

	if (dataIsEmpty()) { 
		std::cout << "Data not initialized before writeWAV called."
		          << "Load data using record() or readWAV(). \n";
		return -1;
	}

	const float* ptr;
	ptr = &data.front();

	if (! (outfile = sf_open(filename, SFM_WRITE, &sfinfo))) {
		std::cout << "Not able to open output file " << filename << ".\n";
		puts(sf_strerror(NULL));
		return -1;
	}

	/* this is where all audio data is written */
	sf_write_float(outfile, ptr, data.size());

	sf_close(outfile);

	return 0;
}

int AudioUtil::readWAV(const char* filename) {
	
	int readcount;
	float *buffer;

	/* if we've previously wrote a WAV file, data is already loaded. This removes that
	 * pre-existing data. */
	if (!dataIsEmpty()) {
		dataFlush();
	}

	if (! (infile = sf_open(filename, SFM_READ, &sfinfo))) {
		std::cout << "Not able to open input file " << filename << ".\n";
		puts(sf_strerror (NULL));
		return -1;

	}
	while ((readcount = sf_read_float(infile, buffer, framesPerBuffer))) {
		std::cout << readcount << "\n";
		
		float* ptr = buffer;
		for(int i=0; i<readcount; ++i) {
			data.push_back(*ptr);
			ptr = ptr + 1;
		}	
	}

	sf_close(infile);

	return 0;
}

/* Used to determine if data member variable has been initialized.
 *
 * PARAMS: None
 *
 * OUTPUTS: None
 *
 * RETURNS: true if data member variable is empty, false if not.
 *
 */
bool AudioUtil::dataIsEmpty() {
	if (data.size() == 0) {
		return true;
	} return false;
}


void AudioUtil::dataFlush() {
	data.clear();
	data.resize(0);
}
