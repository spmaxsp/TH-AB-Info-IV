#include <stdlib.h>

#include <SDL.h>
#include <SDL_vulkan.h>

#include <vulkan/vulkan.h>

#include <opencv2/opencv.hpp>

#include "vulkan_base/vulkan_base.hpp"

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_vulkan.h>

#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 1
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#include <vk_mem_alloc.h>

#include <BSlogger.hpp>

//#include "py_extention/PyRunner.hpp"
#include "py_extention/PyShellExec.hpp"
//#include "py_extention/modules/ModuleTemplate/PyModule.hpp"
//#include "py_extention/modules/Shimmersensor/Shimmersensor.hpp"

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
    #define Sleep(x) usleep((x)*1000)
#endif

#define _DEBUG

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

bool handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return false;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                return false;
            break;
        }
    }
    return true;
}

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

class VulkanApp {
    public:
        VulkanApp(){  
        }

        void InitVulkan(SDL_Window* window);
        void ExitVulkan();

        void render(cv::Mat&);
    private:
        static const int MAX_FRAMES_IN_FLIGHT = 2;
        VulkanContext context;
        VkSurfaceKHR surface;
        VulkanSwapchain swapchain;
        VulkanRenderPass renderPass;
        VulkanPipeline pipeline;

        VkFence copyFence;
        VulkanCommandBuffer copyCommandBuffer;

        VkFence commandBufferFence;
        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        std::vector<VulkanCommandBuffer> commandBuffers;

        VmaAllocator allocator;
        VulkanBuffer vertexBuffer;
        VulkanBuffer indexBuffer;
        VulkanImgBuffer webcamBuffer;

        VkDescriptorPool descriptorPool;
        VkDescriptorSet descriptorSet;
        VkDescriptorSetLayout descriptorSetLayout;

        VkDescriptorPool imguiDescriptorPool;
};

