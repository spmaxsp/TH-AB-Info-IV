#pragma once

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_vulkan.h>

#include <SDL.h>

#include <vulkan/vulkan.h>

#include "../vulkan_base/vulkan_base.hpp"

#include "../gamelogic.hpp"

class ImguiUI {
public:
    void init(SDL_Window* window, VulkanContext* context, VulkanRenderPass* renderPass, VkDescriptorPool* imguiDescriptorPool);
    void loadFonts(VulkanContext* context, VkFence* copyFence, VulkanCommandBuffer* copyCommandBuffer);
    void loadStyle();

    void supplyWebcamTexture(VulkanImgBuffer* webcamBuffer){
        webcamImageGUI = ImGui_ImplVulkan_AddTexture(webcamBuffer->Sampler, webcamBuffer->ImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }

    void newFrame();
    void render(VkCommandBuffer* commandBuffer);

    void destroy();

    ImguiUI(GameLogic* gameLogic) : gameLogic(gameLogic) {
    }

private:
    void mainGame();
    void mainMenue();
    void errorDialog();
    void pythonError();
    void pauseMenue();
    void settingsMenue();

    void DrawCrosshair(ImDrawList* drawList, float x, float y, float size, ImColor color);

    VkDescriptorSet webcamImageGUI;

    GameLogic* gameLogic;
};