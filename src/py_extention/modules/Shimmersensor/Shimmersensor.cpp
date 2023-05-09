#include "Shimmersensor.hpp"

Shimmersensor::Shimmersensor() : shell_exec("python -u", "scripts/Shimmersensor.py", {}), client("127.0.0.1", 50008) {
}

Shimmersensor::~Shimmersensor() {
    shell_exec.stop();
}

void Shimmersensor::run() {
    shell_exec.run();
}

void Shimmersensor::stop() {
    shell_exec.stop();
}

void Shimmersensor::connect() {
    client.sockConnect();
}

void Shimmersensor::startStream() {
    LOG_INIT_CERR();

    ShimmersensorProt::SendCommand pb;
    pb.set_command(ShimmersensorProt::Command::COMMAND_START_STREAM);
    std::string data;
    pb.SerializeToString(&data);

    std::string dbg = pb.DebugString();
    log(LOG_INFO) << "Sending data (raw): " << dbg << "\n";

    client.sockSend(data);
}

void Shimmersensor::stopStream() {
    ShimmersensorProt::SendCommand pb;
    pb.set_command(ShimmersensorProt::Command::COMMAND_STOP_STREAM);
    std::string data;
    pb.SerializeToString(&data);
    client.sockSend(data);

}

void Shimmersensor::readData() {
    LOG_INIT_CERR();

    std::string data;
    int state = client.sockRecv(data);
    if (state <= 0) {
        log(LOG_ERROR) << "Error reading data with state: " << state << "\n";
    }
    else {
        ShimmersensorProt::DataPacket pb;
        pb.ParseFromString(data);
        int accel_ln_x = pb.accel_ln_x();
        int accel_ln_y = pb.accel_ln_y();
        int accel_ln_z = pb.accel_ln_z();
        log(LOG_INFO) << "Data:  (x):" << accel_ln_x << "  (y):" << accel_ln_y << "  (z):" << accel_ln_x << "\n";
    }
}