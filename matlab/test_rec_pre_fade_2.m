
% continued...
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

% when looping repeatedly over a single region, it's best to keep pre-level
% constant.
% however, this will create clicks when moving the region.
% so make a very small transition around the actual loop point.
% record level should be 1
function [rec, pre] = fade_curve_tables(f, r0, p0)
    % rec table: shortened sinusoid
    n = 512; % table size
    ap = 64; % number of samples in pre curve
    ar = 4; % number of samples _not_ in rec curve
    bp = n-ap; % number of samples _not_ in pre curve
    br = n-ar; % number of samples in rec curve
    
    % pre table
    ptab = ones(1, n) * p0;
    rtab = zeros(1, n);
    % build rec curve
    brx = linspace(0, 1, br);
    ry = cos((brx+1)*pi)*0.5 + 0.5;
    % hm.. 
    % ry = brx;
    
    rtab(ar+1:n) = ry * r0;
    % build pre curve
    apx = linspace(0, 1, ap);
    py = (cos(apx*pi) * 0.5 + 0.5) * (1-p0) + p0;
    ptab(1:ap) = py;
    
    % apply the tables
    rec = interp1(1:n, rtab, f * (n-1) + 1);
    pre = interp1(1:n, ptab, f * (n-1) + 1);
    "done with curve tables"
end

% convenience wrapper
function [rec, pre] = fade_curve(f, r0, p0)
   [rec, pre] = fade_curve_tables(f, r0, p0);
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