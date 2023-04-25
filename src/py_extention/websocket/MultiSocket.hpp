#pragma once

#include <stdlib.h>
#include <string>
#include <vector>

#include "SocketClient.hpp"

struct ClientInfo {
    int index;
    std::string host;
    int port;
    bool connected;
    int out_protocol;
    int in_protocol;
    SocketClient* client;
};

class MultiSocket {
private:
    std::vector<ClientInfo> clients;
    int numClients;
public:
    MultiSocket();
    ~MultiSocket();

    int addConnection(const std::string host, int& port);
    void removeConnection(int index);

    int connect(int index);
    void disconnect(int index);

    void send(int index, const std::string& message);

    int getNumClients();
    std::string getHost(int index);
    int getPort(int index);
    bool isConnected(int index);
};