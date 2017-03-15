#ifndef _SocketFileTransfer_H_
#define _SocketFileTransfer_H_


#include "CommonHeader.h"
#include "File.h"

#define DEFAULT_FILE_BUFLEN 4194304 //4M
#define SERVER_FILE_PORT 1620

class SocketFileTransfer
{
public:
    int                 listen_sockfd, file_sockfd;
    int                 listen_len, file_len;
    struct sockaddr_in  listenSocket, fileSocket;

	int				result;

	//char			sendbuf[DEFAULT_FILE_BUFLEN];
	//char			recvbuf[DEFAULT_FILE_BUFLEN];
	
	//HANDLE		hThread;

public:
	SocketFileTransfer();
	~SocketFileTransfer();

	int		initSocket();
	int		connect2FileServer();
	int		acceptFileSocket();

	int		transfer(int sockfd, File *file);
	int		sendFile(int sockfd, File *file);
	int		receiveFile(int sockfd, File *file);

	void	closeFileSocket(int sockfd);
};

#endif // !_SocketFileTransfer_H_
