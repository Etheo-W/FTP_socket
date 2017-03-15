#ifndef _SocketManger_H_
#define _SocketManger_H_


#include "CommonHeader.h"
#include "SocketCommand.h"
#include "SocketFileTransfer.h"

class SocketManger
{
public:
	SocketCommand		*socketCommond;
	SocketFileTransfer	*socketFileTransfer;
	AFHV				allFileHashValue;

	//HANDLE			hThread;

public:
	SocketManger();
	~SocketManger();

	int init();
	int work();
	int newThread(File* pParam);
};

#endif // !_SocketManger_H_
