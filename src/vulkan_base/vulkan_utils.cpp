#include "vulkan_base.hpp"

void VulkanBuffer::createBuffer(VmaAllocator* allocator, int size, VkBufferUsageFlags usage) {
    this->allocator = allocator;

    VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
    bufferInfo.size = size;
    bufferInfo.usage = usage;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    vmaCreateBuffer(*allocator, &bufferInfo, &allocInfo, &Buffer, &BufferAllocation, nullptr);
    vmaMapMemory(*allocator, BufferAllocation, &BufferMemory);
}

void VulkanBuffer::destroyBuffer() {
    vmaUnmapMemory(*allocator, BufferAllocation);
    vmaDestroyBuffer(*allocator, Buffer, BufferAllocation);
}

void VulkanImgBuffer::createImgBuffer(VmaAllocator* allocator, VulkanContext* context, VkExtent3D extent, VkFormat format, VkImageUsageFlags usage) {
    this->allocator = allocator;
    this->context = context;

    VkImageCreateInfo imageInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent = extent;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage = usage;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    vmaCreateImage(*allocator, &imageInfo, &allocInfo, &Image, &ImageAllocation, nullptr);

    VkImageViewCreateInfo viewInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
    viewInfo.image = Image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.layerCount = 1;

    VKA(vkCreateImageView(context->device, &viewInfo, nullptr, &ImageView));

    VkSamplerCreateInfo samplerInfo = { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 1.0f;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;

    VKA(vkCreateSampler(context->device, &samplerInfo, nullptr, &Sampler));

    vmaMapMemory(*allocator, ImageAllocation, &ImageMemory);
}