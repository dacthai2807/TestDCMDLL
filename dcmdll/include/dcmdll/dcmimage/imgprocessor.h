#ifndef IMGPROCESSOR
#define IMGPROCESSOR

enum ROTATE_TYPE {
	ROTATE_LEFT,
	ROTATE_RIGHT,
	ROTATE_TOP_DOWN
};

enum FLIP_TYPE {
	FLIP_HORIZONTALLY,
	FLIP_VERTICALLY,
	FLIP_BOTH_AXES
};

enum SCALE_TYPE {
	SCALE_X,
	SCALE_Y
};

enum VOI_LUT_FUNCTION {
	LINEAR,
	SIGMOID,
};


extern "C" DCMIMAGE_API bool rotateImage(const char* inputFilename, const char* outputFilename, const ROTATE_TYPE type);

extern "C" DCMIMAGE_API bool flipImage(const char* inputFilename, const char* outputFilename, const FLIP_TYPE type);

extern "C" DCMIMAGE_API bool scaleImageByFactor(const char* inputFilename, const char* outputFilename, const SCALE_TYPE type, const float factor);

extern "C" DCMIMAGE_API bool clipAndScaleImageByFactor(const char* inputFilename, const char* outputFilename, const int left, const int top, const int width, const int height, const SCALE_TYPE type, const float factor);

extern "C" DCMIMAGE_API bool changeContrastOfImage(const char* inputFilename, const char* outputFilename);

extern "C" DCMIMAGE_API bool applyExistingVOIWindows(const char* inputFilename, const char* outputFilename, const signed int numOfWindows, const VOI_LUT_FUNCTION voiLutFunc = LINEAR);

extern "C" DCMIMAGE_API bool applyMinMaxVOIWindows(const char* inputFilename, const char* outputFilename, const VOI_LUT_FUNCTION voiLutFunc = LINEAR);

extern "C" DCMIMAGE_API bool applyHistogramVOIWindows(const char* inputFilename, const char* outputFilename, const signed int ignoringPercent, const VOI_LUT_FUNCTION voiLutFunc = LINEAR);

extern "C" DCMIMAGE_API bool applySettingVOIWindows(const char* inputFilename, const char* outputFilename, const float center, const float width, const VOI_LUT_FUNCTION voiLutFunc = LINEAR);

#endif
