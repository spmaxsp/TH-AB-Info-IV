# pragma once

#include <string>
#include <vector>

#include "../../PyShellExec.hpp"
#include "../../websocket/SocketClient.hpp"

#include "../../../BSlogger.hpp"

#include "proto/PyModuleProt.pb.h"

class Movinghead {
    private:
        PyShellExec shell_exec;
        SocketClient client;
    public:
        Movinghead();
        ~Movinghead();

        void run();
        void stop();
        
        void connect();

        void move_right();
        void move_left();
        void move_up();
        void move_down();
        void move_normal();
        void move_set_step(int);
};