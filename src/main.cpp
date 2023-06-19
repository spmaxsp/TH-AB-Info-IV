#include <stdlib.h>

#include "vulkan_engine.hpp"
#include "imgui_ui/imgui_ui.hpp"

#include "gamelogic.hpp"

<<<<<<< src/main.cpp
//#include "py_extention/PyRunner.hpp"
#include "py_extention/PyShellExec.hpp"
//#include "py_extention/modules/ModuleTemplate/PyModule.hpp"
#include "py_extention/modules/Shimmersensor/Shimmersensor.hpp"
#include "py_extention/modules/Movinghead/Movinghead.hpp"

=======
#include <BSlogger.hpp>
>>>>>>> src/main.cpp

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
    #define Sleep(x) usleep((x)*1000)
#endif

<<<<<<< src/main.cpp
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
=======
#define _DEBUG

bool handleEvents() {
	ImGuiIO& io = ImGui::GetIO();

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL2_ProcessEvent(&event);

		switch (event.type) {
		case SDL_QUIT:
			return false;
		case SDL_KEYDOWN:
			if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE && !io.WantCaptureKeyboard) {
				SDL_SetRelativeMouseMode(SDL_FALSE);
			}
		case SDL_MOUSEBUTTONDOWN:
			if(event.button.button == SDL_BUTTON_LEFT && !io.WantCaptureMouse) {
				SDL_SetRelativeMouseMode(SDL_TRUE);
			}
			break;
		}
	}
	return true;
}

int main(int, char**) {
    // Initiate logger (default name is 'log')
    LOG_INIT_CERR();
    
    //log(LOG_INFO) << "Loading module\n";
    //Shimmersensor module;

    // Init SDL and create a window
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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

    // Init Sensors
    Shimmersensor shimmersensor;
    //EEG eeg;
    //Movinghead movinghead;
>>>>>>> src/main.cpp

    // Init Webcam
    Webcam webcam(1280, 720);

<<<<<<< src/main.cpp
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
=======
    // Init GameLogic
    GameLogic game(&shimmersensor, &webcam);

    // Initiate Vulkan and ImGui
    ImguiUI imgui(&game);

    VulkanEngine app;
    app.InitVulkan(window, &imgui);

    // run Main Loop
    while (handleEvents() && game.gstate.AppRunning) {
        // Update GameLogic
        game.updateGame();

        // Update Render
        app.update(&webcam.webcamImage);

        // Render Vulkan
        app.render();
    }

    // Quit GameLogic
    game.QuitApp();
    
    // Exit Vulkan
    app.ExitVulkan();

    // Destroy window and quit SDL
    SDL_DestroyWindow(window);
    SDL_Quit();
>>>>>>> src/main.cpp

    log(LOG_INFO) << "Exiting\n";
    return 0;
}