#pragma once

#include "../BSlogger.hpp"
#include <vulkan/vulkan.h>
#include <cassert>

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

        VulkanContext(std::vector<const char*> layers, std::vector<const char*> extensions, std::vector<const char*> device_extensions);
        ~VulkanContext();
};

class VulkanSwapchain {
    public:
        VulkanContext* context;
        VkSurfaceKHR surface;

        VkSwapchainKHR swapchain;
        VkExtent2D extent;
        VkFormat format;
        std::vector<VkImage> images;

        VulkanSwapchain(VulkanContext* context, VkSurfaceKHR surface, VkImageUsageFlags usageFlags);
        ~VulkanSwapchain();
};

