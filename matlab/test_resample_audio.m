% resampler
rs = resampler();
rs.r = 0.46

% input
x = audioread('../src/resampler-test/dont_explain_short.wav');
n = length(x)
y = zeros(1, int32((n * rs.r) + 10));
m = 1;

for i=1:n 
    nf = rs.process(x(i));
    for j=1:nf 
        y(m) = rs.out(1, j);
        m = m + 1;
    end
    if mod(i, 1000) == 0 
        fprintf(1, '%d \n', cast(i / n * 100, 'int32'));
    end
end

audiowrite('../src/resampler-test/dont_explain_short_resampled_matlab.wav', y, 16000);