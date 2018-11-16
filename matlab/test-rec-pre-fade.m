% problem: 
% when crossfading, we need to correctly modulate the record and pre-record
% levels as well as the playback level.

% requirements:
% - at the end of the xfade, should have rec == 0, pre == 1.
%
% - there should never be perceived volume drops or volume spikes after
% repeated passes over the same material
%
% - rec/pre curves should be "smooth," preferably with continuous derivative.
% 
% - rec should be symmetrical, and rec(x) + rec(1-x) == 1.
%
% we'll simulate the effects of repeatedly looping over the same part of a
% buffer with record and crossfade. 
%
% in the SoftCut structure, there are 2 alternating heads doing this,
% and also playing back recorded sample values. 
% so during the fade, there are actually two passes of overdub on each
% sample.

% count of samples in each fade section (in and out)
nf = 200;
% count of samples to hold without fade
nh = 200;

n = nf*2 + nh;

% initial input signal (impulse)
in = ones(1, n);

% count of iterations
count = 100;

buf = zeros(1, n);

% base rec/pre levels (before fade)
rec0 = 1.0;
pre0 = 0.9;

% compute rec/pre level buffers
[rec, pre] = make_curves(n, nf, rec0, pre0);

% signal history
h = [];

% make the initial record pass
buf = apply(buf, in, rec, pre);

plot(buf);

% apply additional record passes with zero input
% we should see a smooth decay of the original noise burst,
% without bumps or dips (besides the initial imprint of the fade curves)


% with zero input (impulse response)
z = zeros(1, n);

% with constant input (step response)
% z = ones(1,n) * 0.5;

for i=1:count
    % sum the buffer with a shifted copy of itself, to better observe xfade
    nsum = nf*3 + nh*2;
    sumbuf = zeros(1, nsum);
    sumbuf(1:n) = buf;
    sumbuf(nf+nh+1:nsum) = sumbuf(nf+nh+1:nsum) + buf;
    h = [h; sumbuf];
    buf = apply(buf, z, rec, pre);
end

subplot(4, 1, 1)
plot(pre, 'DisplayName', sprintf('pre=%f',pre0));
hold on;
plot(rec, 'DisplayName', sprintf('rec=%f',rec0))
legend;
ylim([0 1.2]);
hold off;

subplot(4, 1, [2 3 4])
waterfall(h);
view([100 20]);

% apply simulated recording pass
% x0: current buffer
% x: new signal
function y = apply(x0, x, r, p)
    n = length(x0);
    y = zeros(1, n);
    for i=1:n 
        y(i) = (x0(i) * p(i)) + (x(i) * r(i));
    end
end

% compute record and pre level curves for a single fade segment
% given a  range of fade values and the "base" rec and pre levels.
function [rec, pre] = fade_curve_naive(f, r0, p0)
    % linear x-fade:
    % record level should simply be scaled by the fade position..
    rec = f * r0;
    % record level should fall from 1 to the base level..
    pre = p0 + (1-f)*(1-p0);
    % ... but this causes recorded material to decay too slowly during the
    % fade.
end

% better analysis: for each position in the crossfade, we are effectively
% applying a different recursive filter to the signal. the difference
% equation is y(n) = r*x(n) + p*y(n-1). so this is a 1-pole lowpass.
% for a smooth decay, we should attempt to make each filter have a similar
% magnitude/impulse response. 
function g = mag_response(w, r, p)
    b = r;
    a = -1*p;
    g = b / (1 + a*a - 2*a*cos(w))^0.5;
end

function y = rising_raised_halfcos(f)
    y = cos((f+2)*pi/2);
end

function y = falling_raised_cos(f)
    y = (cos(f*pi)+1) * 0.5;
end

% helper: invert fade position and scale to [1, pre]
function y = scale_pre(f, p0);
    y = p0 + (1-f) * (1-p0);
end

% so we should be able to get a better curve by modulating the gain factor
% (rec level) to compensate

% although the impulse/step response is simple, it's not clear that there
% is a general way to equalize the output for all inputs, given that we must increase the
% feedback term to 1.0 at the endpoint. 

% some variations...
% ...
  

% with a straight linear recfade curve, 
% applying a sharper raised-cos envelope to the pre-level gives us a dip...
function [rec, pre] = fade_curve_better(f, r0, p0)
    pre = p0 + rising_raised_halfcos(f) * ( 1-p0);
    rec = f * r0;
end

% but we may want a sinusoidal rec taper; 
% then, squaring the pre curve looks _almost_ ok...
function [rec, pre] = fade_curve_better_2(f, r0, p0)
    pre = scale_pre(f .^ 2, p0);
    rec = (1-falling_raised_cos(f)) * r0;
end

% but we may want a sinusoidal rec taper; 
% then, squaring the pre curve looks _almost_ ok...
function [rec, pre] = fade_curve_exp_sine(f, r0, p0)
    pre = scale_pre(f.^0.1, p0);
    rec = (1-falling_raised_cos(f)) * r0;
end


% so.. the "real" answer would i guess be to scale the gain factor by the
% inverse of the response of the filter produced by pre-level.
% (this seems maybe impossible without knowing about the input signal,
% right?)
% let's test it with the unit input and the impulse response, which we do
% know?

function [rec, pre] = fade_curve_hm(f, r0, p0)
    pre = scale_pre(f, p0);
    %{
    step= r0 * ((1-p0^2)/(1-p0));
    rec = zeros(1, length(pre));
    for i=1:length(pre)
        rec(i) = step * (1-pre(i)) / (1-pre(i)^2);
    end
    %}
    % ohho, i'm dumb. of course! the rec level will be used twice in xfade,
    % b/c in reality there are two heads passing over the fade section
    rec = (max(0, log(f) + 1));
end

% convenience wrapper
function [rec, pre] = fade_curve(f, r0, p0)
   [rec, pre] = fade_curve_exp_sine(f, r0, p0);
end 

function [rec, pre] = make_curves(n, nf, r0, p0)
    rec = zeros(1, n);
    pre = zeros(1, n);
    % fade-in
    f = linspace(0, 1, nf);
    [r, p] = fade_curve(f, r0, p0);
    rec(1:nf) = r;
    pre(1:nf) = p;
    % sustain
    rec(nf+1:n-nf) = r0;
    pre(nf+1:n-nf) = p0;
    %fade-out
    f = linspace(1, 0, nf);
    [r, p] = fade_curve(f, r0, p0);
    rec(n-nf+1:n) = r;
    pre(n-nf+1:n) = p;
end