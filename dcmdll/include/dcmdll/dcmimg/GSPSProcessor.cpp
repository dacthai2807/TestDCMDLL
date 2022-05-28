#include "pch.h"
#include "dcmimg.h"

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmpstat/dcmpstat.h"
#include "dcmtk/dcmpstat/dvpstat.h"
#include "dcmtk/dcmpstat/dvpsgr.h" 
#include "dcmtk/dcmpstat/dvpstx.h"

using namespace std;

bool createGSPS(const char* dcmFilename, const char* gspsFilename) {

	DcmFileFormat dfile;
	OFCondition cond = dfile.loadFile(dcmFilename);

	if (cond.bad()) return false;

	DcmDataset* dataset = dfile.getDataset();

	/* make sure that pixel data is loaded before output file is created */
	dataset->loadAllDataIntoMemory();

	DVPresentationState pstate;

	if (pstate.createFromImage(*dataset).bad()) return false;

	if (pstate.attachImage(dataset, OFFalse).bad()) return false;

	DcmFileFormat gspsfile;

	if (pstate.write(*gspsfile.getDataset(), OFTrue).bad()) return false;

	if (gspsfile.saveFile(gspsFilename, EXS_LittleEndianExplicit).bad()) return false;

	return true;
}

float addLineToGSPS(const char* dcmFilename, const char* gspsFilename, const char* layerName, const float pointX1, const float pointY1, const float pointX2, const float pointY2) {

	DcmFileFormat dfile;
	DcmFileFormat gspsfile;

	if (dfile.loadFile(dcmFilename).bad()) return -1;

	if (gspsfile.loadFile(gspsFilename).bad()) return -1;

	DVPresentationState pstate;

	if (pstate.read(*gspsfile.getDataset()).bad()) return -1;

	if (pstate.attachImage(&dfile, OFFalse).bad()) return -1;

	/* add graphical annotation, create a new layer first ...  */
	pstate.addGraphicLayer(layerName);

	/* ... and then a graphic object */
	DVPSGraphicObject* graphic = pstate.addGraphicObject(pstate.getGraphicLayerIndex(layerName));
	if (graphic == NULL) return -1;

	const Float32 data[] = { pointX1, pointY1, pointX2, pointY2 };
	graphic->setGraphicType(DVPST_polyline);
	graphic->setData(2, data, DVPSA_pixels);

	if (pstate.write(*gspsfile.getDataset(), OFTrue).bad()) return -1;

	if (gspsfile.saveFile(gspsFilename, EXS_LittleEndianExplicit).bad()) return -1;

	pstate.detachImage();

	return measureLengthOfLine(dcmFilename, pointX1, pointY1, pointX2, pointY2);
}

bool addTextToGSPS(const char* dcmFilename, const char* gspsFilename, const char* layerName, const float anchorPointX, const float anchorPointY, const char* text) {
	DcmFileFormat dfile;
	DcmFileFormat gspsfile;

	if (dfile.loadFile(dcmFilename).bad()) return false;

	if (gspsfile.loadFile(gspsFilename).bad()) return false;

	DVPresentationState pstate;

	if (pstate.read(*gspsfile.getDataset()).bad()) return false;

	if (pstate.attachImage(&dfile, OFFalse).bad()) return false;

	/* add graphical annotation, create a new layer first ...  */
	pstate.addGraphicLayer(layerName);

	/* ... and then a graphic object */
	DVPSTextObject* textObj = pstate.addTextObject(pstate.getGraphicLayerIndex(layerName));
	if (textObj == NULL) return -1;

	textObj->setText(text);
	textObj->setAnchorPoint(anchorPointX, anchorPointY, DVPSA_pixels, OFFalse);

	if (pstate.write(*gspsfile.getDataset(), OFTrue).bad()) return false;

	if (gspsfile.saveFile(gspsFilename, EXS_LittleEndianExplicit).bad()) return false;

	pstate.detachImage();

	return true;
}

