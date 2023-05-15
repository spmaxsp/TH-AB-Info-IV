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

class VulkanApp {
    public:
        VulkanApp(){  
        }

        void InitVulkan(SDL_Window* window);
        void ExitVulkan();

        void render();
    private:
        VulkanContext context;
        VkSurfaceKHR surface;
        VulkanSwapchain swapchain;
        VulkanRenderPass renderPass;
        VkFence fence;
        VulkanCommandBuffer commandBuffer;
};

void VulkanApp::InitVulkan(SDL_Window* window){
    LOG_INIT_CERR();

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
        const std::vector<const char*> layers = {

        };
    #endif

    // Set Device Extensions
    const std::vector<const char*> device_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    // Init Vulkan
    context.createVulkanContext(layers, extensions, device_extensions);

    // Get Surface
    VkSurfaceKHR surface;
    if (!SDL_Vulkan_CreateSurface(window, context.instance, &surface)) {
        log(LOG_ERROR) << "SDL_Vulkan_CreateSurface Error: " << SDL_GetError() << "\n";
        SDL_Quit();
    }

    // Create Swapchain
    swapchain.createSwapchain(&context, surface, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    // Create Render Pass
    renderPass.createRenderPass(&context, &swapchain);

    // Create Fence
    VkFenceCreateInfo fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
    VKA(vkCreateFence(context.device, &fenceInfo, nullptr, &fence));

    // Create Command Buffer
    commandBuffer.createCommandBuffer(&context, &swapchain, &renderPass);

}


void VulkanApp::ExitVulkan() {

    VKA(vkDeviceWaitIdle(context.device));

    VK(vkDestroyFence(context.device, fence, nullptr));

    commandBuffer.destroyCommandBuffer();

    renderPass.destroyRenderPass();
    swapchain.destroySwapchain();
    VK(vkDestroySurfaceKHR(context.instance, surface, nullptr));
    context.destroyVulkanContext();
}

void VulkanApp::render() {
    static float greenChannel = 0.0f;
	greenChannel += 0.01f;
	if (greenChannel > 1.0f) greenChannel = 0.0f;

	uint32_t imageIndex = 0;
    VKA(vkResetFences(context.device, 1, &fence));
    VKA(vkAcquireNextImageKHR(context.device, swapchain.swapchain, UINT64_MAX, VK_NULL_HANDLE, fence, &imageIndex));

    VKA(vkResetCommandPool(context.device, commandBuffer.commandPool, 0));

    VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    VKA(vkBeginCommandBuffer(commandBuffer.commandBuffer, &beginInfo));
    {
        VkClearValue clearValue = { 1.0f, greenChannel, 0.0f, 1.0f };
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass.renderPass;
        renderPassInfo.framebuffer = renderPass.framebuffers[imageIndex];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapchain.extent;
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearValue;
        vkCmdBeginRenderPass(commandBuffer.commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdEndRenderPass(commandBuffer.commandBuffer);
    }
    VKA(vkEndCommandBuffer(commandBuffer.commandBuffer));

    VKA(vkWaitForFences(context.device, 1, &fence, VK_TRUE, UINT64_MAX));
    VKA(vkResetFences(context.device, 1, &fence));

    VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer.commandBuffer;
    VKA(vkQueueSubmit(context.graphicsQueue.queue, 1, &submitInfo, fence));

    VKA(vkDeviceWaitIdle(context.device));

    VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain.swapchain;
    presentInfo.pImageIndices = &imageIndex;
    VK(vkQueuePresentKHR(context.graphicsQueue.queue, &presentInfo));
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

    // Initiate Vulkan
    VulkanApp app;
    app.InitVulkan(window);

    // run Main Loop
    while (handleEvents()) {
        app.render();
    }
    
    // Exit Vulkan
    app.ExitVulkan();

    // Destroy window and quit SDL
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}