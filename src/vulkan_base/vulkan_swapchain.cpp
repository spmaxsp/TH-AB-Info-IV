#include "vulkan_base.hpp"

void VulkanSwapchain::createSwapchain(VulkanContext* context, VkSurfaceKHR surface, VkImageUsageFlags usageFlags) {
    this->context = context;
    this->surface = surface;

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

    // Create image views 
    imageViews.resize(numSwapchainImages);
    for (uint32_t i = 0; i < numSwapchainImages; i++) {
        VkImageViewCreateInfo imageViewCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
        imageViewCreateInfo.image = images[i];
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = format;
        imageViewCreateInfo.components = { };
        imageViewCreateInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
        VKA(vkCreateImageView(context->device, &imageViewCreateInfo, nullptr, &imageViews[i]));
    }
}

void VulkanSwapchain::destroySwapchain() {
    LOG_INIT_CERR();

    log(LOG_INFO) << "Destroying Image Views\n";
    for (auto& imageView : imageViews) {
        VK(vkDestroyImageView(context->device, imageView, nullptr));
    }
    log(LOG_INFO) << "Destroying Swapchain\n";
    VK(vkDestroySwapchainKHR(context->device, swapchain, nullptr));
    log(LOG_INFO) << "Destroying Surface\n";
    VK(vkDestroySurfaceKHR(context->instance, surface, nullptr));
}

