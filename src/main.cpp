#include <stdlib.h>
#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "vulkan_base/vulkan_base.hpp"

#include "BSlogger.hpp"

//#include "py_extention/PyRunner.hpp"
#include "py_extention/PyShellExec.hpp"
//#include "py_extention/modules/ModuleTemplate/PyModule.hpp"
//#include "py_extention/modules/Shimmersensor/Shimmersensor.hpp"
#include "py_extention/modules/Movinghead/Movinghead.hpp"


#include <Windows.h>

int main(int argc, char *argv[]) {
    // Initializing logger
    LOG_INIT_CERR();
    
    log(LOG_INFO) << "Loading module\n";
    Movinghead module;

    log(LOG_INFO) << "Running module\n";
    module.run();

    Sleep(500);

    log(LOG_INFO) << "Connecting to module\n";
    module.connect();

    Sleep(3000);

    for(int i = 0; i < 4; i++) {
        log(LOG_INFO) << "Moving right\n";
        module.move_right();
        Sleep(2000);
        log(LOG_INFO) << "Moving right\n";
        module.move_right();
        Sleep(2000);
        log(LOG_INFO) << "Moving up\n";
        module.move_up();
        Sleep(2000);
        log(LOG_INFO) << "Moving up\n";
        module.move_up();
        Sleep(2000);
        log(LOG_INFO) << "Moving left\n";
        module.move_left();
        Sleep(2000);
        log(LOG_INFO) << "Moving left\n";
        module.move_left();
        Sleep(2000);
        log(LOG_INFO) << "Moving down\n";
        module.move_down();
        Sleep(2000);
        log(LOG_INFO) << "Moving down\n";
        module.move_down();
        Sleep(2000);
    }

    log(LOG_INFO) << "Stopping module\n";
    //module.stop();

    return 0;
}