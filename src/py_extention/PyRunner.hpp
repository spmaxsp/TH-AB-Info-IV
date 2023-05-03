#pragma once

#include <stdlib.h>
#include "../BSlogger.hpp"
#include <Python.h>
#include <thread>
#include <fstream>
#include <windows.h>

// Structs for passing data to threads
struct RunFunctionParams {
    PyObject* PyClass;
    std::string function_name;
    std::vector<std::string> args;
    PyInterpreterState* interp;
};

struct LoadModuleParams {
    std::string module_name;
    PyObject* PyClass;
    PyInterpreterState* interp;
};

struct ModuleInfo {
    int moduleId;

    std::thread active_thread;

    std::string loadedModuleName;
    bool moduleLoaded;
    PyObject* PyClass;

    std::string loadedFunctionName;
    bool functionRunning;

    PyThreadState* PyThreadState;
};

// PythonRunnerFunctions.cpp (functions called by threads)
void ActionRunFunction(std::shared_ptr<RunFunctionParams> params);
void ActionLoadModule(std::shared_ptr<LoadModuleParams> params);
void ActionUnloadModule(std::shared_ptr<LoadModuleParams> params);

// PythonRunner.cpp (class)
class PythonRunner {
    private:
        std::vector<ModuleInfo> Modules;
        PyThreadState* mainThreadState;
    public:
        PythonRunner();
        ~PythonRunner();
        int LoadModule(std::string module_name);
        void UnloadModule(int moduleIndex);

        void StartProcess(int moduleIndex, std::string function_name, std::vector<std::string> args);
        void JoinProcess(int moduleIndex);

        int getnumModules();
        int getModuleIndex(std::string module_name);
        std::string getModuleName(int moduleIndex);

        bool getFunctionRunning(int moduleIndex);
        std::string getLoadedFunctionName(int moduleIndex);

        bool checkModuleExists(std::string module_name);
        bool checkFunctionExists(std::string module_name, std::string function_name);
};
