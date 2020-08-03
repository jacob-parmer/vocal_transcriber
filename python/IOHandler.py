import pyaudio
import wave
import sys
import librosa
import matplotlib.pyplot as plt
import numpy as np

class Audio:

	# INPUTS: name - ID of audio object
	# 	  ad_format - format of audio data (e.g. paInt16 = 16 bit Int)
	#	  channels - number of audio channels
	#         rate - sample rate of audio in Hz
	def __init__(self, name, ad_format=pyaudio.paInt16, channels=1, rate=44100):
		self.name = name
		self.frames = []
		self.ad_format= ad_format
		self.channels = channels
		self.rate = rate
		self.p = pyaudio.PyAudio()
		print("------------------------------------------------------------------\n")
	
	# Stores incoming audio data in "frames"
	# INPUTS: record_seconds - how long audio will be recorded for in seconds
	#	  chunk - amount of frames in each buffer of stream
	#
	# OUTPUTS: N/A 
	def record(self, record_seconds=10, chunk=1024):


		stream = self.p.open(format=self.ad_format,
				channels=self.channels,
				rate=self.rate,
				input=True,
				frames_per_buffer=chunk)

		print("* recording")

		# play stream
		for i in range(0, int(self.rate  / chunk * record_seconds)):
			data = stream.read(chunk)
			self.frames.append(data)

		print("* finished recording")
		stream.stop_stream()
		stream.close()

	# Takes frame input data and turns it into a .wav file
	# INPUTS: wave_output_filename - desired filename to store .wav file
	#	  channels - number of input channels in frames (MUST MATCH WHAT WAS RECORDED)
	#	  ad_format - format of audio data to be stored
	#	  rate - audio sample rate in Hz of data to be stored
	#
	# OUTPUTS: N/A
	def write_to_wav(self, wave_output_filename): 
		wf = wave.open(wave_output_filename, "wb")
		wf.setnchannels(self.channels)
		wf.setsampwidth(self.p.get_sample_size(self.ad_format))
		wf.setframerate(self.rate)
		wf.writeframes(b''.join(self.frames))
		wf.close()

	def __del__(self):
		self.p.terminate()

class AudioDisplay:

	def __init__(self, y, sr):
		self.y = y
		self.sr = sr

	def display_spectrogram(self):
		S_full, phase = librosa.magphase(librosa.stft(self.y))
		plt.figure(figsize=(12,4))
		librosa.display.specshow(librosa.amplitude_to_db(S_full, ref=np.max),
					 y_axis='log', x_axis='time', sr=self.sr)

		plt.colorbar()
		plt.tight_layout()
		plt.show()

	def display_chromagram(self):
		S = np.abs(librosa.stft(self.y, n_fft=4096))**2
		chroma_og = librosa.feature.chroma_stft(S=S, sr=self.sr)
		chroma = librosa.decompose.nn_filter(chroma_og, aggregate=np.median,
						     metric='cosine')
		fig, ax = plt.subplots()
		img = librosa.display.specshow(chroma, y_axis='chroma', x_axis='time', ax=ax)
		plt.show()
