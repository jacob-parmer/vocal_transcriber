import librosa
import librosa.display
import numpy as np
import matplotlib.pyplot as plt
import noisereduce as nr
from IOHandler import Audio
from statsmodels.graphics.tsaplots import plot_acf
class AudioProcessor:

	def __init__(self, filename):
		self.filename = filename
		self.y, self.sr = librosa.load(filename)

	def remove_noise(self):
		noise_file = "noise.wav"
		noise_audio = Audio("Noise")
	
		print("Recording noise levels, please be quiet")

		noise_audio.record(record_seconds=5)
		noise_audio.write_to_wav(noise_file)

		noise_clip = AudioProcessor(noise_file)
		
		reduced_noise = nr.reduce_noise(audio_clip=self.y, noise_clip=noise_clip.y)
		return reduced_noise

class PitchClassifier(AudioProcessor):

	def get_pitches_from_wav(self):
		self.estimate_autocorrelation(tau=3, window_length=245)
		plot_acf(self.y)
		plt.show()

	def yin_algorithm(self):
		# Step 1. Autocorrelation method
		at = np.fft.fft(self.y)
