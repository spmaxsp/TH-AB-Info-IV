#include "vulkan_base.hpp"

VkShaderModule VulkanPipeline::createShaderModule(const char* shaderFilename){
    LOG_INIT_CERR();

    log(LOG_INFO) << "Creating Shader Module: " << shaderFilename << "\n	";
    // Read Shader File
    std::ifstream file(shaderFilename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        log(LOG_ERROR) << "Failed to open shader file: " << shaderFilename << "\n	";
        throw std::runtime_error("Failed to open shader file");
    }
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    // Create Shader Module
    VkShaderModuleCreateInfo createInfo{ VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
    createInfo.codeSize = buffer.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());
    VkShaderModule shaderModule = VK_NULL_HANDLE;
    VKA(vkCreateShaderModule(context->device, &createInfo, nullptr, &shaderModule));

    return shaderModule;
}

void VulkanPipeline::createPipeline(VulkanContext* context, VulkanRenderPass* renderPass, const char* vertexShaderFilename, const char* fragmentShaderFilename, VkExtent2D extent) {
    LOG_INIT_CERR();

    this->context = context;
    this->renderPass = renderPass;

    // Create Shader Modules
    VkShaderModule vertexShaderModule = createShaderModule(vertexShaderFilename);
    VkShaderModule fragmentShaderModule = createShaderModule(fragmentShaderFilename);

    // Create Shader Stages
    VkPipelineShaderStageCreateInfo vertexShaderStageInfo = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
    vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageInfo.module = vertexShaderModule;
    vertexShaderStageInfo.pName = "main";
    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
    fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageInfo.module = fragmentShaderModule;
    fragmentShaderStageInfo.pName = "main";
    int shaderStageCount = 2;
    VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageInfo, fragmentShaderStageInfo };

    // Create Vertex Input
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };

    // Create Input Assembly
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    // Create Viewport
    VkPipelineViewportStateCreateInfo viewportState = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    // Create Rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer = { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
    rasterizer.lineWidth = 1.0f;

    // Create Multisampling
    VkPipelineMultisampleStateCreateInfo multisampling = { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    // Create Color Blending
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo colorBlending = { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    // Create Pipeline Layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
    VKA(vkCreatePipelineLayout(context->device, &pipelineLayoutInfo, nullptr, &pipelineLayout));

    // Create Dynamic State
    VkPipelineDynamicStateCreateInfo dynamicState = {VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
	std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    dynamicState.dynamicStateCount = dynamicStates.size();
	dynamicState.pDynamicStates = dynamicStates.data();

    // Create Graphics Pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
    pipelineInfo.stageCount = shaderStageCount;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass->renderPass;
    pipelineInfo.subpass = 0;

    VKA(vkCreateGraphicsPipelines(context->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline));

    // Destroy Shader Modules
    VK(vkDestroyShaderModule(context->device, vertexShaderModule, nullptr));
    VK(vkDestroyShaderModule(context->device, fragmentShaderModule, nullptr));
}

void VulkanPipeline::destroyPipeline() {
    LOG_INIT_CERR();

    log(LOG_INFO) << "Destroying Pipeline" << "\n	";
    VK(vkDestroyPipeline(context->device, graphicsPipeline, nullptr));
    VK(vkDestroyPipelineLayout(context->device, pipelineLayout, nullptr));
}