#ifndef __LINUX_SDUSB_H__
#define __LINUX_SDUSB_H__

#ifdef OPTiMUM_EXPORTS
#define OPTiMUM_API __attribute__ ((visibility ("default")))
#else
#define OPTiMUM_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

    OPTiMUM_API int Ini_WinUSBDevice(int DeviceNumber, int* CpSN);
    OPTiMUM_API int Close_WinUSBDevice();

    OPTiMUM_API int SetAutoITLimit(int nIT);
    OPTiMUM_API int TrsPWDLL(double* WArray1, int* ArraySize, double* MinW, double* MaxW);

    OPTiMUM_API int GetBias(double* result);
    OPTiMUM_API int SetBiasData(int nIT, double* Data);

    OPTiMUM_API int SDUSBTrigger(int nIT, double* pDArray, int* pLS, int* pPeak);
    
    OPTiMUM_API int GetCIE1931Yxy(double* pY, double* px, double* py);
    OPTiMUM_API int GetCIE1931PeakWavelength(double* pPWLV, double* pPWLIR);
    OPTiMUM_API int GetCIE1931LeltaLanda(double* pLL);
    OPTiMUM_API int GetCIE1931DominateWavelength(double* pDWL);
    OPTiMUM_API int GetCIE1931Purity(double* pPy);
    OPTiMUM_API int GetCIE1931CentroidWavelength(int nMode, double* pLC);
    OPTiMUM_API int GetCIE1931CorrelatedColorTemperature(double* pCCT);
    OPTiMUM_API int GetCIE1931ColorRenderingIndex(double* pRn, double* pRa);
    OPTiMUM_API int GetCIE1931Illuminance(double* pLx);
    OPTiMUM_API int GetGIlluminance(double* pGLx);
    OPTiMUM_API int GetWatt(double* pW);
    OPTiMUM_API int GetAutoITInformation(int* pCurrentIT, int* pCurrentPeak);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __LINUX_SDUSB_H__
