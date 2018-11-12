% problem: 
% when crossfading, we need to correctly modulate the record and pre-record
% levels as well as the playback level.

% requirement:
% at the end of the xfade, should have rec == 0, pre == 1.

% requirement: 
% there should never be perceived volume drops or volume spikes after
% repeated passes over the same material

% requirement:
% rec/pre curves should be "smooth," preferably with continuous derivative.

% we'll simulate the effects of repeatedly looping over the same part of a
% buffer with record and crossfade. 
% in the SoftCut structure, there would be 2 alternating heads doing this,
% and also playing back recorded sample values. 
% for the purpose of focusing on recording effects, a single head is
% equivalent; we can simply observe the buffer in the loop region with
% repeated passes.

% count of samples in each fade section (in and out)
nf = 2000;
% count of samples to hold without fade
nh = 2000;

n = nf*2 + nh;
% initial input signal (white noise)
in = rand(1, n);

% count of iterations
count = 30;

buf = zeros(1, n);

% base rec/pre levels (before fade)
rec0 = 1;
pre0 = 0.7;

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
z = zeros(1, n);
for i=1:count
    h = [h; buf];
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
function [rec, pre] = segment(f, r0, p0)
    % linear x-fade:
    % record level should simply be scaled by the fade position..
    rec = f * r0;
    % record level should fall from 1 to the base level..
    pre = p0 + (1-f)*(1-p0);
    % ... but this causes recorded material to decay too slowly during the
    % fade. so, pre should not be allowed to exceed inverse of rec?
    % pre = min(pre, 1-rec);
end

function [rec, pre] = make_curves(n, nf, r0, p0)
    rec = zeros(1, n);
    pre = zeros(1, n);
    % fade-in
    f = linspace(0, 1, nf);
    [r, p] = segment(f, r0, p0);
    rec(1:nf) = r;
    pre(1:nf) = p;
    % sustain
    rec(nf+1:n-nf) = r0;
    pre(nf+1:n-nf) = p0;
    %fade-out
    f = linspace(1, 0, nf);
    [r, p] = segment(f, r0, p0);
    rec(n-nf+1:n) = r;
    pre(n-nf+1:n) = p;
end