# pragma once

#include <string>
#include <vector>

#include "../../PyShellExec.hpp"
#include "../../websocket/SocketClient.hpp"

#include "../../../BSlogger.hpp"

#include "proto/EEGProt.pb.h"

class EEG {
    private:
        PyShellExec shell_exec;
        SocketClient client;
    public:
        EEG();
        ~EEG();

        void run();
        void stop();
        
        void connect();

        void startStream();
        void stopStream();

        void readData();
};