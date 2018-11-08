function Y = spectrogram(x, N)
% x: input vector
% N: FFT frame size
    M = 1025;       % window size
    R = (M-1)/2;    % hop size
    % L = (M-1)*8;    % signal length

    wa = hamming(M);        % analysis window
    wa(1) = wa(1) * 0.5;    % endpoints /2 for COLA
    wa(M) = wa(M) * 0.5;

    Y = stft(x, wa, N, R);
    % return only real magnitudess
    Y = Y(1:N/2, :);
    Y = abs(Y);
end