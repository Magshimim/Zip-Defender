#include "ZipHeader.h"

ZipHeader::ZipHeader(FILE* &file)
{
	signature = readInt(file); 
	versionNeeded = readShort(file);
	bitFlag = readShort(file);
	compressionMethod = readShort(file);
	lastModifiedTime = readShort(file);
	lastModifiedDate = readShort(file);
	crc32 = readInt(file);
	compressedSize = readInt(file);
	uncompressedSize = readInt(file);
	double ratio = (double)uncompressedSize / compressedSize;

	nameLength = readShort(file);
	extraLength = readShort(file);

	fileName = readString(file, nameLength);
	extraField = readString(file, extraLength);
}

void ZipHeader::print()
{
	printf("Signature: %d\n", signature);
	printf("version needed: %d\n", versionNeeded);
	printf("bit flag: %d\n", bitFlag);
	printf("compression method: %d\n", compressionMethod);
	printf("last modified time: %d\n", lastModifiedTime);
	printf("last modified date: %d\n", lastModifiedDate);
	printf("CRC32: %d\n", crc32);
	printf("compressed size: %d\n", compressedSize);
	printf("uncompressed size: %d\n", uncompressedSize);
	double ratio = (double)uncompressedSize / compressedSize;
	printf("Ration: %lf\n", ratio);

	printf("name length: %d\n", nameLength);
	printf("extra length: %d\n", extraLength);

	cout << "file name: " << fileName << endl;
	//cout << "extra field: " << extraField << endl;
}

int ZipHeader::getCompressedSize()
{
	return compressedSize;
}

int readInt(FILE* file)
{
	char temp[5];
	memset(temp, 0, 5);
	if(fgets(temp, 5, file) == NULL)
		return -1;
	return *(int*)temp;
}

short readShort(FILE* file)
{
	char temp[3];
	memset(temp, 0, 3);
	if(fgets(temp, 3, file) == NULL)
		return -1;
	return *(short*)temp;
}

string readString(FILE* file, int length)
{
	if(length <= 0)
		return "";
	char *temp = new char[length+1];
	memset(temp, 0, length+1);
	string toRet;
	if(fgets(temp, length+1, file) == NULL)
	{
		toRet = "";
	}
	else
		toRet = string(temp, length);
	delete[] temp;
	return toRet;
}