#include "EEG.hpp"

EEG::EEG() : shell_exec("python -u", "scripts/EEG.py", {}), client("127.0.0.1", 50008) {
}

EEG::~EEG() {
    shell_exec.stop();
}

void EEG::run() {
    shell_exec.run();
}

void EEG::stop() {
    shell_exec.stop();
}

void EEG::connect() {
    client.sockConnect();
}

void EEG::startStream() {
    LOG_INIT_CERR();

    EEGProt::SendCommand pb;
    pb.set_command(EEGProt::Command::COMMAND_START_STREAM);
    std::string data;
    pb.SerializeToString(&data);

    std::string dbg = pb.DebugString();
    log(LOG_INFO) << "Sending data (raw): " << dbg;

    client.sockSend(data);
}

void EEG::stopStream() {
    EEGProt::SendCommand pb;
    pb.set_command(EEGProt::Command::COMMAND_STOP_STREAM);
    std::string data;
    pb.SerializeToString(&data);
    client.sockSend(data);

}

void EEG::readData() {
    LOG_INIT_CERR();

    std::string data;
    int state = client.sockRecv(data);
    if (state <= 0) {
        log(LOG_ERROR) << "Error reading data with state: " << state << "\n";
    }
    else {
        EEGProt::DataPacket pb;
        pb.ParseFromString(data);
        int accel_ln_x = pb.accel_ln_x();
        int accel_ln_y = pb.accel_ln_y();
        int accel_ln_z = pb.accel_ln_z();
        log(LOG_INFO) << "Data:  (x):" << accel_ln_x << "  (y):" << accel_ln_y << "  (z):" << accel_ln_x << "\n";
    }
}