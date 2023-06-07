# pragma once

#include <string>
#include <vector>

#include "../../PyShellExec.hpp"
#include "../../websocket/SocketClient.hpp"

#include "../../../BSlogger.hpp"

#include "proto/EEGProt.pb.h"

#define EEG_PORT 50008
#define EEG_HOST "172.0.0.1"

class EEG {
    private:
        PyShellExec shell_exec;
        SocketClient client;
    public:
        EEG();
        ~EEG();

        void run(int port, std::string host, int polling_rate, std::string userID, std::string token);
        void stop();
        
        void connect();

        void startStream();
        void stopStream();

        void pollInitState(std::vector<std::string> &profiles, bool &init_ready);
        void setProfile(std::string profile);

        void readDataStream();

        std::string getLogs();
        bool getRunningState();
        bool getConnectedState();
        bool getInitState();
        std::string getLatestDataPacket();
};