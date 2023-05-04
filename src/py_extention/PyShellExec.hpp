# pragma once

#include <string>
#include <vector>
#include <thread>

class PyShellExec {
    private:
        std::string py_path;
        std::string py_script;
        std::vector<std::string> args;

        bool running;
        FILE* pipe;
        std::thread output_thread;

        std::string output;

        void generateCommand(std::string &command, std::string py_path, std::string py_script, std::vector<std::string> args);
        void readOutput();
    public:
        PyShellExec(std::string py_path, std::string py_script, std::vector<std::string> args);
        ~PyShellExec();

        void run();
        void stop();

        std::string getOutput();
        bool isRunning();
};