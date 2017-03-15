#include "SocketCommand.h"

SocketCommand::SocketCommand()
{
}

SocketCommand::~SocketCommand()
{
}

int SocketCommand::initSocket()
{
    this->listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->listen_sockfd < 0) {
        printf("jiantingsocket error\n");
        return 0;
    }

	bzero(&(this->listenSocket), sizeof(this->listenSocket));
	this->listenSocket.sin_family = AF_INET;
	this->listenSocket.sin_addr.s_addr = htonl(INADDR_ANY);
	this->listenSocket.sin_port = htons(SERVER_COMMAND_PORT);

	return 1;
}

int	SocketCommand::connect2FileServer()
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
	// closesocket(this->listenSocket);

	return 1;
}
int SocketCommand::acceptCommandSocket()
{
	// Accept a client socket
    socklen_t addrlen = sizeof(struct sockaddr);
	this->command_sockfd = accept(this->listen_sockfd, (struct sockaddr*)&(this->commandSocket), &addrlen);
	
	if (this->command_sockfd < 0) {
		printf("accept failed\n");
		return 0;
	}
	return 1;
}

int	SocketCommand::respons(int sockfd, File *file)
{
	//WaitForSingleObject(this->hThread, INFINITE);

	if (!this->receiveCommandInfo(sockfd, file)) return 0;

	if (!this->sendCommandInfo(sockfd, file, file->fileOperateStyle == GETALLFILEHASH ? GETALLFILEHASH : (file->fileOperateStyle == UPLOAD ? DOWNLOAD : UPLOAD))) return 0;

	if (file->fileOperateStyle == GETALLFILEHASH)
	{
		if (!this->receiveCommandInfo(sockfd, file)) return 0;

		if (!this->sendCommandInfo(sockfd, file, file->fileOperateStyle == UPLOAD ? DOWNLOAD : UPLOAD)) return 0;
	}

	//Sleep(1000);
	//ReleaseMutex(this->hThread);

	//CloseHandle(this->hThread);
	//this->closeCommandSocket(cmdSocket);

	return 1;
}

int	SocketCommand::sendCommandInfo(int sockfd, File *file, REQUEST_STYLE requestStyle)
{
	char *sendbuf = new char[DEFAULT_COMMAND_BUFLEN];

	for (int i = 0; i < DEFAULT_COMMAND_BUFLEN; i++)
	{
		sendbuf[i] = '\0';
	}

	this->getResponseInfo(file, sendbuf, getRequestStyle(requestStyle));

	// Send an initial buffer
	this->result = (int)write(sockfd, sendbuf, (int)strlen(sendbuf));
	if (this->result < 0) {
		printf("send failed\n");
		return 0;
	}

	delete[] sendbuf;

	return 1;
}

int	SocketCommand::receiveCommandInfo(int sockfd, File *file)
{
	char *recvbuf = new char[DEFAULT_COMMAND_BUFLEN];

	for (int i = 0; i < DEFAULT_COMMAND_BUFLEN; i++)
	{
		recvbuf[i] = '\0';
	}

	this->result = (int)read(sockfd, recvbuf, DEFAULT_COMMAND_BUFLEN);
	if (this->result > 0)
	{
		this->parseRequestInfo(file, recvbuf);
		

		if (file->fileOperateStyle == UPLOAD)
		{
			file->getFileFullPath();

			file->getFileSize(UPLOAD);
			file->fileReceivedSize = file->fileSize;
		}

		if (file->fileOperateStyle == DOWNLOAD)
		{
			file->getFileFullPath();

			uint32_t fileSize = file->fileSize;
			file->getFileSize(DOWNLOAD);
			file->fileReceivedSize = file->fileSize;
			file->fileSize = fileSize;
			file->fileSentSize = file->fileSize;
		}
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

	delete[] recvbuf;

	return 1;
}

void SocketCommand::closeCommandSocket(int sockfd)
{
	// cleanup
	close(sockfd);
	//WSACleanup();
}

void SocketCommand::getResponseInfo(File *file, char *response, const char *requestStyle)
{
    //响应信息格式：请求类型|文件已接受大小|文件已发送大小|文件大小
    //若请求类型为UPLOAD，则文件大小为请求消息中文件大小
    //若请求类型为DOWNLOAD，则文件已上传大小为文件大小
    
	if (!strcmp(requestStyle, getRequestStyle(GETALLFILEHASH)))
	{
		strcat(response, requestStyle);
		
		AFHV allFHV = getAllFileHashValue();

		for (int i = 0; i < allFHV->length; i++)
		{
			strcat(response, "|");
			strcat(response, allFHV->allFileHashValue[i]);
		}

		return;
	}

	strcat(response, requestStyle);
	strcat(response, "|");

	string fileReceivedSizeString = to_string(file->fileReceivedSize);
	//int2String((int)file->fileReceivedSize, fileReceivedSizeString, 10);
	strcat(response, fileReceivedSizeString.c_str());
	strcat(response, "|");

	string fileSentSizeString = to_string(file->fileSentSize);
	//int2String((int)file->fileSentSize, fileSentSizeString, 10);
	strcat(response, fileSentSizeString.c_str());
	strcat(response, "|");

	string fileSizeString = to_string(file->fileSize);
	//int2String((int)file->fileSize, fileSizeString, 10);
	strcat(response, fileSizeString.c_str());
}

void SocketCommand::parseRequestInfo(File *file, char *request)
{
    //请求信息格式：请求类型|文件名|hash值|文件大小
    //若请求类型为UPLOAD，则文件大小为客户端已上传文件大小
    //若请求类型为DOWNLOAD，则文件大小为客户端已下载文件大小
    
    char *token = NULL;
	
	token = strtok(request, "|");

	if (!strcmp(token, getRequestStyle(GETALLFILEHASH)))
	{
		file->fileOperateStyle = GETALLFILEHASH;
		return;
	}

	if (!strcmp(token, getRequestStyle(UPLOAD)))
	{
		file->fileOperateStyle = DOWNLOAD;
	}

	if (!strcmp(token, getRequestStyle(DOWNLOAD)))
	{
		file->fileOperateStyle = UPLOAD;
	}

	token = strtok(NULL, "|");
	token = strtok(NULL, "|");
	strcpy(file->fileHashSha256, token);

	if (file->fileOperateStyle == UPLOAD)
	{
		token = strtok(NULL, "|");
		file->fileSentSize = (uint32_t)atof(token);
	}

	if (file->fileOperateStyle == DOWNLOAD)
	{
		token = strtok(NULL, "|");
		file->fileSize = (uint32_t)atof(token);
	}
}
