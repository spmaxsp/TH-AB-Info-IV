#include <stdlib.h>
#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "vulkan_base/vulkan_base.hpp"

#include "BSlogger.hpp"

//#include "py_extention/PyRunner.hpp"
#include "py_extention/PyShellExec.hpp"
#include "py_extention/modules/ModuleTemplate/PyModule.hpp"
//#include "py_extention/modules/Shimmersensor/Shimmersensor.hpp"

#include <Windows.h>

int main(int argc, char *argv[]) {
    // Initializing logger
    LOG_INIT_CERR();
    
    log(LOG_INFO) << "Loading module\n";
    PyModule module;
    PyModule module2;

    log(LOG_INFO) << "Running module\n";
    module.run();
    Sleep(1000);
    module2.run();

    Sleep(10000);

    log(LOG_INFO) << "Connecting module\n";
    module.connect();

    log(LOG_INFO) << "Reading data\n";
    module.readData();

    log(LOG_INFO) << "Done\n";
    module.stop();
    module2.stop();
    log(LOG_INFO) << "Stopped\n";

    return 0;
}