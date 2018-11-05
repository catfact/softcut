# softcut
library for seamless audio looping

test project for crossfaded sample playback/recording with interpolation and partial erase.

`softcut` creates a single read/write "head" structure and runs it under Portaudio. 

the audio process can be controlled by OSC on port number 9999. see `OscInterface.h` for command paths.

---------


requirements:

- cmake
- portaudio
- liblo


currently only tested on macOS 10.14, where libraries are assumed to be installed to `/usr/local` (e.g. by homebrew.)

will probably require different include paths under linux but otherwise should work.

--------
caveats:

- write-head resampling is only linear and so doesn't sound very good. i'm pretty sure it is correct (based on output spectrograms) but should compare against a canonical implementation like SRC to be sure.

higher-order resampling should be pretty straightforward to add - `Resampler.h` is primitive (doesn't have required input ringbuffer, only output ringbuffer,) but at least it is a separate module. 
