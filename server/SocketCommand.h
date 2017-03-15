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
    int                 listen_sockfd, command_sockfd;
    int                 listen_len, command_len;
	struct sockaddr_in  listenSocket, commandSocket;

	//char			sendbuf[DEFAULT_COMMAND_BUFLEN];
	//char			recvbuf[DEFAULT_COMMAND_BUFLEN];

	int				result;

	//HANDLE		hThread;

public:
	SocketCommand();
	~SocketCommand();

	int		initSocket();
	int		connect2FileServer();
	int		acceptCommandSocket();

	int		respons(int sockfd, File *file);
	int		sendCommandInfo(int sockfd, File *file, REQUEST_STYLE requestStyle);
	int		receiveCommandInfo(int sockfd, File *file);

	void 	closeCommandSocket(int sockfd);

private:
	void	getResponseInfo(File *file, char *response, const char *requestStyle);
	void	parseRequestInfo(File *file, char *request);
};

#endif // !_SocketCommand_H_
