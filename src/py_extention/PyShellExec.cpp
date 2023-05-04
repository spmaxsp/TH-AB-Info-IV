#include "PyShellExec.hpp"

#include <iostream>
#include <string>

#include <Windows.h>

PyShellExec::PyShellExec(std::string py_path, std::string py_script, std::vector<std::string> args){
    this->py_path = py_path;
    this->py_script = py_script;
    this->args = args;

    pipe = nullptr;
    running = false;
}

PyShellExec::~PyShellExec(){
}

void PyShellExec::generateCommand(std::string &command, std::string py_path, std::string py_script, std::vector<std::string> args){
    command = py_path + " " + py_script;
    for (int i = 0; i < args.size(); i++){
        command += " " + args[i];
    }
}

void PyShellExec::run(){
    std::string command;
    generateCommand(command, py_path, py_script, args);
    pipe = _popen(command.c_str(), "r");
    if (!pipe){
        std::cout << "Failed to open pipe" << std::endl;
        return;
    }
    running = true;

    output_thread = std::thread(&PyShellExec::readOutput, this);
}

void PyShellExec::stop(){
    if (running){
        _pclose(pipe);
        output_thread.join();
        running = false;
    }
}

std::string PyShellExec::getOutput(){
    return output;
}

bool PyShellExec::isRunning(){
    return running;
}

void PyShellExec::readOutput(){
    char buffer[1024];
    while (fgets(buffer, 1024, pipe) != NULL){

        std::cout << "\033[0;31m[PyShellExec]\033[0m" << "[Thread " << std::this_thread::get_id() << "] " << buffer << std::endl;
        output += buffer;
    }
}