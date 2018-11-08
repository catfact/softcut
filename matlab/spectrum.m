% return the single-sided real amplitude spectrum of the given vector
% x = inoput
% n = FFT length
% fs = sampling frequency
function y = spectrum(x, n, fs)
    if n > length(x)
        n = length(x);
    end
    y = abs(fft(x));
    y = y(1:n/2) / fs;
end
