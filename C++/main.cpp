#include "AudioUtil.h"
#include "AudioFeatures.h"

int main() {

	int seconds = 4;
	const int channelCount = 1;
	const int sampleRate = 48000;
	const int framesPerBuffer = 1024;
	
	AudioUtil* ad = new AudioUtil(channelCount, sampleRate, framesPerBuffer);
	int err  = ad->record(seconds);
	err = ad->writeWAV("test.wav");

	AudioFeatures* ft = new AudioFeatures(channelCount, sampleRate, framesPerBuffer); 
	ft->setAudioData(ad->getAudioData());
	ft->processPitches();

	delete ad;
	delete ft;
}
