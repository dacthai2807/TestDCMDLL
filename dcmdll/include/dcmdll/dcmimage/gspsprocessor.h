#ifndef GSPSPROCESSOR
#define GSPSPROCESSOR

enum GSPS_ROTATE_TYPE {
	GSPS_ROTATE_LEFT,
	GSPS_ROTATE_RIGHT,
	GSPS_ROTATE_TOP_DOWN
};

extern "C" DCMIMAGE_API bool createGSPS(const char* dcmFilename, const char* gspsFilename);

extern "C" DCMIMAGE_API bool addTextToGSPS(const char* dcmFilename, const char* gspsFilename, const char* layerName, const float anchorPointX, const float anchorPointY, const char* text);

extern "C" DCMIMAGE_API bool removeLastTextFromGSPS(const char* dcmFilename, const char* gspsFilename, const char* layerName);

extern "C" DCMIMAGE_API float addLineToGSPS(const char* dcmFilename, const char* gspsFilename, const char* layerName, const float pointX1, const float pointY1, const float pointX2, const float pointY2);

extern "C" DCMIMAGE_API bool removeLastLineFromGSPS(const char* dcmFilename, const char* gspsFilename, const char* layerName);

extern "C" DCMIMAGE_API bool setRotationToGSPS(const char* dcmFilename, const char* gspsFilename, const GSPS_ROTATE_TYPE type);

extern "C" DCMIMAGE_API bool setFlipToGSPS(const char* dcmFilename, const char* gspsFilename);


#endif
