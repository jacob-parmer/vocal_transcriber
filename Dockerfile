FROM ubuntu
WORKDIR /usr/src/vocal_transcriber
RUN apt-get -y update && apt-get -y install mercurial 
RUN  hg clone https://code.soundsoftware.ac.uk/hg/pyin

FROM gcc:4.9
COPY . /usr/src/vocal_transcriber
WORKDIR /usr/src/vocal_transcriber
RUN g++ -o Test C++/Test.cpp

CMD ["./Test"]
