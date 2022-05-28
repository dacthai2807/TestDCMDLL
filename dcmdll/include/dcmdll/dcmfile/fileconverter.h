#ifndef FILECONVERTER
#define FILECONVERTER

extern "C" DCMFILE_API bool convertDcmToBMP(const char* dcmFilename, const char* bmpFilename);

extern "C" DCMFILE_API bool convertDcmToJPG(const char* dcmFilename, const char* jpgFilename);

extern "C" DCMFILE_API bool convertDcmToPNG(const char* dcmFilename, const char* pngFilename);

extern "C" DCMFILE_API bool convertDcmToBMPWC(const wchar_t* dcmFilename, const char* bmpFilename);

extern "C" DCMFILE_API bool convertDcmToJPGWC(const wchar_t* dcmFilename, const char* jpgFilename);

extern "C" DCMFILE_API bool convertDcmToPNGWC(const wchar_t* dcmFilename, const char* pngFilename);

#endif
