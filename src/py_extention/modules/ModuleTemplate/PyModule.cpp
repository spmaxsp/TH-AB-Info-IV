#include "PyModule.hpp"

PyModule::PyModule() : shell_exec("python -u", "scripts/PyModule.py"), client("127.0.0.1", 50007) {
}

PyModule::~PyModule() {
    shell_exec.stop();
}

void PyModule::run() {
    shell_exec.run({});
}

void PyModule::stop() {
    shell_exec.stop();
}

void PyModule::connect() {
    client.sockConnect();
}

void PyModule::startStream() {
    PyModuleProt::SendCommand pb;
    pb.set_command(PyModuleProt::Command::COMMAND_START_STREAM);
    std::string data;
    pb.SerializeToString(&data);
    //std::string dbg = pb.DebugString();
    //std::cout << "Debug: " << dbg << "\n";
    client.sockSend(data);
}

void PyModule::stopStream() {
    PyModuleProt::SendCommand pb;
    pb.set_command(PyModuleProt::Command::COMMAND_STOP_STREAM);
    std::string data;
    pb.SerializeToString(&data);
    client.sockSend(data);

}

void PyModule::readData() {
    LOG_INIT_CERR();

    std::string data;
    int state = client.sockRecv(data);
    if (state <= 0) {
        log(LOG_ERROR) << "Error reading data with state: " << state << "\n";
    }
    else {
        PyModuleProt::DataPacket pb;
        pb.ParseFromString(data);
        int sensor_data = pb.some_sensor();
        log(LOG_INFO) << "Data: " << sensor_data << "\n";
    }
}