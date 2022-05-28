#ifndef PIXELMEASURER
#define PIXELMEASURER

extern "C" DCMIMAGE_API float measureLengthOfLine(const char* dcmFilename, const float pointX1, const float pointY1, const float pointX2, const float pointY2);

extern "C" DCMIMAGE_API float measureAngle(const float pointX1, const float pointY1, const float pointX2, const float pointY2, const float pointX3, const float pointY3, const float pointX4, const float pointY4);

#endif
