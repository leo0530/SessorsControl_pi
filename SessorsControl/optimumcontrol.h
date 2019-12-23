#ifndef OPTIMUMCONTROL_H
#define OPTIMUMCONTROL_H
#include "libLinuxSDUSB.h"

class OptimumControl
{
public:
    OptimumControl();

    int Init(double* WArray, int* dataCount);
    int Close();
    int Measure(int time/*, double* WArray, double* DArray, int dataLenght*/);
    int GetLxAndGLx(double &Lx,double &GLx);


};

#endif // OPTIMUMCONTROL_H
