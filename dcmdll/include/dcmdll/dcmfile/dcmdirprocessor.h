#ifndef DCMDIRPROCESSOR
#define DCMDIRPROCESSOR

#include "dcmfile.h"

extern "C" DCMFILE_API bool createDcmDir(const int numOfDcmFile, const char* dcmFilenames[]);

#endif