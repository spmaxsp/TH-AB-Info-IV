#pragma once

#include <string>
#include <vector>
#include <thread>

#ifdef _WIN32
#include <Windows.h>
#else
#include <signal.h>
#endif

//#include <unistd.h>

class PyShellExec {
    private:
        std::string py_path;
        std::string py_script;
        std::vector<std::string> args;

        bool running;   

#ifdef _WIN32
        HANDLE handle;
        HANDLE stdout_pipe;  
        HANDLE stderr_pipe;   
#else
        pid_t pid;
        int stdout_pipe;
        int stderr_pipe;
#endif

        std::thread output_thread;

        std::string output;

        void generateCommand(std::string &command, std::string py_path, std::string py_script, std::vector<std::string> args);
        void readOutput();
    public:
        PyShellExec(std::string py_path, std::string py_script, std::vector<std::string> args);
        ~PyShellExec();

        bool run();
        bool stop();

        //std::string getOutput();
        bool isRunning();
};