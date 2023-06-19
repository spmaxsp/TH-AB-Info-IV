#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 1
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0

#include "vulkan_base.hpp"

void VulkanBuffer::createBuffer(VmaAllocator* allocator, int size, VkBufferUsageFlags usage) {
    this->allocator = allocator;

    LOG_INIT_CERR();
    log(LOG_INFO) << "Creating Buffer with usage " << usage << "\n";

    // Create buffer and allocate memory
    VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
    bufferInfo.size = size;
    bufferInfo.usage = usage;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    vmaCreateBuffer(*allocator, &bufferInfo, &allocInfo, &Buffer, &BufferAllocation, nullptr);
    vmaMapMemory(*allocator, BufferAllocation, &BufferMemory);
}

void VulkanBuffer::uploadBufferData(VulkanCommandBuffer* commandBuffer, VkFence* fence, VulkanContext* context, void* data, int size) {
    
    // Create staging buffer
    VkBuffer stagingBuffer;
    VmaAllocation stagingBufferAllocation;
    VkBufferCreateInfo stagingBufferCreateInfo = {};
    stagingBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    stagingBufferCreateInfo.size = size;
    stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo stagingBufferAllocCreateInfo = {};
    stagingBufferAllocCreateInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

    vmaCreateBuffer(*allocator, &stagingBufferCreateInfo, &stagingBufferAllocCreateInfo, &stagingBuffer, &stagingBufferAllocation, nullptr);

    // Map staging buffer memory and copy data
    void* stagingBufferData;
    vmaMapMemory(*allocator, stagingBufferAllocation, &stagingBufferData);
    memcpy(stagingBufferData, data, size);
    vmaUnmapMemory(*allocator, stagingBufferAllocation);

    // Begin command buffer recording
    VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer->commandBuffer, &beginInfo);

    // Copy staging buffer to buffer
    VkBufferCopy copyRegion = {};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer->commandBuffer, stagingBuffer, Buffer, 1, &copyRegion);

    // End command buffer recording
    vkEndCommandBuffer(commandBuffer->commandBuffer);

    // Submit the command buffer and wait for it to complete
    VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer->commandBuffer;
    vkQueueSubmit(context->graphicsQueue.queue, 1, &submitInfo, *fence);

    // Wait for the fence and reset it
    vkWaitForFences(context->device, 1, fence, VK_TRUE, UINT64_MAX);
    vkResetFences(context->device, 1, fence);

    // Reset the command buffer
    vkResetCommandBuffer(commandBuffer->commandBuffer, 0);

    // Destroy the staging buffer
    vmaDestroyBuffer(*allocator, stagingBuffer, stagingBufferAllocation);
}

void VulkanBuffer::destroyBuffer() {
    vmaUnmapMemory(*allocator, BufferAllocation);
    vmaDestroyBuffer(*allocator, Buffer, BufferAllocation);
}

void VulkanImgBuffer::createImgBuffer(VmaAllocator* allocator, VulkanContext* context, VkExtent3D extent, VkFormat format, VkImageUsageFlags usage) {
    this->allocator = allocator;
    this->context = context;

    // Create image and allocate memory
    VkImageCreateInfo imageInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent = extent;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage = usage;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    allocInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    vmaCreateImage(*allocator, &imageInfo, &allocInfo, &Image, &ImageAllocation, nullptr);

    // Create image view
    VkImageViewCreateInfo viewInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
    viewInfo.image = Image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.layerCount = 1;

    VKA(vkCreateImageView(context->device, &viewInfo, nullptr, &ImageView));

    // Create sampler
    VkSamplerCreateInfo samplerInfo = { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 1.0f;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;

    VKA(vkCreateSampler(context->device, &samplerInfo, nullptr, &Sampler));
}

void VulkanImgBuffer::destroyImgBuffer() {
    vmaUnmapMemory(*allocator, ImageAllocation);
    vmaDestroyImage(*allocator, Image, ImageAllocation);
    vkDestroyImageView(context->device, ImageView, nullptr);
    vkDestroySampler(context->device, Sampler, nullptr);
}

void VulkanImgBuffer::uploadImgBuffer(VulkanCommandBuffer* commandBuffer, VkFence* fence, VulkanContext* context, void* data, int size, VkExtent3D extent) {
    // Create staging buffer
    VkBuffer stagingBuffer;
    VmaAllocation stagingBufferAllocation;
    VkBufferCreateInfo stagingBufferCreateInfo = {};
    stagingBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    stagingBufferCreateInfo.size = size;
    stagingBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo stagingBufferAllocCreateInfo = {};
    stagingBufferAllocCreateInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

    vmaCreateBuffer(*allocator, &stagingBufferCreateInfo, &stagingBufferAllocCreateInfo, &stagingBuffer, &stagingBufferAllocation, nullptr);

    // Map staging buffer memory and copy data
    void* stagingBufferData;
    vmaMapMemory(*allocator, stagingBufferAllocation, &stagingBufferData);
    memcpy(stagingBufferData, data, size);
    vmaUnmapMemory(*allocator, stagingBufferAllocation);

    // Begin command buffer recording
    VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer->commandBuffer, &beginInfo);

    // Transition image layout to transfer destination
    VkImageMemoryBarrier barrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = Image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    vkCmdPipelineBarrier(commandBuffer->commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    // Copy staging buffer to imagebuffer
    VkBufferImageCopy copyRegion = {};
    copyRegion.bufferOffset = 0;
    copyRegion.bufferRowLength = 0;
    copyRegion.bufferImageHeight = 0;
    copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copyRegion.imageSubresource.mipLevel = 0;
    copyRegion.imageSubresource.baseArrayLayer = 0;
    copyRegion.imageSubresource.layerCount = 1;
    copyRegion.imageOffset = { 0, 0, 0 };
    copyRegion.imageExtent = extent;
    vkCmdCopyBufferToImage(commandBuffer->commandBuffer, stagingBuffer, Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

    // Transition image layout to shader read
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    vkCmdPipelineBarrier(commandBuffer->commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    // End command buffer recording
    vkEndCommandBuffer(commandBuffer->commandBuffer);

    // Submit the command buffer and wait for it to complete
    VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer->commandBuffer;
    vkQueueSubmit(context->graphicsQueue.queue, 1, &submitInfo, *fence);

    // Wait for the fence and reset it
    vkWaitForFences(context->device, 1, fence, VK_TRUE, UINT64_MAX);
    vkResetFences(context->device, 1, fence);

    // Reset the command buffer
    vkResetCommandBuffer(commandBuffer->commandBuffer, 0);

    // Destroy the staging buffer
    vmaDestroyBuffer(*allocator, stagingBuffer, stagingBufferAllocation);
}