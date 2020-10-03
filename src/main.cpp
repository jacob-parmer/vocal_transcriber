#include "AudioUtil.h"
#include "AudioFeatures.h"
#include "SignalProcessing.h"
#include <fstream>

int main() {

	int seconds = 7;
	const int channelCount = 1;
	const int sampleRate = 48000;
	const int framesPerBuffer = 1024;
	
	AudioUtil* ad = new AudioUtil(channelCount, sampleRate, framesPerBuffer);
	int err  = ad->record(seconds);
	err = ad->writeWAV("test.wav");
	
	Signal sig;
	std::vector<double> sigZCR;
	std::vector<float> audioData = ad->getAudioData();
	
	int dataSize = audioData.size();
	for (int i=0; i<dataSize; i=i+framesPerBuffer) {
		double frameZCR = sig.zeroCrossingRate(audioData.data() + i, framesPerBuffer);
		sigZCR.push_back(frameZCR);
	}


	std::cout << "Testing123";

	delete ad;
}
