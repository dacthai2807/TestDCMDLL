#include "pch.h"
#include "dcmimg.h"

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmdata/dcrledrg.h" 
#include "dcmtk/dcmdata/dctk.h"


bool rotateImage(const char* inputFilename, const char* outputFilename, const ROTATE_TYPE type) {

	DcmFileFormat dfile;
	OFCondition cond = dfile.loadFile(inputFilename);

	if (cond.bad()) return false;

	DcmDataset* dataset = dfile.getDataset();

	/* make sure that pixel data is loaded before output file is created */
	dataset->loadAllDataIntoMemory();

	E_TransferSyntax xfer = dataset->getOriginalXfer();

	DicomImage* di = new DicomImage(dataset, xfer);

	if (di == NULL) return false;

	if (di->getStatus() != EIS_Normal) return false;

	int rotateDegree;

	switch (type) {
	case ROTATE_LEFT:
		rotateDegree = 270; break;
	case ROTATE_RIGHT:
		rotateDegree = 90; break;
	case ROTATE_TOP_DOWN:
		rotateDegree = 180; break;
	default:
		break;
	}

	if (!di->rotateImage(rotateDegree)) return false;

	if (!di->writeImageToDataset(*dataset)) return false;

	// update Image Type
	OFString imageType = "DERIVED";
	const char* oldImageType = NULL;
	if (dataset->findAndGetString(DCM_ImageType, oldImageType).good())
	{
		if (oldImageType != NULL)
		{
			// append old image type information beginning with second entry
			const char* pos = strchr(oldImageType, '\\');
			if (pos != NULL)
				imageType += pos;
		}
	}
	dataset->putAndInsertString(DCM_ImageType, imageType.c_str());

	// update SOP Instance UID
	// add reference to source image
	DcmItem* ditem = NULL;
	const char* sopClassUID = NULL;
	const char* sopInstanceUID = NULL;
	if (dataset->findAndGetString(DCM_SOPClassUID, sopClassUID).good() &&
		dataset->findAndGetString(DCM_SOPInstanceUID, sopInstanceUID).good())
	{
		dataset->findAndDeleteElement(DCM_SourceImageSequence);
		if (dataset->findOrCreateSequenceItem(DCM_SourceImageSequence, ditem).good())
		{
			ditem->putAndInsertString(DCM_SOPClassUID, sopClassUID);
			ditem->putAndInsertString(DCM_SOPInstanceUID, sopInstanceUID);
		}
	}
	// create new SOP instance UID
	char new_uid[100];
	dataset->putAndInsertString(DCM_SOPInstanceUID, dcmGenerateUniqueIdentifier(new_uid));

	// ======================================================================
	// write back output file
	dfile.saveFile(outputFilename, xfer, EET_ExplicitLength, EGL_recalcGL, EPD_noChange, OFstatic_cast(Uint32, 0), OFstatic_cast(Uint32, 0), EWM_fileformat);

	// done, now cleanup. 
	delete di;

	return true;
}

