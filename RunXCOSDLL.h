
#ifdef OUTDLL
    #define DLL_API __declspec(dllexport)
#else
    #define DLL_API __declspec(dllimport) 
#endif


DLL_API int init(double *input_data,int input_size,double *output_data,int output_size);
DLL_API void operate(void);
