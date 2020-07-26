from IOHandler import Audio
from IOHandler import AudioDisplay
from audio_processing import AudioProcessor
def main():

	filename = "voice.wav"
	my_audio = Audio("testing")
	
	my_audio.record()
	my_audio.write_to_wav(filename)

	my_audio_processor = AudioProcessor(filename)	
	my_audio_processor.remove_noise()
	
	audio_data = AudioDisplay(my_audio_processor.y, my_audio_processor.sr)
	audio_data.display_spectrogram()
	audio_data.display_chromagram()
	
if __name__ == "__main__":
	main()

