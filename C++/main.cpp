#include "AudioUtil.h"

int main() {

	int seconds = 15;
	const int channelCount = 1;
	const int sampleRate = 48000;
	const int framesPerBuffer = 1024;
	AudioUtil* ad = new AudioUtil(channelCount, sampleRate, framesPerBuffer);
	int err  = ad->record(seconds);
	const char *filename = "recording.wav";
	err = ad->writeWAV(filename);
	err = ad->readWAV(filename);
	delete ad;
}
