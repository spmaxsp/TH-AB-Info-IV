# pragma once

#include <string>
#include <vector>

#include "../../PyShellExec.hpp"
#include "../../websocket/SocketClient.hpp"

#include <BSlogger.hpp>

#include "proto/ShimmersensorProt.pb.h"

#define SHIMMER_PORT 50007
#define SHIMMER_HOST "127.0.0.1"


class Shimmersensor {
    private:
        PyShellExec shell_exec;
        SocketClient client;

        bool streaming = false;
        bool connected = false;

        int accel_ln_x;
        int accel_ln_y;
        int accel_ln_z;

    public:
        Shimmersensor(std::string py_path);
        ~Shimmersensor();

        void run(int polling_rate, std::string port);
        void stop();
        
        void connect();
        void disconnect();

        // Control Commands
        void startStream();
        void stopStream();

        void readDataStream();

        // Getter
        std::string getLogs();
        bool getRunningState();
        bool getConnectedState();
        bool getStreamingState();
        
        int get_Accel_x();
        int get_Accel_y();
        int get_Accel_z();
};