g = 1.2;
t = 0.68;

[a, b] = tsq_coeff(g, t)

% x = sin(linspace(0, 4*pi, 256));
% 
% n = length(x);
% 
% y = zeros(1, n);
% 
% for i=1:n 
%     y(i) = tsq_clip(x(i), t, g, a, b);
% end
% 
% plot(y);

[snd, fs] = audioread('dont_explain_short.wav');
n = length(snd)
snd_clip = zeros(1, n);

for i=1:n 
    snd_clip(i) = tsq_clip(snd(i), t, g, a, b);
end

audiowrite('dont_explain_short_clip.wav', snd_clip, 44100);

