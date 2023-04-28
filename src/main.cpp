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
    LOG_INIT_CERR();

    log(LOG_INFO) << "Starting\n";
    PythonRunner runner;
    PyModule module(&runner);

    module.RunFunctionMain();
    module.RunFunctionSomeFunction();
    module.RunFunctionSomeLoopingFunction();

    for(int i = 0; i < 10; i++){
        log(LOG_INFO) << "Looping\n";
        Sleep(1000);
    }

    return 0;
}