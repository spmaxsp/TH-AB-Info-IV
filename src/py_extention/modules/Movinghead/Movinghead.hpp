# pragma once

#include <string>
#include <vector>

#include "../../PyShellExec.hpp"
#include "../../websocket/SocketClient.hpp"

#include <BSlogger.hpp>

#include "proto/MovingheadProt.pb.h"

#define MH_PORT 50009
#define MH_HOST "127.0.0.1"


class Movinghead {
    private:
        PyShellExec shell_exec;
        SocketClient client;

        bool connected = false;

    public:
        Movinghead(std::string py_path);
        ~Movinghead();

        void run(int angle);
        void stop();
        
        void connect();
        void disconnect();

        // Control Commands
        void move_right();
        void move_left();
        void move_up();
        void move_down();
        void move_normal();
        void move_set_step(int step);

        // Getter
        std::string getLogs();
        bool getRunningState();
        bool getConnectedState();
};