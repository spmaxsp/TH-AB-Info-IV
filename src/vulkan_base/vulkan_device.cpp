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
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
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
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
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


bool VulkanContext::InitInstance() {
    LOG_INIT_CERR();
    // Init Vulkan instance

    // Validation layers
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
    if (!CheckLayerSupport(validationLayers)) {
        log(LOG_ERROR) << "Validation layers requested, but not available!\n";
        return false;
    }

    // Extensions
    const std::vector<const char*> extensions = {
        "VK_KHR_surface",
        "VK_KHR_win32_surface"
    };
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
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    // Create instance
    if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        return false;
    }
    return true;
}

bool VulkanContext::selectPhysicalDevice() {
    // Select physical device
    LOG_INIT_CERR();

    // Get list of available devices
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        log(LOG_ERROR) << "Failed to find GPUs with Vulkan support!\n";
        return false;
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // Print list of available devices
    log(LOG_INFO) << "Found " << deviceCount << " devices\n";
    for (const auto& device : devices) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        log(LOG_INFO) << "Device: " << deviceProperties.deviceName << "\n";
    }

    // Select first device
    physicalDevice = devices[0];
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    log(LOG_INFO) << "Selected device: " << deviceProperties.deviceName << "\n";

    return true;
}

VulkanContext::VulkanContext() {
    LOG_INIT_CERR();

    // Create Vulkan instance
    log(LOG_INFO) << "Initializing Vulkan Instance\n";
    if (!InitInstance()) {
        log(LOG_ERROR) << "Failed to init Vulkan instance\n";
    }

    // Select physical device
    log(LOG_INFO) << "Selecting physical device\n";
    if (!selectPhysicalDevice()) {
        log(LOG_ERROR) << "Failed to select physical device\n";
    }
}   