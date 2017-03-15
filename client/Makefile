OBJECTS = main.o AllFileHashValue.o File.o SHA256.o SocketCommand.o SocketFileTransfer.o SocketManger.o

socketClient: $(OBJECTS)
	g++ -o socketClient $(OBJECTS)

main.o: CommonHeader.h SocketManger.h main.cpp
	g++ -c main.cpp
AllFileHashValue.o: AllFileHashValue.cpp
	g++ -c AllFileHashValue.cpp
File.o: CommonHeader.h SHA256.h AllFileHashValue.h File.cpp
	g++ -c File.cpp
SHA256.o: SHA256.cpp
	g++ -c SHA256.cpp
SocketCommand.o: CommonHeader.h File.h SocketCommand.cpp
	g++ -c SocketCommand.cpp
SocketFileTransfer.o: CommonHeader.h File.h SocketFileTransfer.cpp
	g++ -c SocketFileTransfer.cpp
SocketManger.o: CommonHeader.h SocketCommand.h SocketFileTransfer.h SocketManger.cpp
	g++ -c SocketManger.cpp

clean:
	rm $(OBJECTS)
