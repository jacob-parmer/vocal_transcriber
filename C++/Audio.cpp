#include "portaudio.h"
#include <iostream>

static const int NO_PA_ERROR = 0;

class Audio {

	public:
	Audio();
	const char* getPAVersion();
	virtual ~Audio();
	
};

Audio::Audio() {
	PaError errorCode = Pa_Initialize();
	if (errorCode != paNoError) {
		std::cout << Pa_GetErrorText(errorCode) << "\n";
	}	
}

const char* Audio::getPAVersion() {

	const PaVersionInfo* version = Pa_GetVersionInfo();
	return version->versionText;
}

Audio::~Audio() {
	PaError errorCode = Pa_Terminate();
	if (errorCode != paNoError) {
		std::cout << Pa_GetErrorText(errorCode) << "\n";
	}
}
int main() {

	Audio* ad = new Audio();
	std::cout << ad->getPAVersion() << "\n";
	delete ad;

}
