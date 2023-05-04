#pragma once

#include "../../PyRunner.hpp"
#include "../../websocket/SocketClient.hpp"

#include "../../../BSlogger.hpp"

#include "proto/ShimmersensorProt.pb.h"

#include <string>

class Shimmersensor {
    private:
        PythonRunner* runner;
        SocketClient client;
        int moduleIndex;
    public:
        Shimmersensor(PythonRunner* runner):client("127.0.0.1", 50008){
            this->runner = runner;
            this->moduleIndex = runner->LoadModule("Shimmersensor");
        }
        ~Shimmersensor(){
            if (runner->getFunctionRunning(moduleIndex)){
                runner->JoinProcess(moduleIndex);
            }
            runner->UnloadModule(moduleIndex);
        }

        void StartServer(){
            LOG_INIT_CERR();
            log(LOG_INFO) << "Starting server\n";
            runner->StartProcess(moduleIndex, "startServer", {});
            runner->JoinProcess(moduleIndex);

            log(LOG_INFO) << "Waiting for connection\n";
            runner->StartProcess(moduleIndex, "serverWaitConnection", {});
            int state = client.sockConnect();
            log(LOG_INFO) << "Connection state: " << state << "\n";
            runner->JoinProcess(moduleIndex);

        }

        void StartSensor(){
            runner->StartProcess(moduleIndex,"StarteSensor", {});
            runner->JoinProcess(moduleIndex);
        }

        void fetchData(){
            LOG_INIT_CERR();
            log(LOG_INFO) << "Getting data\n";
            std::string data;
            client.sockRecv(data);
            shimmer::DataPacket pb;
            pb.ParseFromString(data);
            log(LOG_INFO) << "Data: " << pb.accel_ln_x() << "\n";

        }

        void StoppeSensor(){
            runner->StartProcess(moduleIndex,"StoppeSensor", {});
            runner->JoinProcess(moduleIndex);
        }



        void StopServer(){
            LOG_INIT_CERR();
            log(LOG_INFO) << "Stopping server\n";
            client.sockDisconnect();
            runner->StartProcess(moduleIndex, "stopServer", {});
            runner->JoinProcess(moduleIndex);
        }
};

