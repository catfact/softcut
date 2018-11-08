% x: input vector
% w: window vector
% N: FFT frame size
% R: hop ratio
function X = stft(x, w, N, R)
    M = length(w);
    nframes = floor(((length(x) - M) / R));
    X = zeros(N, nframes);
    zp = zeros(N-M, 1);  
    mR = (M-1) / 2;    % time normalization offset
    xoff = 0;          
    for m=1:nframes
      xt = x(xoff+1:xoff+M); % extract frame of input data
      xtw = w .* xt;         % apply window to current frame
      % plot(xtw);
      xtwz = [xtw(mR+1:M); zp; xtw(1:mR)]; % windowed, zero padded
      % plot(xtwz);
      X(:,m) = fft(xtwz);
      xoff = xoff + R;
    end
end