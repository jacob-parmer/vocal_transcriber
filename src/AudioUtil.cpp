#include "AudioUtil.h"

AudioUtil::AudioUtil(int channelCount, int sampleRate, int framesPerBuffer)
	: Audio(channelCount, sampleRate, framesPerBuffer) { };

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

	Audio::startStream();
	
	std::cout << "Recording...\n";
	
	bool stop = false;

	// Branches off to read stream while main thread waits to stop the function.
	std::thread read (&Audio::readStream, this, std::ref(stop));
	std::this_thread::sleep_for (std::chrono::seconds(seconds));
	stop = true;

	read.join();

	std::cout << "Finished recording.\n";
	
	Audio::stopStream();

	return 0;
}

/* Writes audio data to WAV using libsndfile. 
 *
 * PARAMS: filename (const char*) - Name of the file to write to. This will overwrite
 *				    if the file already exists.
 *
 * OUTPUTS: WAV file with data loaded into AudioUtil object. 
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

/* Reads WAV file into audio data vector using libsndfile.
 *
 * PARAMS: filename (const char*) - Name of the file to read from.
 *
 * OUTPUTS: Audio signal in data vector
 * 	    
 * RETURNS: -1 if function failed, 0 if not.
 *
 */
int AudioUtil::readWAV(const char* filename) {
	
	int readcount;
	float *buffer;

	/* if we've previously wrote a WAV file, data is already loaded. This removes that
	 * pre-existing data. Not recommended, just make a new Audio object. */
	if (!dataIsEmpty()) {
		std::cout << "WARNING: Rewriting data with readWAV() w/o flushing beforehand.\n";
		Audio::dataFlush();
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

