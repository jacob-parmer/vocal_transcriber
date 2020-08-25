#include "AudioUtil.h"

int main() {

	int seconds = 5;
	const int channelCount = 1;
	const int sampleRate = 44100;
	const int framesPerBuffer = 512;
	AudioUtil* ad = new AudioUtil(channelCount, sampleRate, framesPerBuffer);
	int err = ad->record(seconds);
	const char *filename = "recording.wav";
	err = ad->writeWAV(filename);
	delete ad;
}
