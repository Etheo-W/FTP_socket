#ifndef _File_H_
#define _File_H_


#include "CommonHeader.h"
#include "SHA256.h"
#include "AllFileHashValue.h"

#include <fstream>

#define MAX_PATH 260

typedef enum fileUsageMode { R = 0, W , A} FILE_USAGE_MODE;

typedef enum { UPLOAD = 0, DOWNLOAD, GETALLFILEHASH } FILE_OPERATE_STYLE;

#define getFileUsageMode(FILE_USAGE_MODE) this->fileUsageModeName[FILE_USAGE_MODE]

class File
{
private:
	const char* fileUsageModeName[3] = { "rb", "wb", "ab" };
public:
	char filePath[MAX_PATH];
	char fileName[MAX_PATH];
	char fileFullPath[MAX_PATH];
	uint32_t fileSize;
	uint32_t fileSentSize;
	uint32_t fileReceivedSize;
	char fileHashSha256[65];

	FILE *fp;
public:
	File(FILE_OPERATE_STYLE requestStyle);
	~File();

	void	getFileHashSha256();

	int		openFile(FILE_USAGE_MODE mode);
	int		readFile(char* str, int n);
	int		writeFile(char* str, int n);
	int		closeFile();
	int		seekFile(FILE_OPERATE_STYLE style);
	int		getFileSize(FILE_OPERATE_STYLE style);

private:
	void	getFileFullPath();
};

#endif // !_File_H_
