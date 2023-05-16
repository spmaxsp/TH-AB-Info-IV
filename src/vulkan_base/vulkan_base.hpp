#pragma once

#include "../BSlogger.hpp"
#include <vulkan/vulkan.h>
#include <cassert>
#include <fstream>

#define ASSERT_VULKAN(val) if(val != VK_SUCCESS) {assert(false);}
#ifndef VK
#define VK(f) (f)
#endif
#ifndef VKA
#define VKA(f) ASSERT_VULKAN(VK(f))
#endif

struct VulkanQueue {
    VkQueue queue;
    uint32_t familyIndex;
};

class VulkanContext {
    private:
        bool CheckLayerSupport(std::vector<const char*> layers);
        bool CheckExtensionSupport(std::vector<const char*> extensions);

        bool InitInstance(std::vector<const char*> layers, std::vector<const char*> extensions);

        bool selectPhysicalDevice();
        bool createLogicalDevice(std::vector<const char*> device_extensions);
    public:
        VkInstance instance;
        VkPhysicalDevice physicalDevice;
        VkPhysicalDeviceProperties selectedDeviceProperties;
        VkDevice device;
        VulkanQueue graphicsQueue;

        void createVulkanContext(std::vector<const char*> layers, std::vector<const char*> extensions, std::vector<const char*> device_extensions);
        void destroyVulkanContext();

        VulkanContext(){ 
            instance = VK_NULL_HANDLE; 
            physicalDevice = VK_NULL_HANDLE; 
            selectedDeviceProperties = {};
            device = VK_NULL_HANDLE; 
            graphicsQueue = {};
        }
};

class VulkanSwapchain {
    private:
        VulkanContext* context;
        VkSurfaceKHR surface;

    public:
        VkSwapchainKHR swapchain;
        VkExtent2D extent;
        VkFormat format;
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;

        void createSwapchain(VulkanContext* context, VkSurfaceKHR surface, VkImageUsageFlags usageFlags);
        void destroySwapchain();

        VulkanSwapchain(){ 
            context = nullptr; 
            surface = VK_NULL_HANDLE; 
            swapchain = VK_NULL_HANDLE; 
            extent = {0,0}; 
            format = VK_FORMAT_UNDEFINED; 
            images = {};
            imageViews = {};
        }
};

class VulkanRenderPass {
    private:
        VulkanContext* context;
        VulkanSwapchain* swapchain;

    public:
        VkRenderPass renderPass;
        std::vector<VkFramebuffer> framebuffers;

        void createRenderPass(VulkanContext* context, VulkanSwapchain* swapchain);
        void destroyRenderPass();

        VulkanRenderPass(){ 
            context = nullptr; 
            swapchain = nullptr; 
            renderPass = VK_NULL_HANDLE; 
            framebuffers = {};
        }
};


class VulkanCommandBuffer {
    private:
        VulkanContext* context;
        VulkanSwapchain* swapchain;
        VulkanRenderPass* renderPass;

    public:
        VkCommandPool commandPool;
        VkCommandBuffer commandBuffer;

        void createCommandBuffer(VulkanContext* context, VulkanSwapchain* swapchain, VulkanRenderPass* renderPass);
        void destroyCommandBuffer();

        void beginCommandBuffer();
        void endCommandBuffer();

        void resetCommandPool();	

        VulkanCommandBuffer(){ 
            context = nullptr; 
            swapchain = nullptr; 
            renderPass = nullptr; 
            commandPool = VK_NULL_HANDLE; 
            commandBuffer = VK_NULL_HANDLE;
        }
};


class VulkanPipeline {
    private:
        VulkanContext* context;
        VulkanRenderPass* renderPass;

        VkShaderModule createShaderModule(const char* filename);
    public:
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;

        void createPipeline(VulkanContext* context, VulkanRenderPass* renderPass, const char* vertexShaderFilename, const char* fragmentShaderFilename, VkExtent2D extent);
        void destroyPipeline();

        VulkanPipeline(){ 
            context = nullptr; 
            renderPass = nullptr; 
            pipelineLayout = VK_NULL_HANDLE; 
            graphicsPipeline = VK_NULL_HANDLE;
        }
};


