#include <stdlib.h>

#include "vulkan_engine.hpp"

#include "gamestate.hpp"

#include <BSlogger.hpp>

#include "py_extention/PyShellExec.hpp"

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

    // Init Game State
    gamestate gstate = {};

    // Init ImGui
    ImguiUI imgui;

    // Initiate Vulkan
    VulkanEngine app;
    app.InitVulkan(window, &imgui);

    cv::VideoCapture cap(0);
    cv::Mat webcamImage;

    // run Main Loop
    while (handleEvents()) {
        // Capture webcam image using OpenCV
        cap.read(webcamImage);

        // Convert the image format if necessary (e.g., from BGR to RGBA)
        cv::Mat convertedImage;
        cv::cvtColor(webcamImage, convertedImage, cv::COLOR_BGR2RGBA);

        // Update Game State
        app.update(gstate, true, &convertedImage);

        // Render Vulkan
        app.render();
    }
    
    // Exit Vulkan
    app.ExitVulkan();

    // Destroy window and quit SDL
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}