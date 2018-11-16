n = 8192 * 8;
x = rand(1, n) * 2 - 1;
sr = 48000;

%fx = brickwall_butter8(x);
% fx = test_filter_fir(x);

% % ok, dummy test with 1-pole lowpass
% a = 0.1; %coeff
% y0 = 0; % state
% y = zeros(1, n);
% for i=1:n
%     y(i) = y0;
%     y0 = y0 + (x(i) - y0) * a;
% end
% fx = y;


%% wtf... none of these are doing anything
% fx = brickwall_butter8(x);
% fx = test_filter_fir(x);
% fx = lpf_butter_2(x);

lpFilt = designfilt('lowpassfir','PassbandFrequency',0.25, ...
         'StopbandFrequency',0.35,'PassbandRipple',0.5, ...
         'StopbandAttenuation',65,'DesignMethod','kaiserwin');
fx = filter(lpFilt, x);



subplot(2, 1, 1);
plot(spectrum(x, n, sr));

subplot(2, 1, 2);
plot(spectrum(fx, n, sr));

