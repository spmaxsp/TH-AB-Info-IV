#include <stdlib.h>
#include <string.h>

#include "vulkan_base.hpp"


bool VulkanContext::CheckLayerSupport(std::vector<const char*> layers) {
    // Check if validation layers are supported
    LOG_INIT_CERR();
    #ifdef NDEBUG
        log.set_log_level(LOG_DEBUG);
    #else
        log.set_log_level(LOG_INFO);
    #endif

    // Get list of available layers
    uint32_t layerCount;
    VKA(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));
    std::vector<VkLayerProperties> availableLayers(layerCount);
    VKA(vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()));
    log(LOG_DEBUG) << "Available layers:\n";
    for (const auto& layerProperties : availableLayers) {
        log(LOG_DEBUG) << "Available layer: " << layerProperties.layerName << "\n";
    }
    
    // Go through requested layers and check if they are available
    for (const char* layerName : layers) {
        bool layerFound = false;
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                log(LOG_INFO) << "Layer " << layerName << " found!\n";
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            log(LOG_ERROR) << "Layer " << layerName << " not found!\n";
            return false;
        }
    }
    return true;
}

bool VulkanContext::CheckExtensionSupport(std::vector<const char*> extensions) {
    // Check if extensions are supported
    LOG_INIT_CERR();
    #ifdef _DEBUG
        log.set_log_level(LOG_DEBUG);
    #else
        log.set_log_level(LOG_INFO);
    #endif

    // Get list of available extensions
    uint32_t extensionCount = 0;
    VKA(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr));
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    VKA(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data()));
    log(LOG_DEBUG) << "Available extensions:\n";
    for (const auto& extensionProperties : availableExtensions) {
        log(LOG_DEBUG) << "Available extension: " << extensionProperties.extensionName << "\n";
    }

    // Go through requested extensions and check if they are available
    for (const char* extensionName : extensions) {
        bool extensionFound = false;
        for (const auto& extensionProperties : availableExtensions) {
            if (strcmp(extensionName, extensionProperties.extensionName) == 0) {
                log(LOG_INFO) << "Extension " << extensionName << " found!\n";
                extensionFound = true;
                break;
            }
        }
        if (!extensionFound) {
            log(LOG_ERROR) << "Extension " << extensionName << " not found!\n";
            return false;
        }
    }
    return true;
}


bool VulkanContext::InitInstance(std::vector<const char*> layers, std::vector<const char*> extensions) {
    LOG_INIT_CERR();
    // Init Vulkan instance

    // Validation layers
    if (!CheckLayerSupport(layers)) {
        log(LOG_ERROR) << "Validation layers requested, but not available!\n";
        return false;
    }

    // Extensions
    if (!CheckExtensionSupport(extensions)) {
        log(LOG_ERROR) << "Extensions requested, but not available!\n";
        return false;
    }

    // Application info
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Base";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Instance info
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
    createInfo.ppEnabledLayerNames = layers.data();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    // Create instance
    if(VK(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)) {
        return false;
    }
    return true;
}

bool VulkanContext::selectPhysicalDevice() {
    // Select physical device
    LOG_INIT_CERR();

    // Get list of available devices
    uint32_t deviceCount = 0;
    VKA(vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr));
    if (deviceCount == 0) {
        log(LOG_ERROR) << "Failed to find GPUs with Vulkan support!\n";
        return false;
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    VKA(vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data()));

    // Print list of available devices
    log(LOG_INFO) << "Found " << deviceCount << " devices\n";
    for (const auto& device : devices) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        log(LOG_INFO) << "Device: " << deviceProperties.deviceName << "\n";
    }

    // Select first device
    physicalDevice = devices[0];
    vkGetPhysicalDeviceProperties(physicalDevice, &selectedDeviceProperties);
    log(LOG_INFO) << "Selected device: " << selectedDeviceProperties.deviceName << "\n";

    return true;
}

bool VulkanContext::createLogicalDevice(std::vector<const char*> device_extensions) {
    // Get queue families
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    // Find queue family with graphics support
    uint32_t graphicsQueueIndex = 0;
    for (uint32_t i = 0; i < queueFamilies.size(); i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsQueueIndex = i;
            break;
        }
    }

    // Create queue info
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
    queueCreateInfo.queueFamilyIndex = graphicsQueueIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    // Get Physical Device Features
    VkPhysicalDeviceFeatures enabledFeatures = {};

    // Create logical device info
    VkDeviceCreateInfo deviceCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.enabledExtensionCount = device_extensions.size();
    deviceCreateInfo.ppEnabledExtensionNames = device_extensions.data();
    deviceCreateInfo.pEnabledFeatures = &enabledFeatures;

    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
        return false;
    }

    // Get graphics queue
    graphicsQueue.familyIndex = graphicsQueueIndex;
    VK(vkGetDeviceQueue(device, graphicsQueueIndex, 0, &graphicsQueue.queue));

    return true;
}

void VulkanContext::createVulkanContext(std::vector<const char*> layers, std::vector<const char*> extensions, std::vector<const char*> device_extensions) {
    LOG_INIT_CERR();

    // Create Vulkan instance
    log(LOG_INFO) << "Initializing Vulkan Instance\n";
    if (!InitInstance(layers, extensions)) {
        log(LOG_ERROR) << "Failed to init Vulkan instance\n";
    }

    // Select physical device
    log(LOG_INFO) << "Selecting physical device\n";
    if (!selectPhysicalDevice()) {
        log(LOG_ERROR) << "Failed to select physical device\n";
    }

    // Create logical device
    log(LOG_INFO) << "Creating logical device\n";
    if (!createLogicalDevice(device_extensions)) {
        log(LOG_ERROR) << "Failed to create logical device\n";
    }
}   

void VulkanContext::destroyVulkanContext() {
    LOG_INIT_CERR();

    log(LOG_INFO) << "Exiting Vulkan\n";
    VKA(vkDeviceWaitIdle(device));
    VK(vkDestroyDevice(device, nullptr));
    VK(vkDestroyInstance(instance, nullptr));
}