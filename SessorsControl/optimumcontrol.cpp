#include "optimumcontrol.h"

double WArray[2048] = {0};
double DArray[2048] = {0};

OptimumControl::OptimumControl()
{

}

//char SerialNumber[12];
//void ShowDeviceInfo();

int OptimumControl::Init(double* WArray, int* dataCount)
{
    int SN[128*11] = {0};
    double MinW, MaxW;

    // SN
    //   Please pass in a double array of size 127x11.
    // return
    //   0 : No error.
    //   5 : Serial Number error.
    //   others : connection error.

    int initState = Ini_WinUSBDevice(0, SN);
    if (initState != 0)
    {
        return -1;
    }

    // WArray
    //   Please pass in a double array of size 2048.
    int initState2 = TrsPWDLL(WArray, dataCount, &MinW, &MaxW);

    if (initState2 != 0)
    {
      //  printf("[Erro] Init failed!\r\n");
        return -2;
    }

   int initState3 = SetAutoITLimit(8000);
   if (initState3 != 0)
   {
     //  printf("[Erro] Init failed!\r\n");
       return -3;
   }

    return 0;
}

int OptimumControl::Close()
{
    int ret = Close_WinUSBDevice();
    return  ret;
}


int OptimumControl::Measure(int time/*, double* WArray, double* DArray, int dataLenght*/)
{
 //   int hasData = 0;
    int LS, Peak;
  //  double DArray;
    // nIT (ms)
    //   0   : Automatic decision time.
    //   2~n : 2 ~ n exposure time.
    int triggerState = SDUSBTrigger(time, DArray, &LS, &Peak);

    if (triggerState != 0)
    {
      //  printf("[Erro] SDUSBTrigger failed!\r\n");
        return -2;
    }

    int useTime = time;
    if (time == 0)
    {
        GetAutoITInformation(&useTime, &Peak);
    }


    return 0;
}

int OptimumControl::GetLxAndGLx(double &Lx,double &GLx)
{
    int ret = 0;
    ret = GetCIE1931Illuminance(&Lx);
    if(ret !=0)
    {
        return -1;
    }

    GetGIlluminance(&GLx);
    if(ret !=0)
    {
        return -1;
    }

    Lx = Lx/1000.0;
    GLx = GLx/1000.0;

    return 0;
}
