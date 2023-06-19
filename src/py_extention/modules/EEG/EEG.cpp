#include "EEG.hpp"

EEG::EEG() : shell_exec("python -u", "scripts/EEG.py"), client(EEG_HOST, EEG_PORT) {
}

EEG::~EEG() {
    shell_exec.stop();
}

void EEG::run(int polling_rate, std::string userID, std::string token) {
    std::vector<std::string> args;

    args.push_back(std::to_string(EEG_PORT));
    args.push_back(EEG_HOST);
    args.push_back(std::to_string(polling_rate));
    args.push_back(userID);
    args.push_back(token);

    shell_exec.run(args);
}

void EEG::stop() {
    shell_exec.stop();
    connected = false;
    streaming = false;
}

void EEG::connect() {
    client.sockConnect();
    connected = true;
}

void EEG::disconnect() {
    client.sockDisconnect();
    connected = false;
}

void EEG::startStream() {
    //Prepare START_STREAM command
    EEGProt::SendCommand pb;
    pb.set_command(EEGProt::Command::COMMAND_START_STREAM);
    std::string data;
    pb.SerializeToString(&data);

    //Send START_STREAM command
    client.sockSend(data);
}

void EEG::stopStream() {
    //Prepare STOP_STREAM command
    EEGProt::SendCommand pb;
    pb.set_command(EEGProt::Command::COMMAND_STOP_STREAM);
    std::string data;
    pb.SerializeToString(&data);

    //Send STOP_STREAM command
    client.sockSend(data);

    streaming = false;
}

void EEG::pollInitState() {
    //Prepare GET_STATE command
    EEGProt::SendCommand pb;
    pb.set_command(EEGProt::Command::COMMAND_GET_STATE);
    std::string data;
    pb.SerializeToString(&data);

    //Send GET_STATE command
    client.sockSend(data);

    //Read response
    std::string response;
    int state = client.sockRecv(response);
    if (state <= 0) {
    }
    else {
        EEGProt::DataPacket pb;
        pb.ParseFromString(response);
        int state = pb.state();
        if (state == EEGProt::State::STATE_WAITING) {
            state_ready = false;
        }
        else if (state == EEGProt::State::STATE_READY) {
            state_ready = true;
            std::stringstream profile_sstr(pb.data());
            std::string item;
            while(std::getline(profile_sstr, item, ',')) {
                profiles.push_back(item);
            }
        }
    }
}

void EEG::setProfile(std::string profile) {
    //Prepare LOAD_PROFILE command
    EEGProt::SendCommand pb;
    pb.set_command(EEGProt::Command::COMMAND_LOAD_PROFILE);
    pb.set_data(profile);
    std::string data;
    pb.SerializeToString(&data);

    //Send LOAD_PROFILE command
    client.sockSend(data);
}

void EEG::readDataStream() {
    //Read data
    std::string data;
    int state = client.sockRecv(data);
    if (state <= 0) {
    }
    else {
        EEGProt::DataPacket pb;
        pb.ParseFromString(data);
        std::string mentalcommand = pb.data();
        int streaming = pb.state();
        if (streaming == EEGProt::State::STATE_STREAMING) {
            this->streaming = true;
        }
        else {
            this->streaming = false;
        }
    }
}

std::string EEG::getLogs() {
    return shell_exec.getOutput();
}

bool EEG::getRunningState() {
    return shell_exec.isRunning();
}

bool EEG::getConnectedState() {
    return connected;
}

bool EEG::getInitState() {
    return state_ready;
}

bool EEG::getStreamingState() {
    return streaming;
}

std::string EEG::getLatestDataPacket() {
    return latest_data_packet;
}

std::vector<std::string> EEG::getProfiles() {
    return profiles;
}