void VulkanApp::InitVulkan(SDL_Window* window){
    LOG_INIT_CERR();

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

    // Init ImGui
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

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForVulkan(window);
    ImGui_ImplVulkan_InitInfo imguiInitInfo = {};
    imguiInitInfo.Instance = context.instance;
    imguiInitInfo.PhysicalDevice = context.physicalDevice;
    imguiInitInfo.Device = context.device;
    imguiInitInfo.QueueFamily = context.graphicsQueue.familyIndex;
    imguiInitInfo.Queue = context.graphicsQueue.queue;
    imguiInitInfo.PipelineCache = VK_NULL_HANDLE;
    imguiInitInfo.DescriptorPool = imguiDescriptorPool;
    imguiInitInfo.Allocator = nullptr;
    imguiInitInfo.MinImageCount = 2;
    imguiInitInfo.ImageCount = 2;
    imguiInitInfo.CheckVkResultFn = nullptr;
    ImGui_ImplVulkan_Init(&imguiInitInfo, renderPass.renderPass);

    // Upload ImGui Fonts
    VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    VKA(vkBeginCommandBuffer(copyCommandBuffer.commandBuffer, &beginInfo));
    ImGui_ImplVulkan_CreateFontsTexture(copyCommandBuffer.commandBuffer);
    VKA(vkEndCommandBuffer(copyCommandBuffer.commandBuffer));
    VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &copyCommandBuffer.commandBuffer;
    VKA(vkQueueSubmit(context.graphicsQueue.queue, 1, &submitInfo, copyFence));
    VKA(vkWaitForFences(context.device, 1, &copyFence, VK_TRUE, UINT64_MAX));
    vkResetFences(context.device, 1, &copyFence);
    ImGui_ImplVulkan_DestroyFontUploadObjects();

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


void VulkanApp::ExitVulkan() {

    // Wait for device to be idle
    VKA(vkDeviceWaitIdle(context.device));

    // Destroy image buffer
    webcamBuffer.destroyImgBuffer();

    // Destroy vertex buffer
    vertexBuffer.destroyBuffer();

    // Destroy index buffer
    indexBuffer.destroyBuffer();

    // Destroy allocator
    vmaDestroyAllocator(allocator);

    // Destroy semaphores
    VK(vkDestroySemaphore(context.device, renderFinishedSemaphore, nullptr));
    VK(vkDestroySemaphore(context.device, imageAvailableSemaphore, nullptr));
    VK(vkDestroyFence(context.device, commandBufferFence, nullptr));

    // Destroy command buffers
    for (auto& commandBuffer : commandBuffers) {
        commandBuffer.destroyCommandBuffer();
    }

    // Destroy pipeline
    pipeline.destroyPipeline();

    // Destroy render pass
    renderPass.destroyRenderPass();

    // Destroy swapchain
    swapchain.destroySwapchain();

    // Destroy surface
    context.destroyVulkanContext();
}

void VulkanApp::render(cv::Mat& image) {
    LOG_INIT_CERR();

    ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

    static bool showDemoWindow = true;
	if(showDemoWindow) {
		ImGui::ShowDemoWindow(&showDemoWindow);
	}

    static float greenChannel = 0.0f;
	greenChannel += 0.01f;
	if (greenChannel > 1.0f) greenChannel = 0.0f;

	uint32_t imageIndex = 0;

    // Select command buffer
    static uint32_t bufferIndex = 0;
    VulkanCommandBuffer& commandBuffer = commandBuffers[bufferIndex];

    // Copy image data to buffer
    webcamBuffer.uploadImgBuffer(&copyCommandBuffer, &copyFence, &context, image.data, image.cols * image.rows * 4, { 640, 480, 1 });

    //Wait for command buffer to finish
    VKA(vkWaitForFences(context.device, 1, &commandBufferFence, VK_TRUE, UINT64_MAX));
    VKA(vkResetFences(context.device, 1, &commandBufferFence));

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

    VKA(vkResetCommandPool(context.device, commandBuffer.commandPool, 0));

    VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    VKA(vkBeginCommandBuffer(commandBuffer.commandBuffer, &beginInfo));
    {
        VkViewport viewport = { 0.0f, 0.0f, (float)swapchain.extent.width, (float)swapchain.extent.height };
        VkRect2D scissor = { {0, 0}, swapchain.extent };

        vkCmdSetViewport(commandBuffer.commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer.commandBuffer, 0, 1, &scissor);

        VkClearValue clearValue = { 1.0f, greenChannel, 0.0f, 1.0f };
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass.renderPass;
        renderPassInfo.framebuffer = renderPass.framebuffers[imageIndex];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapchain.extent;
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearValue;
        vkCmdBeginRenderPass(commandBuffer.commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.graphicsPipeline);
        
        VkDeviceSize offset = 0;
        vkCmdBindVertexBuffers(commandBuffer.commandBuffer, 0, 1, &vertexBuffer.Buffer, &offset);
        vkCmdBindIndexBuffer(commandBuffer.commandBuffer, indexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(commandBuffer.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

        vkCmdDrawIndexed(commandBuffer.commandBuffer, 6, 1, 0, 0, 0);

        ImGui::Render();
        ImDrawData* drawData = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(drawData, commandBuffer.commandBuffer);

        vkCmdEndRenderPass(commandBuffer.commandBuffer);
    }
    VKA(vkEndCommandBuffer(commandBuffer.commandBuffer));

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


int main(int, char**) {
    // Initiate logger (default name is 'log')
    LOG_INIT_CERR();

    // Init SDL and create a window
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        log(LOG_ERROR) << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    if (window == nullptr) {
        log(LOG_ERROR) << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    // Initiate Vulkan
    VulkanApp app;
    app.InitVulkan(window);

    cv::VideoCapture cap(0);
    cv::Mat webcamImage;

    // run Main Loop
    while (handleEvents()) {
        // Capture webcam image using OpenCV
        cap.read(webcamImage);

        // Convert the image format if necessary (e.g., from BGR to RGBA)
        cv::Mat convertedImage;
        cv::cvtColor(webcamImage, convertedImage, cv::COLOR_BGR2RGBA);

        // Render Vulkan
        app.render(convertedImage);
    }
    
    // Exit Vulkan
    app.ExitVulkan();

    // Destroy window and quit SDL
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}