bool flipImage(const char* inputFilename, const char* outputFilename, const FLIP_TYPE type) {

	DcmFileFormat dfile;
	OFCondition cond = dfile.loadFile(inputFilename);

	if (cond.bad()) return false;

	DcmDataset* dataset = dfile.getDataset();

	/* make sure that pixel data is loaded before output file is created */
	dataset->loadAllDataIntoMemory();

	E_TransferSyntax xfer = dataset->getOriginalXfer();

	DicomImage* di = new DicomImage(dataset, xfer);

	if (di == NULL) return false;

	if (di->getStatus() != EIS_Normal) return false;

	switch (type) {
	case FLIP_HORIZONTALLY:
		if (!di->flipImage(1, 0)) return false; break;
	case FLIP_VERTICALLY:
		if (!di->flipImage(0, 1)) return false; break;
	case FLIP_BOTH_AXES:
		if (!di->flipImage(1, 1)) return false; break;
	default:
		break;
	}

	if (!di->writeImageToDataset(*dataset)) return false;

	// update Image Type
	OFString imageType = "DERIVED";
	const char* oldImageType = NULL;
	if (dataset->findAndGetString(DCM_ImageType, oldImageType).good()) {
		if (oldImageType != NULL) {
			// append old image type information beginning with second entry
			const char* pos = strchr(oldImageType, '\\');
			if (pos != NULL)
				imageType += pos;
		}
	}
	dataset->putAndInsertString(DCM_ImageType, imageType.c_str());

	// update SOP Instance UID
	// add reference to source image
	DcmItem* ditem = NULL;
	const char* sopClassUID = NULL;
	const char* sopInstanceUID = NULL;
	if (dataset->findAndGetString(DCM_SOPClassUID, sopClassUID).good() &&
		dataset->findAndGetString(DCM_SOPInstanceUID, sopInstanceUID).good())
	{
		dataset->findAndDeleteElement(DCM_SourceImageSequence);
		if (dataset->findOrCreateSequenceItem(DCM_SourceImageSequence, ditem).good())
		{
			ditem->putAndInsertString(DCM_SOPClassUID, sopClassUID);
			ditem->putAndInsertString(DCM_SOPInstanceUID, sopInstanceUID);
		}
	}
	// create new SOP instance UID
	char new_uid[100];
	dataset->putAndInsertString(DCM_SOPInstanceUID, dcmGenerateUniqueIdentifier(new_uid));

	// ======================================================================
	// write back output file
	dfile.saveFile(outputFilename, xfer, EET_ExplicitLength, EGL_recalcGL, EPD_noChange, OFstatic_cast(Uint32, 0), OFstatic_cast(Uint32, 0), EWM_fileformat);

	// done, now cleanup. 
	delete di;

	return true;
}

bool scaleImageByFactor(const char* inputFilename, const char* outputFilename, const SCALE_TYPE type, const float factor) {

	DcmFileFormat dfile;
	OFCondition cond = dfile.loadFile(inputFilename);

	if (cond.bad()) return false;

	DcmDataset* dataset = dfile.getDataset();

	/* make sure that pixel data is loaded before output file is created */
	dataset->loadAllDataIntoMemory();

	E_TransferSyntax xfer = dataset->getOriginalXfer();

	DicomImage* di = new DicomImage(dataset, xfer);

	if (di == NULL) return false;

	if (di->getStatus() != EIS_Normal) return false;

	DicomImage* newImage = NULL;

	switch (type)
	{
	case SCALE_X:
		newImage = di->createScaledImage(factor, 0.0, 4);
		break;
	case SCALE_Y:
		newImage = di->createScaledImage(0.0, factor, 4);
		break;
	default:
		break;
	}

	if (!newImage->writeImageToDataset(*dataset)) return false;

	// update Image Type
	OFString imageType = "DERIVED";
	const char* oldImageType = NULL;
	if (dataset->findAndGetString(DCM_ImageType, oldImageType).good()) {
		if (oldImageType != NULL) {
			// append old image type information beginning with second entry
			const char* pos = strchr(oldImageType, '\\');
			if (pos != NULL)
				imageType += pos;
		}
	}
	dataset->putAndInsertString(DCM_ImageType, imageType.c_str());

	// update SOP Instance UID
	// add reference to source image
	DcmItem* ditem = NULL;
	const char* sopClassUID = NULL;
	const char* sopInstanceUID = NULL;
	if (dataset->findAndGetString(DCM_SOPClassUID, sopClassUID).good() &&
		dataset->findAndGetString(DCM_SOPInstanceUID, sopInstanceUID).good())
	{
		dataset->findAndDeleteElement(DCM_SourceImageSequence);
		if (dataset->findOrCreateSequenceItem(DCM_SourceImageSequence, ditem).good())
		{
			ditem->putAndInsertString(DCM_SOPClassUID, sopClassUID);
			ditem->putAndInsertString(DCM_SOPInstanceUID, sopInstanceUID);
		}
	}
	// create new SOP instance UID
	char new_uid[100];
	dataset->putAndInsertString(DCM_SOPInstanceUID, dcmGenerateUniqueIdentifier(new_uid));

	// ======================================================================
	// write back output file
	dfile.saveFile(outputFilename, xfer, EET_ExplicitLength, EGL_recalcGL, EPD_noChange, OFstatic_cast(Uint32, 0), OFstatic_cast(Uint32, 0), EWM_fileformat);

	// done, now cleanup. 
	delete di;
	delete newImage;

	return true;
}

