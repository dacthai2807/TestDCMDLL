#include "pch.h"
#include "dcmfile.h"

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmimgle/dcmimage.h"

#include "dcmtk/dcmjpeg/djdecode.h"      /* for dcmjpeg decoders */
#include "dcmtk/dcmjpeg/dipijpeg.h"      /* for dcmimage JPEG plugin */

#include "dcmtk/dcmimage/dipipng.h"      /* for dcmimage PNG plugin */

bool convertDcmToBMP(const char* dcmFilename, const char* bmpFilename) {
	DcmFileFormat dfile;
	OFCondition cond = dfile.loadFile(dcmFilename);

	if (cond.bad()) return false;

	DcmDataset* dataset = dfile.getDataset();

	/* make sure that pixel data is loaded before output file is created */
	dataset->loadAllDataIntoMemory();

	E_TransferSyntax xfer = dataset->getOriginalXfer();

	DicomImage* di = new DicomImage(dataset, xfer);

	if (di == NULL) return false;

	if (di->getStatus() != EIS_Normal) return false;

	FILE* ofile = NULL;
	OFString ofname;
	unsigned int fcount = OFstatic_cast(unsigned int, di->getFrameCount());
	const char* ofext = "bmp";

	for (unsigned int frame = 0; frame < fcount; ++frame) {
		/* output to file */
		OFOStringStream stream;

		/* generate output filename */
		stream << bmpFilename;
		if (fcount > 1) stream << "." << frame;
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

bool convertDcmToJPG(const char* dcmFilename, const char* jpgFilename) {
	DcmFileFormat dfile;
	OFCondition cond = dfile.loadFile(dcmFilename);

	if (cond.bad()) return false;

	DcmDataset* dataset = dfile.getDataset();

	/* make sure that pixel data is loaded before output file is created */
	dataset->loadAllDataIntoMemory();

	E_TransferSyntax xfer = dataset->getOriginalXfer();

	DicomImage* di = new DicomImage(dataset, xfer);

	if (di == NULL) return false;

	if (di->getStatus() != EIS_Normal) return false;

	FILE* ofile = NULL;
	OFString ofname;
	unsigned int fcount = OFstatic_cast(unsigned int, di->getFrameCount());
	const char* ofext = "jpg";

	for (unsigned int frame = 0; frame < fcount; ++frame) {
		/* output to file */
		OFOStringStream stream;

		/* generate output filename */
		stream << jpgFilename;
		if (fcount > 1) stream << "." << frame;
		stream << "." << ofext << OFStringStream_ends;

		/* convert string stream into a character string */
		OFSTRINGSTREAM_GETSTR(stream, buffer_str)
			ofname.assign(buffer_str);
		OFSTRINGSTREAM_FREESTR(buffer_str)

			ofile = fopen(ofname.c_str(), "wb");

		if (ofile == NULL) return false;

		/* finally create output image file */
		DiJPEGPlugin plugin;
		plugin.setQuality(OFstatic_cast(unsigned int, 90));
		plugin.setSampling(ESS_422);
		if (!di->writePluginFormat(&plugin, ofile, frame)) return false;

		fclose(ofile);
	}

	// done, now cleanup. 
	delete di;

	return true;
}

bool convertDcmToPNG(const char* dcmFilename, const char* pngFilename) {
	DcmFileFormat dfile;
	OFCondition cond = dfile.loadFile(dcmFilename);

	if (cond.bad()) return false;

	DcmDataset* dataset = dfile.getDataset();

	// make sure that pixel data is loaded before output file is created 
	dataset->loadAllDataIntoMemory();

	E_TransferSyntax xfer = dataset->getOriginalXfer();

	DicomImage* di = new DicomImage(dataset, xfer);

	if (di == NULL) return false;

	if (di->getStatus() != EIS_Normal) return false;

	FILE* ofile = NULL;
	OFString ofname;
	unsigned int fcount = OFstatic_cast(unsigned int, di->getFrameCount());
	const char* ofext = "png";

	for (unsigned int frame = 0; frame < fcount; ++frame) {
		// output to file 
		OFOStringStream stream;

		// generate output filename 
		stream << pngFilename;
		if (fcount > 1) stream << "." << frame;
		stream << "." << ofext << OFStringStream_ends;

		// convert string stream into a character string 
		OFSTRINGSTREAM_GETSTR(stream, buffer_str)
			ofname.assign(buffer_str);
		OFSTRINGSTREAM_FREESTR(buffer_str)

			ofile = fopen(ofname.c_str(), "wb");

		if (ofile == NULL) return false;

		// finally create output image file 
		DiPNGPlugin pngPlugin;
		pngPlugin.setInterlaceType(E_pngInterlaceAdam7);
		pngPlugin.setMetainfoType(E_pngFileMetainfo);
		pngPlugin.setBitsPerSample(16);
		if (!di->writePluginFormat(&pngPlugin, ofile, frame)) return false;

		fclose(ofile);
	}

	// done, now cleanup. 
	delete di;

	return true;
}