#ifndef _WIN32
#define _WIN32
#endif

#ifndef WIDE_CHAR_FILE_IO_FUNCTIONS
#define WIDE_CHAR_FILE_IO_FUNCTIONS
#endif

#include "pch.h"
#include "dcmfile.h"

#include "dcmtk/dcmdata/dcddirif.h"

bool createDcmDir(const int numOfDcmFile, const char* dcmFilenames[]) {
	DicomDirInterface dicomdir;
	OFCondition status = dicomdir.createNewDicomDir();
	if (status.bad()) return false;

	for (int i = 0; i < numOfDcmFile; ++i) {
		OFString filename(dcmFilenames[i]);
		if (dicomdir.addDicomFile(OFFilename(filename)).bad()) return false;
	}

	if (dicomdir.writeDicomDir().bad()) return false;

	return true;
}

#if (defined(WIDE_CHAR_FILE_IO_FUNCTIONS) || defined(WIDE_CHAR_MAIN_FUNCTION)) && defined(_WIN32)

bool createDcmDirWC(const int numOfDcmFile, const wchar_t* dcmFilenames[]) {
	DicomDirInterface dicomdir;
	OFCondition status = dicomdir.createNewDicomDir();
	if (status.bad()) return false;

	for (int i = 0; i < numOfDcmFile; ++i) {
		OFFilename filename(dcmFilenames[i]);
		if (dicomdir.addDicomFile(filename).bad()) return false;
	}

	if (dicomdir.writeDicomDir().bad()) return false;

	return true;
}

#endif