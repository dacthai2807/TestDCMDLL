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