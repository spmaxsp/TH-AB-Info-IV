#pragma once

#include "../../PyRunner.hpp"

class PyModule {
    private:
        PythonRunner* runner;
        int moduleIndex;
    public:
        PyModule(PythonRunner* runner){
            this->runner = runner;
            this->moduleIndex = runner->LoadModule("PyModule");
        }
        ~PyModule(){
            if (runner->getFunctionRunning(moduleIndex)){
                runner->JoinProcess(moduleIndex);
            }
            runner->UnloadModule(moduleIndex);
        }
        void RunFunctionMain(){
            runner->StartProcess(moduleIndex, "main", {});
            runner->JoinProcess(moduleIndex);
        }
        void RunFunctionSomeFunction(){
            runner->StartProcess(moduleIndex, "someFunction", {});
            runner->JoinProcess(moduleIndex);
        }
        void RunFunctionSomeLoopingFunction(){
            runner->StartProcess(moduleIndex, "someLoopingFunction", {});
        }
};

