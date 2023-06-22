#pragma once

#include <SDL.h>
#include <SDL_vulkan.h>

#include <vulkan/vulkan.h>

#include <opencv2/opencv.hpp>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_vulkan.h>

#include <vk_mem_alloc.h>

#include <BSlogger.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#define ASSERT_VULKAN(val) if(val != VK_SUCCESS) {assert(false);}
#ifndef VK
#define VK(f) (f)
#endif
#ifndef VKA
#define VKA(f) ASSERT_VULKAN(VK(f))
#endif

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

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
        VkSwapchainKHR oldSwapchain;
        VkExtent2D extent;
        VkFormat format;
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;

        void createSwapchain(VulkanContext* context, VkSurfaceKHR surface, VkImageUsageFlags usageFlags);
        void destroySwapchain();

        int recreateSwapchain(VkImageUsageFlags usageFlags);

        VulkanSwapchain(){ 
            context = nullptr; 
            surface = VK_NULL_HANDLE; 
            swapchain = VK_NULL_HANDLE;
            oldSwapchain = VK_NULL_HANDLE; 
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

        void recreateRenderPass();

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

        void createCommandBuffer(VulkanContext* context, VulkanSwapchain* swapchain, VulkanRenderPass* renderPass, VkCommandPoolCreateFlags flags);
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

        void createPipeline(VulkanContext* context, VulkanRenderPass* renderPass, const char* vertexShaderFilename, const char* fragmentShaderFilename, VkExtent2D extent, VkVertexInputAttributeDescription* attributes, uint32_t numAttributes, VkVertexInputBindingDescription* binding, VkDescriptorSetLayout* SetLayouts, uint32_t setLayoutCount);
        void destroyPipeline();

        VulkanPipeline(){ 
            context = nullptr; 
            renderPass = nullptr; 
            pipelineLayout = VK_NULL_HANDLE; 
            graphicsPipeline = VK_NULL_HANDLE;
        }
};

class VulkanBuffer {
    private:
        VmaAllocator* allocator;
        
    public:
        VkBuffer Buffer;
        VmaAllocation BufferAllocation;
        void* BufferMemory;

        void createBuffer(VmaAllocator* allocator, int size, VkBufferUsageFlags usage);
        void uploadBufferData(VulkanCommandBuffer* commandBuffer, VkFence* fence, VulkanContext* context, void* data, int size);
        void destroyBuffer();

        VulkanBuffer(){
            allocator = VK_NULL_HANDLE; 
            Buffer = VK_NULL_HANDLE; 
            BufferAllocation = VK_NULL_HANDLE; 
            BufferMemory = nullptr;            
        }
};

class VulkanImgBuffer {
    private:
        VmaAllocator* allocator;
        VulkanContext* context;
        void* ImageMemory;

    public:	
        VkImage Image;
        VkImageView ImageView;
        VmaAllocation ImageAllocation;
        VkSampler Sampler;

        void createImgBuffer(VmaAllocator* allocator, VulkanContext* context, VkExtent3D extent, VkFormat format, VkImageUsageFlags usage);
        void destroyImgBuffer();
        void uploadImgBuffer(VulkanCommandBuffer* commandBuffer, VkFence* fence, VulkanContext* context, void* data, int size, VkExtent3D extent);

        VulkanImgBuffer(){ 
            allocator = VK_NULL_HANDLE; 
            Image = VK_NULL_HANDLE; 
            ImageAllocation = VK_NULL_HANDLE; 
        }
};
