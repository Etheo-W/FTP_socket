#include "SocketCommand.h"

SocketCommand::SocketCommand()
{
	for (int i = 0; i < DEFAULT_COMMAND_BUFLEN; i++)
	{
		this->sendbuf[i] = '\0';
		this->recvbuf[i] = '\0';
	}
}


SocketCommand::~SocketCommand()
{
}

int SocketCommand::initSocket(char *IPAddress)
{
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sockfd < 0)
	{
		printf("commandSocket错误\n");
		return 0;
	}

	bzero(&(this->commandSocket), sizeof(this->commandSocket));
	this->commandSocket.sin_family = AF_INET;
	this->commandSocket.sin_addr.s_addr = htonl(INADDR_ANY);
	this->commandSocket.sin_port = htons(SERVER_COMMAND_PORT);
	inet_pton(AF_INET, IPAddress, &(this->commandSocket.sin_addr));
	this->len = sizeof(this->commandSocket);

	return 1;
}

int	SocketCommand::connect2FileServer()
{
	this->result = connect(this->sockfd, (struct sockaddr *) &(this->commandSocket), this->len);

	if (this->result == -1)
	{
		perror("连接失败");
		return 0;
	}

	return 1;
}

int	SocketCommand::sendCommandInfo(File *file, REQUEST_STYLE requestStyle)
{
	for (int i = 0; i < DEFAULT_COMMAND_BUFLEN; i++)
	{
		this->sendbuf[i] = '\0';
	}

	this->getRequestInfo(file, this->sendbuf, getRequestStyle(requestStyle));

	// Send an initial buffer
	write(this->sockfd, this->sendbuf, (int)strlen(this->sendbuf));
	
	return 1;
}

int	SocketCommand::receiveCommandInfo(File *file)
{
	for (int i = 0; i < DEFAULT_COMMAND_BUFLEN; i++)
	{
		this->recvbuf[i] = '\0';
	}

	this->result = read(this->sockfd, this->recvbuf, DEFAULT_COMMAND_BUFLEN);

	if (this->result > 0)
	{
		this->parseResponseInfo(file, this->recvbuf);
	}
	else if (this->result == 0)
		printf("接受完毕\n");
	else if (this->result == EINTR)
		{
			printf("读取中断\n");
			return 0;
		}
	else if (this->result < 0)
		{
			printf("连接中断\n");
			return 0;
		}

	return 1;
}

void SocketCommand::closeCommandSocket()
{
	// cleanup
	close(this->sockfd);
}

void SocketCommand::getRequestInfo(File *file, char *request, const char *requestStyle)
{
	// 请求信息格式：请求类型|文件名|hash值|文件大小
	// 若请求类型为DOWNLOAD，则文件大小为0
	if (!strcmp(requestStyle, getRequestStyle(GETALLFILEHASH)))
	{
		strcat(request, requestStyle);
		return;
	}

	strcat(request, requestStyle);
	strcat(request, "|");

	strcat(request, file->fileHashSha256);
	strcat(request, "|");

	strcat(request, file->fileHashSha256);
	strcat(request, "|");

	string fileSizeString = "";
	// int2String((int)file->fileSize, fileSizeString, 10);
	fileSizeString = to_string(file->fileSize);
	strcat(request, fileSizeString.c_str());
}

void SocketCommand::parseResponseInfo(File *file, char *response)
{
	// 响应信息格式：请求类型|文件已上传大小|文件已下载大小|文件大小
	// 若请求类型为UPLOAD，则文件大小为请求消息中文件大小
	// 若请求类型为DOWNLOAD，则文件已上传大小为文件大小
	char *token = NULL;
	
	token = strtok(response, "|");

	if (!strcmp(token, getRequestStyle(GETALLFILEHASH)))
	{
		AFHV allFHV = getAllFileHashValue();

		token = strtok(NULL, "|");

		while (token)
		{
			addHashValue(allFHV, token);
			token = strtok(NULL, "|");
		}

		return;
	}

	if (!strcmp(token, getRequestStyle(UPLOAD)))
	{
		token = strtok(NULL, "|");
		file->fileSentSize = (uint32_t)atof(token);
		//file->fileSize = file->fileSentSize;
	}

	if (!strcmp(token, getRequestStyle(DOWNLOAD)))
	{
		token = strtok(NULL, "|");
		token = strtok(NULL, "|");
		//file->fileReceivedSize = (uint32_t)atof(token);

		token = strtok(NULL, "|");
		file->fileSize = (uint32_t)atof(token);
	}
}
