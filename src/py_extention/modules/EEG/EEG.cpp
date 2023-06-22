#include "EEG.hpp"

EEG::EEG(std::string py_path) : shell_exec(py_path, "scripts/EEG.py"), client(EEG_HOST, EEG_PORT) {
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
    stream_enabled = false;
}

void EEG::connect() {
    client.sockConnect();
    connected = true;
    streaming = false;
    stream_enabled = false;
}

void EEG::disconnect() {
    client.sockDisconnect();
    connected = false;
    streaming = false;
    stream_enabled = false;
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
            std::string profiles_str = pb.data();
            std::istringstream profiles_sstr(profiles_str);
            std::string profile;
            while (std::getline(profiles_sstr, profile, ',')) {
                profiles.push_back(profile);
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

void EEG::startStream() {
    //Prepare START_STREAM command
    EEGProt::SendCommand pb;
    pb.set_command(EEGProt::Command::COMMAND_START_STREAM);
    std::string data;
    pb.SerializeToString(&data);

    //Send START_STREAM command
    client.sockSend(data);

    stream_enabled = true;
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
    stream_enabled = false;
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
            latest_data_packet = mentalcommand;
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

bool EEG::getStreamEnabled() {
    return stream_enabled;
}

std::string EEG::getLatestDataPacket() {
    return latest_data_packet;
}

std::vector<const char*> EEG::getProfiles() {
    std::vector<const char*> profiles_cstr;
    for (auto& profile : profiles) {
        profiles_cstr.push_back(profile.c_str());
    }
    return profiles_cstr;
}