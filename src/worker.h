#ifndef WORKER_H
#define WORKER_H

#include <QtCore>
#include <complex>
#include <cmath>

using Complex = std::complex<qreal>;

struct MandelData {
        /*    c = (x, y)     complex plane coordinates
         *    z = z^n + c    mandelbrot data to iterate over (see wiki or whatever)
         *    i              number of iterations before divergence (for assigning color later)
         */
        Complex c;
        Complex z;
        int     i;
};

struct Worker
{
        qreal exponent = 2;
        int   iterationsBegin = 0;
        int   iterationsEnd = 0;
        void  operator()(MandelData& data);
};

#endif // WORKER_H
