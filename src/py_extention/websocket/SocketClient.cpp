#include "SocketClient.hpp"

SocketClient::SocketClient(const std::string& host, int port) : host(host), port(port) {
    
}

SocketClient::~SocketClient() {
    
}

int SocketClient::sockInit(void){
    #ifdef _WIN32
        WSADATA wsa_data;
        return WSAStartup(MAKEWORD(2,2), &wsa_data);
    #else
        return 0;
    #endif
}

int SocketClient::sockQuit(void){
    #ifdef _WIN32
        return WSACleanup();
    #else
        return 0;
    #endif
}

int SocketClient::sockCreate(void){
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        sockClose();
        sockQuit();
        return 0;
    }
    return sock;
}

int SocketClient::sockClose(void){
    #ifdef _WIN32
        return closesocket(sock);
    #else
        return close(sock);
    #endif
}

int SocketClient::sockShutdown(void){
    int status = 0;
    #ifdef _WIN32
        status = shutdown(sock, SD_BOTH);
        //if (status == 0) { status = closesocket(sock); }
    #else
        status = shutdown(sock, SHUT_RDWR);
        //if (status == 0) { status = close(sock); }
    #endif
    return status;
}

int SocketClient::sockConnect(){

    if (sockInit() != 0) {
        throw std::runtime_error("Failed to initialize socket");
    }
    if (sockCreate() == INVALID_SOCKET) {
        throw std::runtime_error("Failed to create socket");
    }

    struct sockaddr_in sad;
    memset(&sad, 0, sizeof(sad));
    sad.sin_family = AF_INET;
    sad.sin_addr.s_addr = inet_addr(host.c_str());
    sad.sin_port = htons((u_short)port);

    if (connect(sock, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
        sockClose();
        sockQuit();
        return 0;
    }
    return 1;
}

int SocketClient::sockSend(const std::string& message){
    return send(sock, message.c_str(), message.length(), 0);
}

int SocketClient::sockRecv(std::string& message) {
    char buffer[1024];
    memset(&buffer, 0, sizeof(buffer));
    int status = 0;

#ifdef _WIN32
    // Set socket to non-blocking mode on Windows
    u_long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);
#else
    // Set socket to non-blocking mode on Unix-like systems
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
#endif

    // Attempt to receive data
    status = recv(sock, buffer, sizeof(buffer), 0);

    // Check for error or no data available
#ifdef _WIN32
    if (status == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK) {
#else
    if (status == -1 && errno == EAGAIN) {
#endif
        return 0; // No data available yet
    } else if (status <= 0) {
        return -1; // Error or connection closed
    }

    // Data received, copy to message string
    message = std::string(buffer, status);

    return status;
}

int SocketClient::sockDisconnect(void){
    sockShutdown();
    sockClose();
    sockQuit();
}
