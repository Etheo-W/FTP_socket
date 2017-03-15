#ifndef _SocketCommand_H_
#define _SocketCommand_H_


#include "CommonHeader.h"
#include "File.h"

#define DEFAULT_COMMAND_BUFLEN 4194304	//4M
#define SERVER_COMMAND_PORT 1621

typedef FILE_OPERATE_STYLE REQUEST_STYLE;

#define getRequestStyle(REQUEST_STYLE) this->requestStyleName[REQUEST_STYLE]

class SocketCommand
{
private:
	const char* requestStyleName[3] = { "UPLOAD", "DOWNLOAD", "GETALLFILEHASH" };

public:
	int 				sockfd;
	int 				len;
	struct sockaddr_in 	commandSocket;
	int 				result;

	char 				sendbuf[DEFAULT_COMMAND_BUFLEN];
	char 				recvbuf[DEFAULT_COMMAND_BUFLEN];

public:
	SocketCommand();
	~SocketCommand();

	int		initSocket(char *IPAddress);
	int		connect2FileServer();

	int		sendCommandInfo(File *file, REQUEST_STYLE requestStyle);
	int		receiveCommandInfo(File *file);

	void 	closeCommandSocket();

private:
	void	getRequestInfo(File *file, char *request, const char *requestStyle);
	void	parseResponseInfo(File *file, char *response);
};

#endif // !_SocketCommand_H_
