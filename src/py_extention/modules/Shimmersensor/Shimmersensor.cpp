#include "Shimmersensor.hpp"

Shimmersensor::Shimmersensor(std::string py_path) : shell_exec(py_path, "scripts/Shimmersensor.py"), client(SHIMMER_HOST, SHIMMER_PORT) {
}

Shimmersensor::~Shimmersensor() {
    shell_exec.stop();
}

void Shimmersensor::run(int polling_rate, std::string port) {
    std::vector<std::string> args;

    args.push_back(std::to_string(SHIMMER_PORT));
    args.push_back(SHIMMER_HOST);
    args.push_back(std::to_string(polling_rate));
    args.push_back(port);

    shell_exec.run(args);
}

void Shimmersensor::stop() {
    shell_exec.stop();
    connected = false;
    streaming = false;
}

void Shimmersensor::connect() {
    client.sockConnect();
    connected = true;
}

void Shimmersensor::disconnect() {
    client.sockDisconnect();
    connected = false;
}

void Shimmersensor::startStream() {
    LOG_INIT_CERR();

    ShimmersensorProt::SendCommand pb;
    pb.set_command(ShimmersensorProt::Command::COMMAND_START_STREAM);
    std::string data;
    pb.SerializeToString(&data);

    std::string dbg = pb.DebugString();
    log(LOG_INFO) << "Sending data (raw): " << dbg;

    client.sockSend(data);

    streaming = true;
}

void Shimmersensor::stopStream() {
    ShimmersensorProt::SendCommand pb;
    pb.set_command(ShimmersensorProt::Command::COMMAND_STOP_STREAM);
    std::string data;
    pb.SerializeToString(&data);
    client.sockSend(data);

    streaming = false;
}

void Shimmersensor::readDataStream() {
    LOG_INIT_CERR();

    std::string data;
    int state = client.sockRecv(data);
    if (state <= 0) {
        log(LOG_ERROR) << "Error reading data with state: " << state << "\n";
    }
    else {
        ShimmersensorProt::DataPacket pb;
        pb.ParseFromString(data);
        accel_ln_x = pb.accel_ln_x();
        accel_ln_y = pb.accel_ln_y();
        accel_ln_z = pb.accel_ln_z();

        if (pb.state() == ShimmersensorProt::State::STATE_STREAMING){
            streaming = true; 
        }
        log(LOG_INFO) << "Data:  (x):" << accel_ln_x << "  (y):" << accel_ln_y << "  (z):" << accel_ln_x << "\n";
    }
}

std::string Shimmersensor::getLogs() {
    return shell_exec.getOutput();
}

bool Shimmersensor::getRunningState() {
    return shell_exec.isRunning();
}

bool Shimmersensor::getConnectedState() {
    return connected;
}

bool Shimmersensor::getStreamingState() {
    return streaming;
}

int Shimmersensor::get_Accel_x() {
    return accel_ln_x;
}

int Shimmersensor::get_Accel_y() {
    return accel_ln_y;
}

int Shimmersensor::get_Accel_z() {
    return accel_ln_z;
}