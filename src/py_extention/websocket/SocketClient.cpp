#include "SocketClient.hpp"

SocketClient::SocketClient(const std::string& host, int port) : host(host), port(port) {
    if (sockInit() != 0) {
        throw std::runtime_error("Failed to initialize socket");
    }
    if (sockCreate() == INVALID_SOCKET) {
        throw std::runtime_error("Failed to create socket");
    }
}

SocketClient::~SocketClient() {
    sockShutdown();
    sockQuit();
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
        if (status == 0) { status = closesocket(sock); }
    #else
        status = shutdown(sock, SHUT_RDWR);
        if (status == 0) { status = close(sock); }
    #endif
    return status;
}

int SocketClient::sockConnect(){
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

int SocketClient::sockDisconnect(void){
    return sockClose();
}
