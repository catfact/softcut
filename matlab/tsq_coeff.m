function [a, b] = tsq_coeff(g, t)
%TSQ_COEFF calculate coefficients for 2-stage quadratic softclip
%   g: linear gain
%   t: linear softclip threshold
    t_1 = t - 1.0;
    if t_1 < 0
        a = g / (2*t_1);
        b = g * t - (a * t_1 * t_1);
    else
        a = 0;
        b = 1;
    end
end

