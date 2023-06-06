#include "vulkan_engine.hpp"

float vertexData[] = {
	0.5f, -0.5f,		// Position
	1.0f, 0.0f, 0.0f,	// Color
	1.0f, 0.0f,			// Texcoord

	0.5f, 0.5f,
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f,

	-0.5f, 0.5f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f,

	-0.5f, -0.5f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f,
};

uint32_t indexData[] = {
	0, 1, 2,
	3, 0, 2,
};

void VulkanEngine::InitVulkan(SDL_Window* window, ImguiUI* imguiUI) {
    LOG_INIT_CERR();

    // Check if ImguiUI is valid
    if (imguiUI == nullptr) {
        log(LOG_ERROR) << "ImguiUI is nullptr\n";
        return;
    }

    // Check if window is valid
    if (window == nullptr) {
        log(LOG_ERROR) << "Window is nullptr\n";
        return;
    }

    // Save ImguiUI reference
    this->imguiUI = imguiUI;

    // Get Instance Extensions
    uint32_t extensionCount = 0;
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
    std::vector<const char*> extensions(extensionCount);
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensions.data());

    // Set Instance Layers
    #ifdef _DEBUG
        const std::vector<const char*> layers = {
            "VK_LAYER_KHRONOS_validation"
        };
    #else
        const std::vector<const char*> layers = {

        };
    #endif

    // Set Device Extensions
    const std::vector<const char*> device_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    // Init Vulkan
    context.createVulkanContext(layers, extensions, device_extensions);

    // Get Surface
    VkSurfaceKHR surface;
    if (!SDL_Vulkan_CreateSurface(window, context.instance, &surface)) {
        log(LOG_ERROR) << "SDL_Vulkan_CreateSurface Error: " << SDL_GetError() << "\n";
        SDL_Quit();
    }

    // Create Swapchain
    swapchain.createSwapchain(&context, surface, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);

    // Create Render Pass
    renderPass.createRenderPass(&context, &swapchain);

    // Create Fences
    VkFenceCreateInfo fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    VKA(vkCreateFence(context.device, &fenceInfo, nullptr, &commandBufferFence));

    fenceInfo.flags = 0;
    VKA(vkCreateFence(context.device, &fenceInfo, nullptr, &copyFence));

    // Create Semaphores
    VkSemaphoreCreateInfo semaphoreInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
    VKA(vkCreateSemaphore(context.device, &semaphoreInfo, nullptr, &imageAvailableSemaphore));
    VKA(vkCreateSemaphore(context.device, &semaphoreInfo, nullptr, &renderFinishedSemaphore));

    // Create Command Buffers/Pools
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for (auto& commandBuffer : commandBuffers){
        commandBuffer.createCommandBuffer(&context, &swapchain, &renderPass, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
    }

    // Create Copy Command Buffer/Pool
    copyCommandBuffer.createCommandBuffer(&context, &swapchain, &renderPass, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

    // Create Allocator
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.physicalDevice = context.physicalDevice;
    allocatorInfo.device = context.device;
    allocatorInfo.instance = context.instance;
    vmaCreateAllocator(&allocatorInfo, &allocator);

    // Create Vertex Buffer
    vertexBuffer.createBuffer(&allocator, sizeof(vertexData), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);

    // Create Index Buffer
    indexBuffer.createBuffer(&allocator, sizeof(indexData), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);

    // Copy Vertex Data
    vertexBuffer.uploadBufferData(&copyCommandBuffer, &copyFence, &context, vertexData, sizeof(vertexData));

    // Copy Index Data
    indexBuffer.uploadBufferData(&copyCommandBuffer, &copyFence, &context, indexData, sizeof(indexData));

    // Create Image Buffer
    webcamBuffer.createImgBuffer(&allocator, &context, { 640, 480, 1 }, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);

    // Create Descriptor Set
    VkDescriptorPoolSize poolSizes[] = {
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 }
    };
    VkDescriptorPoolCreateInfo descriptorPoolInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
    descriptorPoolInfo.maxSets = 1;
    descriptorPoolInfo.poolSizeCount = ARRAY_SIZE(poolSizes);
    descriptorPoolInfo.pPoolSizes = poolSizes;
    VKA(vkCreateDescriptorPool(context.device, &descriptorPoolInfo, nullptr, &descriptorPool));

    VkDescriptorSetLayoutBinding layoutBindings[] = {
        { 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }
    };
    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
    descriptorSetLayoutInfo.bindingCount = ARRAY_SIZE(layoutBindings);
    descriptorSetLayoutInfo.pBindings = layoutBindings;
    VKA(vkCreateDescriptorSetLayout(context.device, &descriptorSetLayoutInfo, nullptr, &descriptorSetLayout));
    
    VkDescriptorSetAllocateInfo descriptorSetAllocInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
    descriptorSetAllocInfo.descriptorPool = descriptorPool;
    descriptorSetAllocInfo.descriptorSetCount = 1;
    descriptorSetAllocInfo.pSetLayouts = &descriptorSetLayout;
    VKA(vkAllocateDescriptorSets(context.device, &descriptorSetAllocInfo, &descriptorSet));

    // Update Descriptor Set
    VkDescriptorImageInfo imageInfo = {};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = webcamBuffer.ImageView;
    imageInfo.sampler = webcamBuffer.Sampler;

    VkWriteDescriptorSet descriptorWrites[1];
    descriptorWrites[0] = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
    descriptorWrites[0].dstSet = descriptorSet;
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[0].pImageInfo = &imageInfo;
    VK(vkUpdateDescriptorSets(context.device, ARRAY_SIZE(descriptorWrites), descriptorWrites, 0, nullptr));

    // Create ImGui Descriptor Set
    VkDescriptorPoolSize imguiPoolSizes[] = {
        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
        { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };

    VkDescriptorPoolCreateInfo imguiDescriptorPoolInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
    imguiDescriptorPoolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    imguiDescriptorPoolInfo.maxSets = 1000 * ARRAY_SIZE(imguiPoolSizes);
    imguiDescriptorPoolInfo.poolSizeCount = ARRAY_SIZE(imguiPoolSizes);
    imguiDescriptorPoolInfo.pPoolSizes = imguiPoolSizes;
    VKA(vkCreateDescriptorPool(context.device, &imguiDescriptorPoolInfo, nullptr, &imguiDescriptorPool));

    // Init ImGui
    imguiUI->init(window, &context, &renderPass, &imguiDescriptorPool);

    // Imgui Style
    imguiUI->loadStyle();

    // Upload ImGui Fonts
    imguiUI->loadFonts(&context, &copyFence, &copyCommandBuffer);

    // Create Pipeline
    VkVertexInputAttributeDescription vertexAttributeDescriptions[3] = {};
	vertexAttributeDescriptions[0].binding = 0;
	vertexAttributeDescriptions[0].location = 0;
	vertexAttributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
	vertexAttributeDescriptions[0].offset = 0;
	vertexAttributeDescriptions[1].binding = 0;
	vertexAttributeDescriptions[1].location = 1;
	vertexAttributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	vertexAttributeDescriptions[1].offset = sizeof(float) * 2;
    vertexAttributeDescriptions[2].binding = 0;
	vertexAttributeDescriptions[2].location = 2;
	vertexAttributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
	vertexAttributeDescriptions[2].offset = sizeof(float) * 5;
	VkVertexInputBindingDescription vertexInputBinding = {};
	vertexInputBinding.binding = 0;
	vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	vertexInputBinding.stride = sizeof(float) * 7;
    pipeline.createPipeline(&context, &renderPass, "./shaders/texture_vert.spv", "./shaders/texture_frag.spv", swapchain.extent, vertexAttributeDescriptions, 3, &vertexInputBinding, &descriptorSetLayout, 1);
}

void VulkanEngine::ExitVulkan() {

    // Wait for device to be idle
    VKA(vkDeviceWaitIdle(context.device));

    // Destroy Pipeline
    pipeline.destroyPipeline();

    // Destroy ImGui
    imguiUI->destroy();

    // Destroy imgui descriptor pool
    vkDestroyDescriptorPool(context.device, imguiDescriptorPool, nullptr);

    // Destroy descriptor set
    vkDestroyDescriptorSetLayout(context.device, descriptorSetLayout, nullptr);

    // Destroy image buffer
    webcamBuffer.destroyImgBuffer();

    // Destroy vertex buffer
    vertexBuffer.destroyBuffer();

    // Destroy index buffer
    indexBuffer.destroyBuffer();

    // Destroy allocator
    vmaDestroyAllocator(allocator);

    // Destroy copy command buffer
    copyCommandBuffer.destroyCommandBuffer();

    // Destroy command buffers
    for (auto& commandBuffer : commandBuffers) {
        commandBuffer.destroyCommandBuffer();
    }

    // Destroy semaphores
    VK(vkDestroySemaphore(context.device, renderFinishedSemaphore, nullptr));
    VK(vkDestroySemaphore(context.device, imageAvailableSemaphore, nullptr));
    VK(vkDestroyFence(context.device, commandBufferFence, nullptr));
    VK(vkDestroyFence(context.device, copyFence, nullptr));

    // Destroy render pass
    renderPass.destroyRenderPass();

    // Destroy swapchain
    swapchain.destroySwapchain();

    // Destroy surface
    context.destroyVulkanContext();
}

void VulkanEngine::update(gamestate gstate, bool new_webcam, cv::Mat* image){
    // Update ImGui
    imguiUI->newFrame();

    // Copy image data to buffer
    webcamBuffer.uploadImgBuffer(&copyCommandBuffer, &copyFence, &context, image->data, image->cols * image->rows * 4, { 640, 480, 1 });

    //Wait for command buffer to finish
    VKA(vkWaitForFences(context.device, 1, &commandBufferFence, VK_TRUE, UINT64_MAX));
    VKA(vkResetFences(context.device, 1, &commandBufferFence));
}

void VulkanEngine::render() {
    LOG_INIT_CERR();

	uint32_t imageIndex = 0;

    // Select command buffer
    static uint32_t bufferIndex = 0;
    VulkanCommandBuffer& commandBuffer = commandBuffers[bufferIndex];

    // Acquire next image and check if swapchain is out of date
    VkResult result = VK(vkAcquireNextImageKHR(context.device, swapchain.swapchain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex));
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        if (swapchain.recreateSwapchain(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)) {
            renderPass.recreateRenderPass();
        }
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        log(LOG_ERROR) << "Failed to acquire swapchain image\n";
        return;
    }

    // Reset command pool
    VKA(vkResetCommandPool(context.device, commandBuffer.commandPool, 0));


    VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    VKA(vkBeginCommandBuffer(commandBuffer.commandBuffer, &beginInfo));
    {
        // Dynamic viewport and scissor
        VkViewport viewport = { 0.0f, 0.0f, (float)swapchain.extent.width, (float)swapchain.extent.height };
        VkRect2D scissor = { {0, 0}, swapchain.extent };

        vkCmdSetViewport(commandBuffer.commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer.commandBuffer, 0, 1, &scissor);

        // Begin render pass
        VkClearValue clearValue = { 0.0f, 0.0f, 0.02f, 1.0f };
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass.renderPass;
        renderPassInfo.framebuffer = renderPass.framebuffers[imageIndex];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapchain.extent;
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearValue;
        vkCmdBeginRenderPass(commandBuffer.commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        // Bind pipeline
        vkCmdBindPipeline(commandBuffer.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.graphicsPipeline);
        
        // Bind vertex buffer and index buffer and descriptor set
        VkDeviceSize offset = 0;
        vkCmdBindVertexBuffers(commandBuffer.commandBuffer, 0, 1, &vertexBuffer.Buffer, &offset);
        vkCmdBindIndexBuffer(commandBuffer.commandBuffer, indexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(commandBuffer.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

        // Draw Vulkan stuff
        vkCmdDrawIndexed(commandBuffer.commandBuffer, 6, 1, 0, 0, 0);

        // Draw ImGui stuff
        imguiUI->render(&commandBuffer.commandBuffer);

        // End render pass
        vkCmdEndRenderPass(commandBuffer.commandBuffer);
    }
    VKA(vkEndCommandBuffer(commandBuffer.commandBuffer));

    // Submit command buffer
    VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer.commandBuffer;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageAvailableSemaphore;    // Wait for an image to render to be available
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderFinishedSemaphore;  // Signal when rendering has finished
    VKA(vkQueueSubmit(context.graphicsQueue.queue, 1, &submitInfo, commandBufferFence)); // Submit command buffer

    // Present swapchain image
    VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain.swapchain;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderFinishedSemaphore;  // Wait for rendering to be finished
    result = VK(vkQueuePresentKHR(context.graphicsQueue.queue, &presentInfo));
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        if (swapchain.recreateSwapchain(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)) {
            renderPass.recreateRenderPass();
        }
    }
    else if (result != VK_SUCCESS) {
        log(LOG_ERROR) << "Failed to present swapchain image\n";
    }

    bufferIndex = (bufferIndex + 1) % MAX_FRAMES_IN_FLIGHT;
}