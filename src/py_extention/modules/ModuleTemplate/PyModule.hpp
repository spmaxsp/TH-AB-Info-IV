# pragma once

#include <string>
#include <vector>

#include "../../PyShellExec.hpp"
#include "../../websocket/SocketClient.hpp"

#include <BSlogger.hpp>

#include "proto/PyModuleProt.pb.h"

class PyModule {
    private:
        PyShellExec shell_exec;
        SocketClient client;
    public:
        PyModule();
        ~PyModule();

        void run();
        void stop();
        
        void connect();

        void startStream();
        void stopStream();

        void readData();
};