#include "Movinghead.hpp"

Movinghead::Movinghead() : shell_exec("python -u", "scripts/Movinghead.py", {}), client("127.0.0.1", 50009) {
}

Movinghead::~Movinghead() {
    shell_exec.stop();
}

void Movinghead::run() {
    shell_exec.run();
}

void Movinghead::stop() {
    shell_exec.stop();
}

void Movinghead::connect() {
    client.sockConnect();
}

void Movinghead::move_right(){
    MovingheadProt::SendCommand pb;
    pb.set_command(MovingheadProt::Command::COMMAND_MOVE_RIGHT);
    std::string data;
    pb.SerializeToString(&data);
    //std::string dbg = pb.DebugString();
    //std::cout << "Debug: " << dbg << "\n";
    client.sockSend(data);
}

void Movinghead::move_left(){
    MovingheadProt::SendCommand pb;
    pb.set_command(MovingheadProt::Command::COMMAND_MOVE_LEFT);
    std::string data;
    pb.SerializeToString(&data);
    //std::string dbg = pb.DebugString();
    //std::cout << "Debug: " << dbg << "\n";
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