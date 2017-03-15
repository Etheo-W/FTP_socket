#include "SocketFileTransfer.h"

SocketFileTransfer::SocketFileTransfer()
{
}


SocketFileTransfer::~SocketFileTransfer()
{
}


int SocketFileTransfer::initSocket()
{
    this->listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->listen_sockfd < 0) {
        printf("jiantingsocket error\n");
        return 0;
    }
  
    bzero(&(this->listenSocket), sizeof(this->listenSocket));
	this->listenSocket.sin_family = AF_INET;
	this->listenSocket.sin_addr.s_addr = htonl(INADDR_ANY);
	this->listenSocket.sin_port = htons(SERVER_FILE_PORT);

	return 1;
}

int SocketFileTransfer::connect2FileServer()
{
	// Setup the TCP listening socket
    this->result = ::bind(this->listen_sockfd, (struct sockaddr*)&(this->listenSocket), sizeof(this->listenSocket));
	if (this->result < 0) {
		printf("bind failed\n");
		return 0;
	}

	this->result = listen(this->listen_sockfd, SOMAXCONN);
	if (this->result < 0) {
		printf("listen failed\n");
		return 0;
	}

	

	// No longer need server socket
	//closesocket(this->listenSocket);

	return 1;
}

int SocketFileTransfer::acceptFileSocket()
{
	// Accept a client socket
    socklen_t addrlen = sizeof(struct sockaddr);
	this->file_sockfd = accept(this->listen_sockfd, (struct sockaddr*)&(this->listenSocket), &addrlen);
    if (this->file_sockfd < 0) {
		printf("accept failed\n");
		return 0;
	}

	return 1;
}

int	SocketFileTransfer::transfer(int sockfd, File *file)
{
	//WaitForSingleObject(this->hThread, INFINITE);

	if (file->fileOperateStyle == DOWNLOAD)
	{
		if (!this->receiveFile(sockfd, file)) return 0;
	}

	if (file->fileOperateStyle == UPLOAD)
	{
		if (!this->sendFile(sockfd, file)) return 0;
	}

	//this->closeFileSocket(FTSocket);

	delete file;

	//Sleep(1000);
	//ReleaseMutex(this->hThread);

	//CloseHandle(this->hThread);

	return 1;
}

int SocketFileTransfer::sendFile(int sockfd, File *file)
{
	int sendbufLength = 0;

	if (!file->openFile(R)) return 0;

	if (!file->seekFile(UPLOAD)) return 0;

	char *sendbuf = new char[DEFAULT_FILE_BUFLEN];

	for (int i = 0; i < DEFAULT_FILE_BUFLEN; i++)
	{
		sendbuf[i] = '\0';
	}

	do {
		// Send an initial buffer
		sendbufLength = file->readFile(sendbuf, DEFAULT_FILE_BUFLEN);
		if (!sendbufLength) break;		

		this->result = (int)write(sockfd, sendbuf, sendbufLength);
		if (this->result < 0) {
			printf("send failed\n");
			return 0;
		}

		file->fileSentSize += sendbufLength;

		printf(file->fileSentSize >= file->fileSize ? "文件发送完毕!\n" : "文件已发送:%5.2f%%\n", (double)(file->fileSentSize * 100 / file->fileSize));

	} while (this->result > 0 && file->fileSentSize < file->fileSize);

	file->closeFile();

	delete[] sendbuf;
	
	return 1;
}

int SocketFileTransfer::receiveFile(int sockfd, File *file)
{
	int recvbufLength = 0;

	if (!file->openFile(A))
	{
		if (!file->openFile(W)) return 0;
	}

	if (!file->seekFile(DOWNLOAD)) return 0;

	char *recvbuf = new char[DEFAULT_FILE_BUFLEN];

	for (int i = 0; i < DEFAULT_FILE_BUFLEN; i++)
	{
		recvbuf[i] = '\0';
	}

	// Receive until the peer closes the connection
	do {

		this->result = (int)read(sockfd, recvbuf, DEFAULT_FILE_BUFLEN);
		if (this->result > 0)
		{
			recvbufLength = this->result;

			if (!file->writeFile(recvbuf, recvbufLength)) return 0;

			file->fileReceivedSize += recvbufLength;

			printf(file->fileReceivedSize >= file->fileSize ? "文件接收完毕!\n" : "文件已接收:%5.2f%%\n", (double)file->fileReceivedSize * 100 / file->fileSize);
			
		}
		else if (this->result == 0)
		{
			printf("Connection closed\n");
			return 0;
		}
		else
		{
			printf("recv failed\n");
			return 0;
		}

	} while (this->result > 0 && file->fileReceivedSize < file->fileSize);

	if (file->fileReceivedSize >= file->fileSize)
	{
		AFHV allFHV = getAllFileHashValue();
		addHashValue(allFHV, file->fileHashSha256);
	}

	file->closeFile();

	delete[] recvbuf;

	return 1;
}

void SocketFileTransfer::closeFileSocket(int sockfd)
{
	// cleanup
	close(sockfd);
	//WSACleanup();
}
