#include <stdlib.h>
#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "vulkan_base/vulkan_base.hpp"

#include "BSlogger.hpp"

#include "py_extention/ExtentionManager.hpp"

#include <Windows.h>

int main(int argc, char *argv[]) {
    LOG_INIT_CERR();

    ExtentionManager extention_manager;

    std::vector<std::string> extention_names;
    extention_manager.ListPythonExtention(extention_names);

    for (int i = 0; i < extention_names.size(); i++) {
        log(LOG_INFO) << "Loading extention: " << extention_names[i] << "\n";
        extention_manager.AddPythonExtention(extention_names[i]);
    }

    log(LOG_INFO) << "Connecting to extention: PythonCapiDemo\n";
    extention_manager.connectPythonExtention("PythonCapiDemo");
    log(LOG_INFO) << "Connecting to extention: PythonCapiDemocopy\n";
    extention_manager.connectPythonExtention("PythonCapiDemocopy");

    log(LOG_INFO) << "Doing stuff\n";
    for (int i = 0; i < 10; i++) {
        log(LOG_INFO) << "Printed from C++\n";
        Sleep(1000);
    }

    return 0;
}