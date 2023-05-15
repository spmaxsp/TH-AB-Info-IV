#include "vulkan_base.hpp"

void VulkanCommandBuffer::createCommandBuffer(VulkanContext* context, VulkanSwapchain* swapchain, VulkanRenderPass* renderPass){
    LOG_INIT_CERR();

    this->context = context;
    this->swapchain = swapchain;
    this->renderPass = renderPass;

    // Create Command Pool
    log(LOG_INFO) << "Creating Command Pool and Buffers" << "\n	";
    VkCommandPoolCreateInfo poolInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
    poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    poolInfo.queueFamilyIndex = context->graphicsQueue.familyIndex;
    VKA(vkCreateCommandPool(context->device, &poolInfo, nullptr, &commandPool));

    // Create Command Buffer
    VkCommandBufferAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;
    VKA(vkAllocateCommandBuffers(context->device, &allocInfo, &commandBuffer));
}

void VulkanCommandBuffer::destroyCommandBuffer(){
    LOG_INIT_CERR();

    // Destroy Command Pool
    log(LOG_INFO) << "Destroying Command Pool and Buffers" << "\n	";
    VK(vkFreeCommandBuffers(context->device, commandPool, 1, &commandBuffer));
    VK(vkDestroyCommandPool(context->device, commandPool, nullptr));
}
