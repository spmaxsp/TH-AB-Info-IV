#include <stdlib.h>
#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "vulkan_base/vulkan_base.hpp"

#include "BSlogger.hpp"

//#include "py_extention/PyRunner.hpp"
#include "py_extention/PyShellExec.hpp"
//#include "py_extention/modules/ModuleTemplate/PyModule.hpp"
#include "py_extention/modules/EEG/EEG.hpp"

#include <Windows.h>

int main(int argc, char *argv[]) {
    // Initializing logger
    LOG_INIT_CERR();
    
    log(LOG_INFO) << "Loading module\n";
    EEG module;

    log(LOG_INFO) << "Running module\n";
    module.run(20, "lcP2cs4kNNbl2ps9F8TeVDRiw59S1xq1APhphqzg", "LKcNIrVNbvtob0qGVg5dSm45nxoLaAiDj8qfW1HjiPGTe4QgowIGzFuOXXMCy5d7xuXcateRNNy38GPZO8KtxKdfJiPfxd8Vh6OkAV5iZ6qF9PpK1KsepVU1YqRFDMg3");

    Sleep(500);

    log(LOG_INFO) << "Connecting to module\n";
    module.connect();

    Sleep(1000);

    log(LOG_INFO) << "Getting profiles\n";
    std::vector<std::string> profiles = module.getProfiles();

    log(LOG_INFO) << "Profiles:\n";
    for (auto profile : profiles) {
        log(LOG_INFO) << profile << "\n";
    }

    Sleep(1000);

    log(LOG_INFO) << "Setting profile\n";
    module.setProfile("default");

    Sleep(1000);

    log(LOG_INFO) << "Starting stream\n";
    module.startStream();

    Sleep(500);

    log(LOG_INFO) << "Reading data\n";
    for (int i = 0; i < 5; i++) {
        module.getLatestDataPacket();
        Sleep(1000);
    }

    log(LOG_INFO) << "Stopping stream\n";
    module.stopStream();

    log(LOG_INFO) << "Stopping module\n";
    module.stop();

    return 0;
}