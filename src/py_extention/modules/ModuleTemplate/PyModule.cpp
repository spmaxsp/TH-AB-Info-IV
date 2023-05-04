#include "PyModule.hpp"

PyModule::PyModule() : shell_exec("python -u", "scripts/PyModule.py", {}), client("127.0.0.1", 50007) {
}

PyModule::~PyModule() {
}

void PyModule::run() {
    shell_exec.run();
}

void PyModule::stop() {
    shell_exec.stop();
}

void PyModule::connect() {
    client.sockConnect();
}

void PyModule::readData() {
    std::string data;
    client.sockRecv(data);
    PyModuleProt::DataPacket pb;
    pb.ParseFromString(data);
    std::cout << "Data: " << pb.some_sensor() << "\n";
}