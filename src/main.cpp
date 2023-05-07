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

#define _DEBUG

bool handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return false;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                return false;
            break;
        }
    }
    return true;
}

int main(int, char**) {
    // Initiate logger (default name is 'log')
    LOG_INIT_CERR();

    // Init SDL and create a window
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        log(LOG_ERROR) << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    if (window == nullptr) {
        log(LOG_ERROR) << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    // Get Instance Extensions
    uint32_t extensionCount = 0;
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
    std::vector<const char*> extensions(extensionCount);
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensions.data());

    // Set Instance Layers
    #ifdef _DEBUG
        const std::vector<const char*> layers = {
            "VK_LAYER_KHRONOS_validation"
        };
    #else
        const std::vector<const char*> layers = {};
    #endif

    // Set Device Extensions
    const std::vector<const char*> device_extensions = {};

    // Init Vulkan
    VulkanContext vkContext(layers, extensions, device_extensions);

    //while (handleEvents()) {
        
    //}
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}