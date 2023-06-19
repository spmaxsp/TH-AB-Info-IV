#include <stdlib.h>

#include "vulkan_engine.hpp"
#include "imgui_ui/imgui_ui.hpp"

#include "gamelogic.hpp"

//#include "py_extention/modules/ModuleTemplate/PyModule.hpp"
#include "py_extention/modules/EEG/EEG.hpp"
#include "py_extention/modules/Shimmersensor/Shimmersensor.hpp"
#include "py_extention/modules/Movinghead/Movinghead.hpp"

#include <BSlogger.hpp>

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
    #define Sleep(x) usleep((x)*1000)
#endif

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
    EEG eeg;
    Movinghead movinghead;

    // Init Webcam
    Webcam webcam(1280, 720);

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

    log(LOG_INFO) << "Exiting\n";
    return 0;
}