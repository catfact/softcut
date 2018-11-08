% 3rd-order hermite interpolation (x-form)
function y = hermite (x, y0, y1, y2, y3)
    c0 = y1;
    c1 = 0.5 * (y2 - y0);
    c2 = y0 - 2.5*y1 + 2*y2 - 0.5*y3;
    c3 = 0.5*(y3-y0) + 1.5*(y1-y2);
    y = ((c3*x + c2)*x + c1)*x + c0;
end