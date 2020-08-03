from IOHandler import Audio
from IOHandler import AudioDisplay
from audio_processing import AudioProcessor
from audio_processing import PitchClassifier
import os
def main():

	"""
	filename = "voice.wav"
	
	my_audio = Audio("testing")

	
	my_audio.record(record_seconds=10)
	my_audio.write_to_wav(filename)
	
	my_audio_processor = AudioProcessor(filename)	
	
	my_audio_processor.remove_noise()
	"""
	pitch = PitchClassifier('vocal_acoustic_000-050-025.wav')
	#pitch.train_ML(verbose=True)
	pitch.get_pitches_from_wav()

	

	
if __name__ == "__main__":
	main()

