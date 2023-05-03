#pragma once

#include "../BSlogger.hpp"
#include <vulkan/vulkan.h>

class VulkanContext {
    private:
        VkInstance instance;
        VkPhysicalDevice physicalDevice;

        bool CheckLayerSupport(std::vector<const char*> layers);
        bool CheckExtensionSupport(std::vector<const char*> extensions);

        bool InitInstance();
        bool selectPhysicalDevice();
    public:
        VulkanContext();
};
