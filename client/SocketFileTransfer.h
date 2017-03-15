#ifndef _SocketFileTransfer_H_
#define _SocketFileTransfer_H_


#include "CommonHeader.h"
#include "File.h"

#define DEFAULT_FILE_BUFLEN 4194304 //4M
#define SERVER_FILE_PORT 1620

class SocketFileTransfer
{
public:
	int 				sockfd;
	int 				len;
	struct sockaddr_in 	fileSocket;
	int 				result;

	char				sendbuf[DEFAULT_FILE_BUFLEN];
	char				recvbuf[DEFAULT_FILE_BUFLEN];

public:
	SocketFileTransfer();
	~SocketFileTransfer();

	int		initSocket(char *IPAddress);
	int		connect2FileServer();

	int		sendFile(File *file);
	int		receiveFile(File *file);

	void	closeFileSocket();
};

#endif // !_SocketFileTransfer_H_
