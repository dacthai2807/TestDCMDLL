#include "pch.h"
#include "dcmimage.h"

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmpstat/dcmpstat.h"

using std::string;

float measureLengthOfLine(const char* dcmFilename, const float pointX1, const float pointY1, const float pointX2, const float pointY2) {

	DcmFileFormat dfile;

	if (dfile.loadFile(dcmFilename).bad()) return -1;

	DcmDataset* dataset = dfile.getDataset();

	/* make sure that pixel data is loaded before output file is created */
	dataset->loadAllDataIntoMemory();

	OFString rowspacing_str, colspacing_str;

	float rowspacing, colspacing;

	if (dataset->findAndGetOFString(DCM_PixelSpacing, rowspacing_str, 0).good() &&
		dataset->findAndGetOFString(DCM_PixelSpacing, colspacing_str, 1).good()) {
		rowspacing = static_cast<float>(stof(string(rowspacing_str.c_str())));
		colspacing = static_cast<float>(stof(string(colspacing_str.c_str())));
	}
	else return -1;

	return sqrt((pointX2 - pointX1) * rowspacing * (pointX2 - pointX1) * rowspacing + (pointY2 - pointY1) * colspacing * (pointY2 - pointY1) * colspacing);
}

float measureAngle(const float pointX1, const float pointY1, const float pointX2, const float pointY2, const float pointX3, const float pointY3, const float pointX4, const float pointY4) {
	float d1x = pointX2 - pointX1, d1y = pointY2 - pointY1;
	float d2x = pointX4 - pointX3, d2y = pointY4 - pointY3;

	return acos((d1x * d2x + d1y * d2y) / (sqrt(d1x * d1x + d1y * d1y) * sqrt(d2x * d2x + d2y * d2y))) * 360 / (2 * acos(-1));
}