#ifdef DCMIMAGE_EXPORTS
#define DCMIMAGE_API __declspec(dllexport)
#else
#define DCMIMAGE_API __declspec(dllimport)
#endif

#include "gspsprocessor.h"
#include "imgprocessor.h"
#include "pixelmeasurer.h"