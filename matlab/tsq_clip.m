function y = tsq_clip(x, t, g, a, b)
% TSQ_CLIP apply 2-stage quadratic softclipping to input sample
    % x : input
    % t: threshold (beginning of knee)
    % g: gain multiplier
    % a: parabolic coefficient
    % b: parabolic offset ( = max level)
   
    ax = abs(x);
    sx = sign(x);
    % hard clip at unity
    if ax > 1
        ax = 1;
    end
    
    if ax < t 
        y = x * g;
    else
        q = ax - 1; 
        y = sx * (a*q*q + b);
    end
end
    