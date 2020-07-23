from IOHandler import Audio
import audio_processing
def main():

	filename = "voice.wav"
	my_audio = Audio("testing")
	
	my_audio.record()
	my_audio.write_frames_to_wav(filename)

	y, sr = audio_processing.load_audio(filename)
	for t in range(0, 400):
		print(str(audio_processing.get_note_in_wav(y, sr, t)))

	
	audio_processing.display_chromagram(y, sr)	

if __name__ == "__main__":
	main()

