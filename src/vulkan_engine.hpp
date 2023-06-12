#pragma once

#include "vulkan_base/vulkan_base.hpp"
#include "imgui_ui/imgui_ui.hpp"

#include "gamestate.hpp"

class VulkanEngine {
    public:
        VulkanEngine(){  
        }

        void InitVulkan(SDL_Window* window, ImguiUI* imguiUI);
        void ExitVulkan();

        void update(cv::Mat* image);
        
        void render();
    private:
        static const int MAX_FRAMES_IN_FLIGHT = 2;

        ImguiUI* imguiUI;

        VulkanContext context;
        VkSurfaceKHR surface;
        VulkanSwapchain swapchain;
        VulkanRenderPass renderPass;
        VulkanPipeline pipeline;

        VkFence copyFence;
        VulkanCommandBuffer copyCommandBuffer;

        VkFence commandBufferFence;
        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        std::vector<VulkanCommandBuffer> commandBuffers;

        VmaAllocator allocator;
        VulkanBuffer vertexBuffer;
        VulkanBuffer indexBuffer;
        VulkanImgBuffer webcamBuffer;

        VkDescriptorPool descriptorPool;
        VkDescriptorSet descriptorSet;
        VkDescriptorSetLayout descriptorSetLayout;

        VkDescriptorPool imguiDescriptorPool;
};
