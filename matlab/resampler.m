classdef resampler < handle
    %RESAMPLER
    properties
        n       % size of input buffer (limits interpolation order)
        m       % size of output buffer (limits upsampling ratio)
        in    % input ringbuffer
        out    % output ringbuffer
        phi     % current output phase
        r       % resampling ratio
    end
    
    methods
        function rs = resampler()
            rs.n = 4;
            rs.m = 16;
            rs.in = zeros(1, rs.n);
            rs.out = zeros(1, rs.m);
            rs.phi = 0;
        end
        
        function push(rs,x)
            % PUSH write sample to input ringbuffer, drop oldest value
            b = [rs.in];
            b(1) = x;
            [rs.in] = circshift(b, -1);
        end
        
        % process an input sample
        % returns number of new output frames
        function nframes = process(rs, x)
            rs.push(x)
            if rs.r > 1
                nframes = rs.writeUp();
            else
                nframes = rs.writeDown();
            end
        end
        
        % compute output with rate > 1 (upsampling)
        % assumption: input has just been pushed
        % returns: number of output frames produced
        function nframes = writeUp(rs)
            % store the old phase
            % this is a remainder of %1 operation
            rem = rs.phi;
            p = rem + rs.r;
            nf = floor(p);
            nframes = nf;
            % we can assume that n >= 1 here
            % we want to track fractional output phase for interpolation
            % this is normalized to the distance between input frames
            % so: the distance to the first output frame boundary:
            f = 1.0 - rem;
            % normalized:
            f = f / rs.r;
            % write the first output frame
            i = 1;
            rs.writeOutInterp(i, f)
            nf = nf - 1;
            % then the distance between consecutive output frames is 1/R
            % (interpolation "slope")
            df = 1 / rs.r;
            while nf > 0
                i = i + 1;
                f = f + df;
                rs.writeOutInterp(i, f);
                nf = nf - 1;
            end
            % store the remainder of the updated, un-normalized output
            % phase
            rs.phi = mod(p, 1.0);
        end
        
        % compute output with rate < 1 (downsampling)
        % assumption: input has just been pushed
        function nframes = writeDown(rs)
            
        end
        
        % store an output frame with interpolation
        % assumptions: input has just been pushed. f is in [0, 1]
        function writeOutInterp(rs, i, f) 
            % first try: linear interpolation
            % just take the last two input values
            x0 = rs.in(rs.n-1);
            x1 = rs.in(rs.n);
            y = x0 + (x1-x0)*f;
            rs.out(1, i) = y;
        end
    end
end


