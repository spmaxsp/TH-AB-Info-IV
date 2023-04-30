#include "../../PyRunner.hpp"
#include "../../websocket/SocketClient.hpp"

#include "../../../BSlogger.hpp"

#include "proto/PyModuleProt.pb.h"

#include <string>

class PyModule {
    private:
        PythonRunner* runner;
        SocketClient client;
        int moduleIndex;
    public:
        PyModule(PythonRunner* runner):client("127.0.0.1", 50007){
            this->runner = runner;
            this->moduleIndex = runner->LoadModule("PyModule");
        }
        ~PyModule(){
            if (runner->getFunctionRunning(moduleIndex)){
                runner->JoinProcess(moduleIndex);
            }
            runner->UnloadModule(moduleIndex);
        }
        void RunFunctionMain(){
            LOG_INIT_CERR();
            log(LOG_INFO) << "Running Python Func: main\n";
            runner->StartProcess(moduleIndex, "main", {});
            runner->JoinProcess(moduleIndex);
        }
        void RunFunctionSomeFunction(){
            LOG_INIT_CERR();
            log(LOG_INFO) << "Running Python Func: someFunction\n";
            runner->StartProcess(moduleIndex, "someFunction", {});
            runner->JoinProcess(moduleIndex);
        }
        void RunFunctionSomeLoopingFunction(){
            LOG_INIT_CERR();
            log(LOG_INFO) << "Running Python Func: someLoopingFunction\n";
            runner->StartProcess(moduleIndex, "someLoopingFunction", {});
        }
        void RunFunctionTestGlobalVariable(){
            LOG_INIT_CERR();
            log(LOG_INFO) << "Running Python Func: testGlobalVariable1\n";
            runner->StartProcess(moduleIndex, "testGlobalVariable1", {});
            runner->JoinProcess(moduleIndex);
            log(LOG_INFO) << "Running Python Func: testGlobalVariable2\n";
            runner->StartProcess(moduleIndex, "testGlobalVariable2", {});
            runner->JoinProcess(moduleIndex);
        }

        void RunStartServer(){
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

        void RunSendData(){
            LOG_INIT_CERR();
            log(LOG_INFO) << "Sending data\n";
            runner->StartProcess(moduleIndex, "serverSendData", {});
            runner->JoinProcess(moduleIndex);
        }

        void getServerData(){
            LOG_INIT_CERR();
            log(LOG_INFO) << "Getting data\n";
            std::string data;
            client.sockRecv(data);
            PyModuleProt::DataPacket pb;
            pb.ParseFromString(data);
            log(LOG_INFO) << "Data: " << pb.some_sensor() << "\n";
        }

        void RunStopServer(){
            LOG_INIT_CERR();
            log(LOG_INFO) << "Stopping server\n";
            client.sockDisconnect();
            runner->StartProcess(moduleIndex, "stopServer", {});
            runner->JoinProcess(moduleIndex);
        }
};

