#include "MultiSocket.hpp"

MultiSocket::MultiSocket() {
    numClients = 0;
}

MultiSocket::~MultiSocket() {
    for(int i = 0; i < numClients; i++) {
        removeConnection(i);
    }
}

int MultiSocket::addConnection(const std::string host, int& port) {
    ClientInfo client;
    client.index = numClients;
    client.host = host;
    client.port = port;
    client.connected = false;
    client.out_protocol = 0;
    client.in_protocol = 0;

    client.client = new SocketClient(host, port);

    clients.push_back(client);
    numClients++;

    return client.index;
}

void MultiSocket::removeConnection(int index) {
    if (index < numClients) {
        delete clients[index].client;
        clients.erase(clients.begin() + index);
        numClients--;
    }
}

int MultiSocket::connect(int index) {
    if (index < numClients) {
        int ret = clients[index].client->sockConnect();
        return ret;
    }
    return 1;
}

void MultiSocket::disconnect(int index) {
    if (index < numClients) {
        clients[index].client->sockDisconnect();
        clients[index].connected = false;
    }
}

void MultiSocket::send(int index, const std::string& message) {
    if (index < numClients) {
        clients[index].client->sockSend(message);
    }
}

int MultiSocket::getNumClients() {
    return numClients;
}

std::string MultiSocket::getHost(int index) {
    if (index < numClients) {
        return clients[index].host;
    }
    return "";
}

int MultiSocket::getPort(int index) {
    if (index < numClients) {
        return clients[index].port;
    }
    return 0;
}

bool MultiSocket::isConnected(int index) {
    if (index < numClients) {
        return clients[index].connected;
    }
    return false;
}

