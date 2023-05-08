#include <stdlib.h>
#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "vulkan_base/vulkan_base.hpp"

#include "BSlogger.hpp"

//#include "py_extention/PyRunner.hpp"
#include "py_extention/PyShellExec.hpp"
//#include "py_extention/modules/ModuleTemplate/PyModule.hpp"
#include "py_extention/modules/Shimmersensor/Shimmersensor.hpp"

#include <Windows.h>

int main(int argc, char *argv[]) {
    // Initializing logger
    LOG_INIT_CERR();
    
    log(LOG_INFO) << "Loading module\n";
    Shimmersensor module;

    log(LOG_INFO) << "Running module\n";
    module.run();

    Sleep(500);

    log(LOG_INFO) << "Connecting to module\n";
    module.connect();

    Sleep(1000);

    log(LOG_INFO) << "Starting stream\n";
    module.startStream();

    Sleep(500);

    log(LOG_INFO) << "Reading data\n";
    for (int i = 0; i < 3; i++) {
        module.readData();
        Sleep(1000);
    }

    log(LOG_INFO) << "Stopping stream\n";
    module.stopStream();

    log(LOG_INFO) << "Stopping module\n";
    module.stop();

    return 0;
}