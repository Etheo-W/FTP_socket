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

public:
	SocketManger();
	~SocketManger();

	int init(char *IPAddress);
	int work(char *IPAddress, REQUEST_STYLE requestStyle);
	int stop();
};

#endif // !_SocketManger_H_
