#pragma once

#include <string>
#include <vector>
#include <thread>

#ifdef _WIN32
#include <Windows.h>
#else
#include <signal.h>
#endif

#define RINGBUFFER_SIZE 1000

//#include <unistd.h>

class PyShellExec {
    private:
        std::string py_path;
        std::string py_script;
        std::vector<std::string> args;

        bool running;   

#ifdef _WIN32
        HANDLE handle;
        int pid;
        HANDLE stdout_pipe;  
        HANDLE stderr_pipe;   
#else
        pid_t pid;
        int stdout_pipe;
        int stderr_pipe;
#endif

        std::thread output_thread;

        int ringbuffer_len = 0;
        int ringbuffer_end = 0;
        char ringbuffer[RINGBUFFER_SIZE];

        void generateCommand(std::string &command, std::string py_path, std::string py_script, std::vector<std::string> args);
        void readOutput();
    public:
        PyShellExec(std::string py_path, std::string py_script);
        ~PyShellExec();

        bool run(std::vector<std::string> args);
        bool stop();

        std::string getOutput();
        bool isRunning();
};