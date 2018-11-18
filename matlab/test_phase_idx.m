x = softcut_test_buffers();

if 1
    phase = x(1,:);
    idx = x(2,:);
    t = (1:length(x));

    plot(t, phase, t, idx);
else
    a = 40000;
    b = 100000;

    phase = x(1,a:b);
    idx = x(2,a:b);
    t = (1:length(phase));

%    plot(t, phase, t, idx)
    stem(floor(phase) - idx, '-o', 'Markersize', 3);
    ylim([7, 10])
    xlim([30000 30250])
end

%{
resampler_rate = 0.47000950574874878

%}