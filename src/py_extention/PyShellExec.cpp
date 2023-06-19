#include "PyShellExec.hpp"

#include <iostream>
#include <string>


PyShellExec::PyShellExec(std::string py_path, std::string py_script){
    this->py_path = py_path;
    this->py_script = py_script;

    stdout_pipe = nullptr;
    running = false;

#ifdef _WIN32
    handle = nullptr;
#else
    pid = 0;
#endif
}

PyShellExec::~PyShellExec(){
}

void PyShellExec::generateCommand(std::string &command, std::string py_path, std::string py_script, std::vector<std::string> args){
    command = py_path + " " + py_script;
    for (int i = 0; i < args.size(); i++){
        command += " " + args[i];
    }
}

bool PyShellExec::run(std::vector<std::string> args){
    std::string command;
    generateCommand(command, py_path, py_script, args);

#ifdef _WIN32
    //Creating Security Attributes
    SECURITY_ATTRIBUTES saAttr = {0};
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    //Creating pipes
    HANDLE outPipeRead, outPipeWrite;
    if (!CreatePipe(&outPipeRead, &outPipeWrite, &saAttr, 0)) {
        std::cerr << "Error: could not create pipe" << std::endl;
        return false;
    }

    //Setting up STARTUPINFO
    STARTUPINFO siStartInfo = {0};
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.dwFlags |= STARTF_USESHOWWINDOW;
    siStartInfo.wShowWindow = SW_HIDE;
    siStartInfo.hStdError = outPipeWrite;
    siStartInfo.hStdOutput = outPipeWrite;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    //Starting process
    PROCESS_INFORMATION piProcInfo = {0};
    if (!CreateProcess(NULL, (LPSTR)command.c_str(), NULL, NULL, TRUE,
                        CREATE_NO_WINDOW | NORMAL_PRIORITY_CLASS,
                        NULL, NULL, &siStartInfo, &piProcInfo)) {
        std::cerr << "Error: could not start process" << std::endl;
        CloseHandle(outPipeRead);
        CloseHandle(outPipeWrite);
        return false;
    }
    CloseHandle(outPipeWrite);
    handle = piProcInfo.hProcess;
    stdout_pipe = outPipeRead;
#else
    int out_pipe[2];
        if (pipe(out_pipe) == -1) {
            std::cerr << "Error: could not create pipe" << std::endl;
            return false;
        }

    pid = ();
    if (pid == -1) {
        std::cerr << "Error: could not start process" << std::endl;
    } 
    else if (pid == 0) {
        dup2(out_pipe[1], STDOUT_FILENO);
        dup2(out_pipe[1], STDERR_FILENO);
        close(out_pipe[0]);
        close(out_pipe[1]);
        return false
    }
    fclose(out_pipe[1]);
    stdout_pipe = fdopen(out_pipe[0], "r");
#endif

    output_thread = std::thread(&PyShellExec::readOutput, this);
    running = true;
    return true;
}

bool PyShellExec::stop(){
#ifdef _WIN32
    if (TerminateProcess(handle, 0)) {
        CloseHandle(handle);
    } 
    else {
        std::cerr << "Error: could not kill process" << std::endl;
    }
    CloseHandle(stdout_pipe);
    //output_thread.join();
    return true;
#else
    if (kill(pid, SIGTERM) == 0) {
    } 
    else {
        std::cerr << "Error: could not kill process" << std::endl;
    }
    fclose(stdout_pipe);
    //output_thread.join();
    return true;
#endif
}

std::string PyShellExec::getOutput(){
    return output;
}

bool PyShellExec::isRunning(){
    return running;
}

void PyShellExec::readOutput(){
    char buffer[BUFSIZ];
#ifdef _WIN32
    DWORD bytesRead;
    while (ReadFile(stdout_pipe, buffer, BUFSIZ, &bytesRead, NULL)) {
        if (bytesRead == 0) {
            break;
        }
        if (!((buffer[0] == '\r' || buffer[0] == '\n') && buffer[2] == 0)) {
            std::cout << "\033[0;32m[PyShellExec]" << "[Thread " << std::this_thread::get_id() << "]\033[0m " << buffer << std::endl;
        }
        memset(buffer, 0, BUFSIZ);
    }
#else
    while (fgets(buffer, BUFSIZ, stdout_pipe) != nullptr) {
        if (strlen(buffer) > 0) {
            std::cout << "\033[0;31m[PyShellExec]\033[0m" << "[Thread " << std::this_thread::get_id() << "] " << buffer << std::endl;
        }
    }
#endif
}