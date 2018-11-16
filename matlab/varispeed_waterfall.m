Y = spectrogram(y', 4096 * 8);
mesh(downsample(imresize(Y, 0.5), 32)');
% set(gca,'ZScale','log')
view([2 12])
title("output STFT (matlab)");