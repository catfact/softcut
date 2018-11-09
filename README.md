# softcut
library for seamless audio looping

test project for crossfaded sample playback/recording with read/write interpolation and partial erase.

`softcut` creates a single read/write "sch" structure and runs it under Portaudio (on mac) or jack (linux.) 

the audio process can be controlled by OSC on port number 9999. see `OscInterface.h` for command paths.

## building on norns

requirements:
```
sudo apt-get install cmake 
sudo apt-get install libboost-dev
```

[other requirements should already be installed (libjack-dev, liblo).]

configure and build:
```
mkdir build && cd build
cmake ..
make
```

running: `./softcut`. included is a supercollider script to send test OSC commands (`test-osc.scd`); edit the NetAddr IP for local/remote use.
