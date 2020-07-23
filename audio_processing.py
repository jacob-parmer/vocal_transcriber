import librosa
import librosa.display
import numpy as np
import matplotlib.pyplot as plt

def load_audio(filename):
	y, sr = librosa.load(filename)
	return y, sr

def get_note_in_wav(y, sr, t):	
	pitches, magnitudes = librosa.core.piptrack(y=y, sr=sr)

	index = magnitudes[:, t].argmax()
	frequency = pitches[index, t]
	
	if (frequency != 0):

		pitch = librosa.hz_to_note(int(frequency))
		return pitch 
	else:
		return ['REST']

def display_spectrogram(y, sr):
	S_full, phase = librosa.magphase(librosa.stft(y))

	plt.figure(figsize=(12,4))
	librosa.display.specshow(librosa.amplitude_to_db(S_full, ref=np.max), y_axis='log',
				 x_axis='time',sr=sr)
	plt.colorbar()
	plt.tight_layout()	
	plt.show()

def display_chromagram(y, sr):
	S = np.abs(librosa.stft(y, n_fft=4096))**2
	chroma_og = librosa.feature.chroma_stft(S=S, sr=sr)
	chroma = librosa.decompose.nn_filter(chroma_og, aggregate=np.median, metric='cosine')

	for i, line in enumerate(chroma):
		for j, note in enumerate(line):
			if (note < 0.9):
				chroma[i][j] = 0.1

			print(str(note))
			print("\n")

	fig, ax = plt.subplots()
	img = librosa.display.specshow(chroma, y_axis='chroma', x_axis='time', ax=ax)
	ax.set(title='Chromagram')
	plt.show()
