//
// Created by ezra on 12/8/17.
//

#ifndef SOFTCUT_INTERPOLATE_H
#define SOFTCUT_INTERPOLATE_H

namespace softcut {
    class Interpolate {
    public:
        template<typename T>
        static inline T hermite(T x, T y0, T y1, T y2, T y3) {
            // 4-point, 3rd-order Hermite (x-form)
            T c0 = y1;
            T c1 = 0.5 * (y2 - y0);
            T c2 = y0 - 2.5 * y1 + 2. * y2 - 0.5 * y3;
            T c3 = 0.5 * (y3 - y0) + 1.5 * (y1 - y2);
            return ((c3 * x + c2) * x + c1) * x + c0;
        }

    };
}

#endif //SOFTCUT_INTERPOLATE_H