bool setRotationToGSPS(const char* dcmFilename, const char* gspsFilename, const GSPS_ROTATE_TYPE type) {
	DcmFileFormat dfile;
	DcmFileFormat gspsfile;

	if (dfile.loadFile(dcmFilename).bad()) return false;

	if (gspsfile.loadFile(gspsFilename).bad()) return false;

	DVPresentationState pstate;

	if (pstate.read(*gspsfile.getDataset()).bad()) return false;

	if (pstate.attachImage(&dfile, OFFalse).bad()) return false;

	switch (type) {
	case GSPS_ROTATE_LEFT:
		if (pstate.setRotation(DVPSR_270_deg).bad()) return false;
		break;
	case GSPS_ROTATE_RIGHT:
		if (pstate.setRotation(DVPSR_90_deg).bad()) return false;
		break;
	case GSPS_ROTATE_TOP_DOWN:
		if (pstate.setRotation(DVPSR_180_deg).bad()) return false;
		break;
	default:
		if (pstate.setRotation(DVPSR_0_deg).bad()) return false;
		break;
	}

	if (pstate.write(*gspsfile.getDataset(), OFTrue).bad()) return false;

	if (gspsfile.saveFile(gspsFilename, EXS_LittleEndianExplicit).bad()) return false;

	pstate.detachImage();

	return true;
}

bool setFlipToGSPS(const char* dcmFilename, const char* gspsFilename) {
	DcmFileFormat dfile;
	DcmFileFormat gspsfile;

	if (dfile.loadFile(dcmFilename).bad()) return false;

	if (gspsfile.loadFile(gspsFilename).bad()) return false;

	DVPresentationState pstate;

	if (pstate.read(*gspsfile.getDataset()).bad()) return false;

	if (pstate.attachImage(&dfile, OFFalse).bad()) return false;

	if (pstate.setFlip(true).bad()) return false;

	if (pstate.write(*gspsfile.getDataset(), OFTrue).bad()) return false;

	if (gspsfile.saveFile(gspsFilename, EXS_LittleEndianExplicit).bad()) return false;

	pstate.detachImage();

	return true;
}

bool removeLastTextFromGSPS(const char* dcmFilename, const char* gspsFilename, const char* layerName) {
	DcmFileFormat dfile;
	DcmFileFormat gspsfile;

	if (dfile.loadFile(dcmFilename).bad()) return false;

	if (gspsfile.loadFile(gspsFilename).bad()) return false;

	DVPresentationState pstate;

	if (pstate.read(*gspsfile.getDataset()).bad()) return false;

	if (pstate.attachImage(&dfile, OFFalse).bad()) return false;
	
	size_t numOfLayers = pstate.getNumberOfGraphicLayers();

	for (size_t layerId = 0; layerId < numOfLayers; ++layerId) {
		const char* curLayerName = pstate.getGraphicLayerName(layerId);

		if (strcmp(curLayerName, layerName) == 0) {
			size_t lastTextId = pstate.getNumberOfTextObjects(layerId) - 1;

			if (pstate.removeTextObject(layerId, lastTextId).bad()) return false;

			if (pstate.write(*gspsfile.getDataset(), OFTrue).bad()) return false;

			if (gspsfile.saveFile(gspsFilename, EXS_LittleEndianExplicit).bad()) return false;

			pstate.detachImage();

			return true;
		}
	}

	pstate.detachImage();

	return false;
}

bool removeLastLineFromGSPS(const char* dcmFilename, const char* gspsFilename, const char* layerName) {
	DcmFileFormat dfile;
	DcmFileFormat gspsfile;

	if (dfile.loadFile(dcmFilename).bad()) return false;

	if (gspsfile.loadFile(gspsFilename).bad()) return false;

	DVPresentationState pstate;

	if (pstate.read(*gspsfile.getDataset()).bad()) return false;

	if (pstate.attachImage(&dfile, OFFalse).bad()) return false;

	size_t numOfLayers = pstate.getNumberOfGraphicLayers();

	for (size_t layerId = 0; layerId < numOfLayers; ++layerId) {
		const char* curLayerName = pstate.getGraphicLayerName(layerId);

		if (strcmp(curLayerName, layerName) == 0) {
			size_t lastGraphicId = pstate.getNumberOfGraphicObjects(layerId) - 1;

			if (pstate.removeGraphicObject(layerId, lastGraphicId).bad()) return false;

			if (pstate.write(*gspsfile.getDataset(), OFTrue).bad()) return false;

			if (gspsfile.saveFile(gspsFilename, EXS_LittleEndianExplicit).bad()) return false;

			pstate.detachImage();

			return true;
		}
	}

	pstate.detachImage();

	return false;
}