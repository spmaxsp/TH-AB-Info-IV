#pragma once

#include <string>
#include <vector>

#ifdef _WIN32
    #include <winsock2.h>
    #include <Ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h> 
#endif

#include <BSlogger.hpp>

class SocketClient {
private:
    std::string host;
    int port;
    SOCKET sock = INVALID_SOCKET;
    bool connected = false;

    int sockInit(void);
    int sockQuit(void);

    int sockCreate(void);
    int sockClose(void);
    int sockShutdown(void);
public:
    SocketClient(const std::string& host, int port);
    ~SocketClient();

    int sockConnect();
    int sockSend(const std::string& message);
    int sockRecv(std::string& message);
    int sockDisconnect(void);

    bool isConnected();
};