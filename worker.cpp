#include "worker.h"

void Worker::operator()(MandelData& data)
{
        if (data.i != -1)
                return;

        auto& c = data.c;
        auto& z = data.z;

        for (int i = iterationsBegin; i < iterationsEnd; i++) {
                z = pow(z, exponent) + c;

                if (abs(z) >= 4) {
                        data.i = i;
                        break;
                }
        }
}
