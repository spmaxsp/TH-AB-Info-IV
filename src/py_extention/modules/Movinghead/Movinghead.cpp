#include "Movinghead.hpp"

Movinghead::Movinghead(std::string py_path) : shell_exec("python -u", "scripts/Movinghead.py"), client(MH_HOST, MH_PORT) {
}

Movinghead::~Movinghead() {
    shell_exec.stop();
}

void Movinghead::run(int angle) {
    std::vector<std::string> args;

    args.push_back(std::to_string(MH_PORT));
    args.push_back(MH_HOST);
    args.push_back(std::to_string(angle));

    shell_exec.run(args);
}

void Movinghead::stop() {
    shell_exec.stop();
    connected = false;
}

void Movinghead::connect() {
    client.sockConnect();
    connected = true;
}

void Movinghead::disconnect() {
    client.sockDisconnect();
    connected = false;
}

void Movinghead::move_right(){
    MovingheadProt::SendCommand pb;
    pb.set_command(MovingheadProt::Command::COMMAND_MOVE_RIGHT);
    std::string data;
    pb.SerializeToString(&data);
    client.sockSend(data);
}

void Movinghead::move_left(){
    MovingheadProt::SendCommand pb;
    pb.set_command(MovingheadProt::Command::COMMAND_MOVE_LEFT);
    std::string data;
    pb.SerializeToString(&data);
    client.sockSend(data);
}

void Movinghead::move_up(){
    MovingheadProt::SendCommand pb;
    pb.set_command(MovingheadProt::Command::COMMAND_MOVE_UP);
    std::string data;
    pb.SerializeToString(&data);
    //std::string dbg = pb.DebugString();
    //std::cout << "Debug: " << dbg << "\n";
    client.sockSend(data);
}

void Movinghead::move_down(){
    MovingheadProt::SendCommand pb;
    pb.set_command(MovingheadProt::Command::COMMAND_MOVE_DOWN);
    std::string data;
    pb.SerializeToString(&data);
    //std::string dbg = pb.DebugString();
    //std::cout << "Debug: " << dbg << "\n";
    client.sockSend(data);
}

void Movinghead::move_normal(){
    MovingheadProt::SendCommand pb;
    pb.set_command(MovingheadProt::Command::COMMAND_MOVE_NORMAL);
    std::string data;
    pb.SerializeToString(&data);
    //std::string dbg = pb.DebugString();
    //std::cout << "Debug: " << dbg << "\n";
    client.sockSend(data);
}

void Movinghead::move_set_step(int value){
    MovingheadProt::SendCommand pb;
    pb.set_command(MovingheadProt::Command::COMMAND_SET_STEP);
    pb.set_value(value);
    std::string data;
    pb.SerializeToString(&data);
    //std::string dbg = pb.DebugString();
    //std::cout << "Debug: " << dbg << "\n";
    client.sockSend(data);
}

std::string Movinghead::getLogs() {
    return shell_exec.getOutput();
}

bool Movinghead::getRunningState() {
    return shell_exec.isRunning();
}

bool Movinghead::getConnectedState() {
    return connected;
}