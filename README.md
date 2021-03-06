# notice

this project has become stale. current softcut lives in the main norns repo, and is baked into the `crone` program as one of two jack / OSC clients (the other is a mixer.)

https://github.com/monome/norns/tree/dev/crone

someday i will find time to refactor it into a proper library and port to other platforms/environments.

-------------

# softcut
library for seamless audio looping

test project for crossfaded sample playback/recording with read/write interpolation and partial erase.

`softcut` creates a pair of read/write "head" structures, and runs under Portaudio (on mac) or jack (linux.) 

the audio process can be controlled by OSC on port number 9999. see `OscInterface.h` for command paths.

## building on norns

requirements:
```
sudo apt-get install cmake 
sudo apt-get install libboost-dev
sudo apt-get install faust-common
```

[other requirements should already be installed (libjack-dev, liblo).]

configure and build:
```
mkdir build && cd build
cmake ..
make
```

to just build and run the main target (from the top directory)
```
cmake --build build/ --target softcut
./build/softcut
```

running: `./softcut`. included is a supercollider script to send test OSC commands (`test-osc.scd`); edit the NetAddr IP for local/remote use.
