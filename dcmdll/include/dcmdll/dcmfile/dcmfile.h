#ifdef DCMFILE_EXPORTS
#define DCMFILE_API __declspec(dllexport)
#else
#define DCMFILE_API __declspec(dllimport)
#endif

#include "fileconverter.h"
#include "dcmdirprocessor.h"