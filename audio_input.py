import pyaudio
import wave
import sys

# Opens a PyAudio Session, prints dividing line due to common warning messages in terminal
# INPUTS: N/A
#
# OUTPUTS: p - pyAudio session 
def open_audio_session():
	p = pyaudio.PyAudio()
	print("------------------------------------------------------------------------------\n")
	return p

# Opens a pyAudio stream and stores incoming audio data in "frames"
# INPUTS: p - pyAudio Session
#	  ad_format - format of audio data in bits and type (e.g. paInt16 = 16 bit Int)
#	  channels - desired number of input channels
#	  rate - sample rate of audio in Hz 
#	  record_seconds - how long audio will be recorded for in seconds
#	  chunk - amount of frames in each buffer of stream
#
# OUTPUTS: frames - list containing all audio data found during stream
def record_audio(p, ad_format=pyaudio.paInt16, channels=1, rate=44100, record_seconds=10, chunk=1024):


	stream = p.open(format=ad_format,
			channels=channels,
			rate=rate,
			input=True,
			frames_per_buffer=chunk)

	print("* recording")

	frames = []

	# play stream
	for i in range(0, int(rate  / chunk * record_seconds)):
		data = stream.read(chunk)
		frames.append(data)

	print("* finished recording")
	stream.stop_stream()
	stream.close()
	return frames

# Closes PyAudio session
# INPUTS: p - pyAudio Session
# 
# OUTPUTS: N/A
def close_audio_session(p):
	p.terminate()

# Takes frame input data and turns it into a .wav file
# INPUTS: p - pyAudio Session
#	  wave_output_filename - desired filename to store .wav file
#	  frames - list data to be written
#	  channels - number of input channels in frames (MUST MATCH WHAT WAS RECORDED)
#	  ad_format - format of audio data to be stored
#	  rate - audio sample rate in Hz of data to be stored
#
# OUTPUTS: N/A
def write_frames_to_wav(p, wave_output_filename, frames, channels=1, ad_format=pyaudio.paInt16, rate=44100): 
	wf = wave.open(wave_output_filename, "wb")
	wf.setnchannels(channels)
	wf.setsampwidth(p.get_sample_size(ad_format))
	wf.setframerate(rate)
	wf.writeframes(b''.join(frames))
	wf.close()

