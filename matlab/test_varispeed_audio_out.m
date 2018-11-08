% input signal properties
sr = 48000
dur = 4
hz = 440

% resampler
rs = resampler()

% time vector
t = linspace(0, dur, sr * dur);
% input signal (sinewave)
x = cos(t * hz * 2 * pi);

y = [];

r = sin(linspace(0, pi*3, sr*dur)) + 1;

n = length(x)
for i=1:n 
    rs.r = r(i);
    nf = rs.process(x(i));
    y = (cat(1, y', (rs.out(1, 1:nf))'))';
end

audiowrite('varispeed_sine.wav', y, sr);

z = audioread('../resampler-test/resamp_varispeed.wav');