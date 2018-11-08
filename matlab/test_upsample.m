% input signal properties
sr = 48000
dur = 0.002
hz = 440

% resampler
rs = resampler()
rs.r = 8/5;

% time vector
t = linspace(0, dur, sr * dur);
% input signal (sinewave)
x = cos(t * hz * 2 * pi);

y = [];

n = length(x)
for i=1:n 
    nf = rs.process(x(i));
    y = (cat(1, y', (rs.out(1, 1:nf))'))';
end

subplot(2, 1, 1);
plot(x,'-*', 'MarkerSize', 4);

subplot(2, 1, 2);
plot(y,'-*', 'MarkerSize', 4);
