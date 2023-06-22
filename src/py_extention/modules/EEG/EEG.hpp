# pragma once

#include <string>
#include <vector>

#include "../../PyShellExec.hpp"
#include "../../websocket/SocketClient.hpp"

#include <BSlogger.hpp>

#include "proto/EEGProt.pb.h"

#define EEG_PORT 50008
#define EEG_HOST "127.0.0.1"


class EEG {
    private:
        PyShellExec shell_exec;
        SocketClient client;

        bool state_ready = false;
        bool streaming = false;
        bool stream_enabled = false;
        bool connected = false;

        std::string latest_data_packet;

        std::vector<std::string> profiles;

    public:
        char profile[20] = "default";

        EEG(std::string py_path);
        ~EEG();

        void run(int polling_rate, std::string userID, std::string token);
        void stop();
        
        void connect();
        void disconnect();

        // Control Commands
        void pollInitState();

        void setProfile(std::string profile);

        void startStream();
        void stopStream();

        void readDataStream();

        // Getter
        std::string getLogs();
        bool getRunningState();
        bool getConnectedState();
        bool getInitState();
        bool getStreamingState();
        bool getStreamEnabled();

        std::string getLatestDataPacket();
        std::vector<const char*> getProfiles();
};