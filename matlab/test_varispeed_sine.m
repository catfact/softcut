% input signal properties
sr = 48000
dur = 1
hz = 440

% resampler
rs = resampler()
rs.r = 4/9;

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
help
% apply lowpass butterworth
lpf_fc = 12000;
[lpf_b, lpf_a] = butter(6, lpf_fc/(sr/2), 'low');

y = filter(lpf_b, lpf_a, y);

subplot(3, 1, 1);
plot(x,'-', 'MarkerSize', 4);
title("input");

subplot(3, 1, 2);
plot(r);
title("resampling ratio");

subplot(3, 1, 3);
plot(y,'-', 'MarkerSize', 4);
title("output");