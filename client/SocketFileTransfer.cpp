#include "SocketFileTransfer.h"



SocketFileTransfer::SocketFileTransfer()
{
	for (int i = 0; i < DEFAULT_FILE_BUFLEN; i++)
	{
		this->sendbuf[i] = '\0';
		this->recvbuf[i] = '\0';
	}
}


SocketFileTransfer::~SocketFileTransfer()
{
}


int SocketFileTransfer::initSocket(char *IPAddress)
{
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sockfd < 0)
	{
		printf("fileSocket错误\n");
		return 0;
	}

	bzero(&(this->fileSocket), sizeof(this->fileSocket));

	this->fileSocket.sin_family = AF_INET;
	this->fileSocket.sin_addr.s_addr = htonl(INADDR_ANY);
	this->fileSocket.sin_port = htons(SERVER_FILE_PORT);
	inet_pton(AF_INET, IPAddress, &(this->fileSocket.sin_addr));
	this->len = sizeof(this->fileSocket);

	return 1;
}

int SocketFileTransfer::connect2FileServer()
{
	this->result = connect(this->sockfd, (struct sockaddr *) &(this->fileSocket), this->len);

	if (this->result == -1)
	{
		perror("连接失败");
		return 0;
	}

	return 1;
}

int SocketFileTransfer::sendFile(File *file)
{
	int sendbufLength = 0;

	if (!file->openFile(R)) return 0;

	if (!file->seekFile(UPLOAD)) return 0;

	do {
		// Send an initial buffer
		sendbufLength = file->readFile(this->sendbuf, DEFAULT_FILE_BUFLEN);
		if (!sendbufLength) break;

		this->result = write(this->sockfd, this->sendbuf, sendbufLength);
		if (this->result == EINTR)
		{
			printf("读取中断\n");
			return 0;
		}
		else if (this->result == EPIPE)
		{
			printf("连接中断\n");
			return 0;
		}

		file->fileSentSize += sendbufLength;

		printf(file->fileSentSize >= file->fileSize ? "文件上传完毕\n" : "文件已上传:%5.2f%%\n", (double)file->fileSentSize * 100 / file->fileSize);

	} while (this->result > 0 && file->fileSentSize < file->fileSize);

	file->closeFile();
	
	return 1;
}

int SocketFileTransfer::receiveFile(File *file)
{
	int recvbufLength = 0;

	if (!file->openFile(A))
	{
		if (!file->openFile(W)) return 0;
	}

	if (!file->seekFile(DOWNLOAD)) return 0;

	// Receive until the peer closes the connection
	do {

		this->result = read(this->sockfd, this->recvbuf, DEFAULT_FILE_BUFLEN);
		if (this->result > 0)
		{
			recvbufLength = this->result;

			if (!file->writeFile(this->recvbuf, recvbufLength)) return 0;

			file->fileReceivedSize += recvbufLength;

			printf(file->fileReceivedSize >= file->fileSize ? "文件下载完毕\n" : "文件已下载:%5.2f%%\n", (double)file->fileReceivedSize * 100 / file->fileSize);
			
		}
		else if (this->result == 0)
			printf("接受完毕\n");
		else if (this->result < 0)
		{
			printf("连接中断\n");
			return 0;
		}

	} while (this->result > 0 && file->fileReceivedSize < file->fileSize);

	file->closeFile();

	return 1;
}

void SocketFileTransfer::closeFileSocket()
{
	// cleanup
	close(this->sockfd);
}