bool clipAndScaleImageByFactor(const char* inputFilename, const char* outputFilename, const int left, const int top, const int width, const int height, const SCALE_TYPE type, const float factor) {
	DcmFileFormat dfile;
	OFCondition cond = dfile.loadFile(inputFilename);

	if (cond.bad()) return false;

	DcmDataset* dataset = dfile.getDataset();

	/* make sure that pixel data is loaded before output file is created */
	dataset->loadAllDataIntoMemory();

	E_TransferSyntax xfer = dataset->getOriginalXfer();

	DicomImage* di = new DicomImage(dataset, xfer);

	if (di == NULL) return false;

	if (di->getStatus() != EIS_Normal) return false;

	DicomImage* newImage = NULL;

	switch (type)
	{
	case SCALE_X:
		newImage = di->createScaledImage(left, top, width, height, factor, 0.0, 4);
		break;
	case SCALE_Y:
		newImage = di->createScaledImage(left, top, width, height, 0.0, factor, 4);
		break;
	default:
		break;
	}

	if (!newImage->writeImageToDataset(*dataset)) return false;

	// update Image Type
	OFString imageType = "DERIVED";
	const char* oldImageType = NULL;
	if (dataset->findAndGetString(DCM_ImageType, oldImageType).good()) {
		if (oldImageType != NULL) {
			// append old image type information beginning with second entry
			const char* pos = strchr(oldImageType, '\\');
			if (pos != NULL)
				imageType += pos;
		}
	}
	dataset->putAndInsertString(DCM_ImageType, imageType.c_str());

	// update SOP Instance UID
	// add reference to source image
	DcmItem* ditem = NULL;
	const char* sopClassUID = NULL;
	const char* sopInstanceUID = NULL;
	if (dataset->findAndGetString(DCM_SOPClassUID, sopClassUID).good() &&
		dataset->findAndGetString(DCM_SOPInstanceUID, sopInstanceUID).good())
	{
		dataset->findAndDeleteElement(DCM_SourceImageSequence);
		if (dataset->findOrCreateSequenceItem(DCM_SourceImageSequence, ditem).good())
		{
			ditem->putAndInsertString(DCM_SOPClassUID, sopClassUID);
			ditem->putAndInsertString(DCM_SOPInstanceUID, sopInstanceUID);
		}
	}
	// create new SOP instance UID
	char new_uid[100];
	dataset->putAndInsertString(DCM_SOPInstanceUID, dcmGenerateUniqueIdentifier(new_uid));

	// ======================================================================
	// write back output file
	dfile.saveFile(outputFilename, xfer, EET_ExplicitLength, EGL_recalcGL, EPD_noChange, OFstatic_cast(Uint32, 0), OFstatic_cast(Uint32, 0), EWM_fileformat);

	// done, now cleanup. 
	delete di;
	delete newImage;

	return true;
}

bool changeContrastOfImage(const char* inputFilename, const char* outputFilename) {

	DcmFileFormat dfile;
	OFCondition cond = dfile.loadFile(inputFilename);

	if (cond.bad()) return false;

	DcmDataset* dataset = dfile.getDataset();

	/* make sure that pixel data is loaded before output file is created */
	dataset->loadAllDataIntoMemory();

	E_TransferSyntax xfer = dataset->getOriginalXfer();

	DicomImage* di = new DicomImage(dataset, xfer);

	if (di == NULL) return false;

	if (di->getStatus() != EIS_Normal) return false;

	if (!di->setWindow(0)) return false;

	if (!di->setPolarity(EPP_Reverse)) return false;

	FILE* ofile = NULL;
	OFString ofname;
	unsigned int fcount = OFstatic_cast(unsigned int, di->getFrameCount());
	const char* ofext = "bmp";

	for (unsigned int frame = 0; frame < fcount; ++frame) {
		/* output to file */
		OFOStringStream stream;

		/* generate output filename */
		stream << outputFilename << ".";
		stream << frame;
		stream << "." << ofext << OFStringStream_ends;

		/* convert string stream into a character string */
		OFSTRINGSTREAM_GETSTR(stream, buffer_str)
			ofname.assign(buffer_str);
		OFSTRINGSTREAM_FREESTR(buffer_str)

			ofile = fopen(ofname.c_str(), "wb");

		if (ofile == NULL) return false;

		/* finally create output image file */
		if (!di->writeBMP(ofile, 32, frame)) return false;

		fclose(ofile);
	}

	// done, now cleanup. 
	delete di;

	return true;
}

