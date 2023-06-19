# pragma once

#include <string>
#include <vector>

#include "../../PyShellExec.hpp"
#include "../../websocket/SocketClient.hpp"

#include <BSlogger.hpp>

#include "proto/ShimmersensorProt.pb.h"

class Shimmersensor {
    private:
        PyShellExec shell_exec;
        SocketClient client;
    public:
        Shimmersensor();
        ~Shimmersensor();

        void run();
        void stop();
        
        void connect();

        void startStream();
        void stopStream();

        void readData();

        int accel_ln_x;
        int accel_ln_y;
        int accel_ln_z;
};