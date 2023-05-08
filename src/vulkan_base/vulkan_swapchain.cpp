#include "vulkan_base.hpp"

VulkanSwapchain::VulkanSwapchain(VulkanContext* context, VkSurfaceKHR surface, VkImageUsageFlags usageFlags) : context(context), surface(surface) {
    LOG_INIT_CERR();
    log(LOG_INFO) << "Creating Swapchain\n";

    swapchain = {};

    // Queue supports present
    VkBool32 surfaceSupport = false;
    VKA(vkGetPhysicalDeviceSurfaceSupportKHR(context->physicalDevice, context->graphicsQueue.familyIndex, surface, &surfaceSupport));
    if (!surfaceSupport) {
        log(LOG_ERROR) << "Selected queue does not support present!\n";
    }

    // Get surface capabilities
    uint32_t numFormats = 0;
    VKA(vkGetPhysicalDeviceSurfaceFormatsKHR(context->physicalDevice, surface, &numFormats, nullptr));
    std::vector<VkSurfaceFormatKHR> surfaceFormats(numFormats);
    VKA(vkGetPhysicalDeviceSurfaceFormatsKHR(context->physicalDevice, surface, &numFormats, surfaceFormats.data()));
    if (numFormats == 0) {
        log(LOG_ERROR) << "Surface has no formats!\n";
    }

    // Take first format
    format = surfaceFormats[0].format;
    VkColorSpaceKHR colorSpace = surfaceFormats[0].colorSpace;

    // Get surface capabilities
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    VKA(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(context->physicalDevice, surface, &surfaceCapabilities));

    if (surfaceCapabilities.currentExtent.width == 0xFFFFFFFF) {
        log(LOG_ERROR) << "Surface has no extent! -> override with minImageExtent\n";
        surfaceCapabilities.currentExtent = surfaceCapabilities.minImageExtent;
    }
    if (surfaceCapabilities.currentExtent.height == 0xFFFFFFFF) {
        log(LOG_ERROR) << "Surface has no extent! -> override with minImageExtent\n";
        surfaceCapabilities.currentExtent = surfaceCapabilities.minImageExtent;
    }
    if (surfaceCapabilities.maxImageCount == 0) {
        log(LOG_ERROR) << "Surface has no maxImageCount -> override with fixed 8\n";
        surfaceCapabilities.maxImageCount = 8;
    }

    extent = surfaceCapabilities.currentExtent;

    // Create swapchain
    VkSwapchainCreateInfoKHR swapchainCreateInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
    swapchainCreateInfo.surface = surface;
    swapchainCreateInfo.minImageCount = 3;
    swapchainCreateInfo.imageFormat = VK_FORMAT_B8G8R8A8_UNORM;
    swapchainCreateInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    swapchainCreateInfo.imageExtent = surfaceCapabilities.currentExtent;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = usageFlags;
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    VKA(vkCreateSwapchainKHR(context->device, &swapchainCreateInfo, nullptr, &swapchain));

    // Get swapchain images
    uint32_t numSwapchainImages = 0;
    VKA(vkGetSwapchainImagesKHR(context->device, swapchain, &numSwapchainImages, nullptr));

    images.resize(numSwapchainImages);
    VKA(vkGetSwapchainImagesKHR(context->device, swapchain, &numSwapchainImages, images.data()));
}

VulkanSwapchain::~VulkanSwapchain() {
    LOG_INIT_CERR();
    log(LOG_INFO) << "Destroying Swapchain\n";
    VK(vkDestroySwapchainKHR(context->device, swapchain, nullptr));
    log(LOG_INFO) << "Destroying Surface\n";
    VK(vkDestroySurfaceKHR(context->instance, surface, nullptr));
}

