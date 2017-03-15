OBJECTS = main.o AllFileHashValue.o File.o SocketCommand.o SocketFileTransfer.o SocketManger.o

socketServer: $(OBJECTS)
	g++ -o socketServer $(OBJECTS)

main.o: CommonHeader.h SocketManger.h main.cpp
	g++ -c main.cpp
AllFileHashValue.o: AllFileHashValue.cpp
	g++ -c AllFileHashValue.cpp
File.o: CommonHeader.h AllFileHashValue.h File.cpp
	g++ -c File.cpp
SocketCommand.o: CommonHeader.h File.h SocketCommand.cpp
	g++ -c SocketCommand.cpp
SocketFileTransfer.o: CommonHeader.h File.h SocketFileTransfer.cpp
	g++ -c SocketFileTransfer.cpp
SocketManger.o: CommonHeader.h SocketCommand.h SocketFileTransfer.h SocketManger.cpp
	g++ -c SocketManger.cpp

clean:
	rm $(OBJECTS)
