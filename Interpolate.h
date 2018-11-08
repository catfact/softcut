//
// Created by ezra on 12/8/17.
//

#ifndef SOFTCUT_INTERP_H
#define SOFTCUT_INTERP_H

inline double cubicinterp(double x, double y0, double y1, double y2, double y3)
{
    // 4-point, 3rd-order Hermite (x-form)
    double c0 = y1;
    double c1 = 0.5 * (y2 - y0);
    double c2 = y0 - 2.5 * y1 + 2. * y2 - 0.5 * y3;
    double c3 = 0.5 * (y3 - y0) + 1.5 * (y1 - y2);

    return ((c3 * x + c2) * x + c1) * x + c0;
}

#endif //SOFTCUT_INTERP_H