bool applyExistingVOIWindows(const char* inputFilename, const char* outputFilename, const signed int numOfWindows, const VOI_LUT_FUNCTION voiLutFunc) {

	DcmFileFormat dfile;
	OFCondition cond = dfile.loadFile(inputFilename);

	if (cond.bad()) return false;

	DcmDataset* dataset = dfile.getDataset();

	/* make sure that pixel data is loaded before output file is created */
	dataset->loadAllDataIntoMemory();

	E_TransferSyntax xfer = dataset->getOriginalXfer();

	DicomImage* di = new DicomImage(dataset, xfer);

	if (di == NULL) return false;

	if (di->getStatus() != EIS_Normal) return false;

	if (voiLutFunc == LINEAR) {
		di->setVoiLutFunction(EFV_Linear);
	}
	else if (voiLutFunc == SIGMOID) {
		di->setVoiLutFunction(EFV_Sigmoid);
	}

	if (!di->setWindow(numOfWindows - 1)) return false;

	FILE* ofile = NULL;
	OFString ofname;
	unsigned int fcount = OFstatic_cast(unsigned int, di->getFrameCount());
	const char* ofext = "bmp";

	for (unsigned int frame = 0; frame < fcount; ++frame) {
		/* output to file */
		OFOStringStream stream;

		/* generate output filename */
		stream << outputFilename << ".";
		stream << frame;
		stream << "." << ofext << OFStringStream_ends;

		/* convert string stream into a character string */
		OFSTRINGSTREAM_GETSTR(stream, buffer_str)
			ofname.assign(buffer_str);
		OFSTRINGSTREAM_FREESTR(buffer_str)

			ofile = fopen(ofname.c_str(), "wb");

		if (ofile == NULL) return false;

		/* finally create output image file */
		if (!di->writeBMP(ofile, 32, frame)) return false;

		fclose(ofile);
	}

	// done, now cleanup. 
	delete di;

	return true;
}

bool applyMinMaxVOIWindows(const char* inputFilename, const char* outputFilename, const VOI_LUT_FUNCTION voiLutFunc) {

	DcmFileFormat dfile;
	OFCondition cond = dfile.loadFile(inputFilename);

	if (cond.bad()) return false;

	DcmDataset* dataset = dfile.getDataset();

	/* make sure that pixel data is loaded before output file is created */
	dataset->loadAllDataIntoMemory();

	E_TransferSyntax xfer = dataset->getOriginalXfer();

	DicomImage* di = new DicomImage(dataset, xfer);

	if (di == NULL) return false;

	if (di->getStatus() != EIS_Normal) return false;

	if (voiLutFunc == LINEAR) {
		di->setVoiLutFunction(EFV_Linear);
	}
	else if (voiLutFunc == SIGMOID) {
		di->setVoiLutFunction(EFV_Sigmoid);
	}

	if (!di->setMinMaxWindow(1)) return false;

	FILE* ofile = NULL;
	OFString ofname;
	unsigned int fcount = OFstatic_cast(unsigned int, di->getFrameCount());
	const char* ofext = "bmp";

	for (unsigned int frame = 0; frame < fcount; ++frame) {
		/* output to file */
		OFOStringStream stream;

		/* generate output filename */
		stream << outputFilename << ".";
		stream << frame;
		stream << "." << ofext << OFStringStream_ends;

		/* convert string stream into a character string */
		OFSTRINGSTREAM_GETSTR(stream, buffer_str)
			ofname.assign(buffer_str);
		OFSTRINGSTREAM_FREESTR(buffer_str)

			ofile = fopen(ofname.c_str(), "wb");

		if (ofile == NULL) return false;

		/* finally create output image file */
		if (!di->writeBMP(ofile, 32, frame)) return false;

		fclose(ofile);
	}

	// done, now cleanup. 
	delete di;

	return true;
}

