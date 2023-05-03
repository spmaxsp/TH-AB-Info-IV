#include <stdlib.h>
#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "vulkan_base/vulkan_base.hpp"

#include "BSlogger.hpp"

#include "py_extention/PyRunner.hpp"
#include "py_extention/modules/ModuleTemplate/PyModule.hpp"

#include <Windows.h>

int main(int argc, char *argv[]) {
    // Initializing logger
    LOG_INIT_CERR();
    
    // Creating PyRunner
    PythonRunner runner;

    // Loading PyModule
    PyModule module(&runner);

    // Running Some simple test functions
    module.RunFunctionMain();
    module.RunFunctionSomeFunction();

    module.RunFunctionTestGlobalVariable();


    // Testing the server and client
    module.RunStartServer();

    Sleep(2000);

    module.RunSendData();
    module.getServerData();

    module.RunStopServer();


    // Testing the looping function
    module.RunFunctionSomeLoopingFunction();

    for(int i = 0; i < 10; i++){
        log(LOG_INFO) << "Looping\n";
        Sleep(1000);
    }

    return 0;
}