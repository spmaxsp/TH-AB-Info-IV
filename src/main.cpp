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
#include "py_extention/modules/Movinghead/Movinghead.hpp"


#include <Windows.h>

#define ShimmerRate 10 //Hz

int main(int argc, char *argv[]) {
    // Initializing logger
    LOG_INIT_CERR();
    
    log(LOG_INFO) << "Loading module Shimmer\n";
    Shimmersensor module_shimmer;
    log(LOG_INFO) << "Loading module Movinghead\n";
    Movinghead module_movinghead;

    log(LOG_INFO) << "Running module Shimmer\n";
    module_shimmer.run();
    log(LOG_INFO) << "Running module Movinghead\n";
    module_movinghead.run();

    Sleep(500);

    log(LOG_INFO) << "Connecting to module Shimmer\n";
    module_shimmer.connect();
    log(LOG_INFO) << "Connecting to module Movinghead\n";
    module_movinghead.connect();

    Sleep(3000);

    log(LOG_INFO) << "Resetting position of module Movinghead\n";
    module_movinghead.move_normal();

    log(LOG_INFO) << "Starting stream of module Shimmer\n";
    module_shimmer.startStream();

    log(LOG_INFO) << "Running Main Loop\n";
    for (int i = 0; i < 400; i++) {
        module_shimmer.readData();
        if(module_shimmer.accel_ln_x > 2200) {
            module_movinghead.move_right();
        }
        else if(module_shimmer.accel_ln_x < 1800) {
            module_movinghead.move_left();
        }
        else if(module_shimmer.accel_ln_y > 2200) {
            module_movinghead.move_up();
        }
        else if(module_shimmer.accel_ln_y < 1800) {
            module_movinghead.move_down();
        }
        Sleep(1000 / ShimmerRate);
    }

    log(LOG_INFO) << "Stopping module\n";
    module_shimmer.stop();
    module_movinghead.stop();

    log(LOG_INFO) << "Exiting\n";
    return 0;
}