bool applyHistogramVOIWindows(const char* inputFilename, const char* outputFilename, const signed int ignoringPercent, const VOI_LUT_FUNCTION voiLutFunc) {

	DcmFileFormat dfile;
	OFCondition cond = dfile.loadFile(inputFilename);

	if (cond.bad()) return false;

	DcmDataset* dataset = dfile.getDataset();

	/* make sure that pixel data is loaded before output file is created */
	dataset->loadAllDataIntoMemory();

	E_TransferSyntax xfer = dataset->getOriginalXfer();

	DicomImage* di = new DicomImage(dataset, xfer);

	if (di == NULL) return false;

	if (di->getStatus() != EIS_Normal) return false;

	if (voiLutFunc == LINEAR) {
		di->setVoiLutFunction(EFV_Linear);
	}
	else if (voiLutFunc == SIGMOID) {
		di->setVoiLutFunction(EFV_Sigmoid);
	}

	if (!di->setHistogramWindow(OFstatic_cast(double, ignoringPercent) / 100.0));

	FILE* ofile = NULL;
	OFString ofname;
	unsigned int fcount = OFstatic_cast(unsigned int, di->getFrameCount());
	const char* ofext = "bmp";

	for (unsigned int frame = 0; frame < fcount; ++frame) {
		/* output to file */
		OFOStringStream stream;

		/* generate output filename */
		stream << outputFilename << ".";
		stream << frame;
		stream << "." << ofext << OFStringStream_ends;

		/* convert string stream into a character string */
		OFSTRINGSTREAM_GETSTR(stream, buffer_str)
			ofname.assign(buffer_str);
		OFSTRINGSTREAM_FREESTR(buffer_str)

			ofile = fopen(ofname.c_str(), "wb");

		if (ofile == NULL) return false;

		/* finally create output image file */
		if (!di->writeBMP(ofile, 32, frame)) return false;

		fclose(ofile);
	}

	// done, now cleanup. 
	delete di;

	return true;
}

bool applySettingVOIWindows(const char* inputFilename, const char* outputFilename, const float center, const float width, const VOI_LUT_FUNCTION voiLutFunc) {

	DcmFileFormat dfile;
	OFCondition cond = dfile.loadFile(inputFilename);

	if (cond.bad()) return false;

	DcmDataset* dataset = dfile.getDataset();

	/* make sure that pixel data is loaded before output file is created */
	dataset->loadAllDataIntoMemory();

	E_TransferSyntax xfer = dataset->getOriginalXfer();

	DicomImage* di = new DicomImage(dataset, xfer);

	if (di == NULL) return false;

	if (di->getStatus() != EIS_Normal) return false;

	if (voiLutFunc == LINEAR) {
		di->setVoiLutFunction(EFV_Linear);
	}
	else if (voiLutFunc == SIGMOID) {
		di->setVoiLutFunction(EFV_Sigmoid);
	}

	if (!di->setWindow(center, width)) return false;

	FILE* ofile = NULL;
	OFString ofname;
	unsigned int fcount = OFstatic_cast(unsigned int, di->getFrameCount());
	const char* ofext = "bmp";

	for (unsigned int frame = 0; frame < fcount; ++frame) {
		/* output to file */
		OFOStringStream stream;

		/* generate output filename */
		stream << outputFilename << ".";
		stream << frame;
		stream << "." << ofext << OFStringStream_ends;

		/* convert string stream into a character string */
		OFSTRINGSTREAM_GETSTR(stream, buffer_str)
			ofname.assign(buffer_str);
		OFSTRINGSTREAM_FREESTR(buffer_str)

			ofile = fopen(ofname.c_str(), "wb");

		if (ofile == NULL) return false;

		/* finally create output image file */
		if (!di->writeBMP(ofile, 32, frame)) return false;

		fclose(ofile);
	}

	// done, now cleanup. 
	delete di;

	return true;
}