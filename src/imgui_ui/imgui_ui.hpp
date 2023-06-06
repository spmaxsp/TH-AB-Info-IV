#pragma once

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_vulkan.h>

#include <SDL.h>

#include <vulkan/vulkan.h>

#include "../vulkan_base/vulkan_base.hpp"

class ImguiUI {
public:
    void init(SDL_Window* window, VulkanContext* context, VulkanRenderPass* renderPass, VkDescriptorPool* imguiDescriptorPool);
    void loadFonts(VulkanContext* context, VkFence* copyFence, VulkanCommandBuffer* copyCommandBuffer);
    void loadStyle();

    void newFrame();
    void render(VkCommandBuffer* commandBuffer);

    void destroy();
};