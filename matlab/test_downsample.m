sr = 48000
dur = 1
hz = 110

% time vector
t = linspace(0, dur, sr * dur);

% input signal (sinewave)
x = sin(t * hz * 2 * pi);

plot(x);


% downsampling ratio
r = 0.8

