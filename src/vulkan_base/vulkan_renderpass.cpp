#include "vulkan_base.hpp"

void VulkanRenderPass::createRenderPass(VulkanContext* context, VulkanSwapchain* swapchain) {
    this->context = context;
    this->swapchain = swapchain;

    LOG_INIT_CERR();
    log(LOG_INFO) << "Creating render pass...\n";

    // Create render pass
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = swapchain->format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // Clear framebuffer before rendering
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // Store framebuffer after rendering
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // Layout before render pass
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // Layout after render pass

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0; // Index of attachment
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Layout during render pass

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; // Pipeline type
    subpass.colorAttachmentCount = 1; // Number of color attachments
    subpass.pColorAttachments = &colorAttachmentRef; // Color attachment

    VkRenderPassCreateInfo renderPassCreateInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
    renderPassCreateInfo.attachmentCount = 1; // Number of attachments
    renderPassCreateInfo.pAttachments = &colorAttachment; // Attachments
    renderPassCreateInfo.subpassCount = 1; // Number of subpasses
    renderPassCreateInfo.pSubpasses = &subpass; // Subpasses

    VKA(vkCreateRenderPass(context->device, &renderPassCreateInfo, nullptr, &renderPass));

    // Create framebuffers
    log(LOG_INFO) << "Creating framebuffers...\n";
    framebuffers.resize(swapchain->imageViews.size());
    for (size_t i = 0; i < swapchain->imageViews.size(); i++) {
        VkFramebufferCreateInfo framebufferCreateInfo = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
        framebufferCreateInfo.renderPass = renderPass;
        framebufferCreateInfo.attachmentCount = 1;
        framebufferCreateInfo.pAttachments = &swapchain->imageViews[i];
        framebufferCreateInfo.width = swapchain->extent.width;
        framebufferCreateInfo.height = swapchain->extent.height;
        framebufferCreateInfo.layers = 1;

        VKA(vkCreateFramebuffer(context->device, &framebufferCreateInfo, nullptr, &framebuffers[i]));
    }
}

void VulkanRenderPass::destroyRenderPass() {
    LOG_INIT_CERR();



    log(LOG_INFO) << "Destroying framebuffers...\n";
    for (size_t i = 0; i < framebuffers.size(); i++) {
        VK(vkDestroyFramebuffer(context->device, framebuffers[i], nullptr));
    }
    framebuffers.clear();

    log(LOG_INFO) << "Destroying render pass...\n";
    VK(vkDestroyRenderPass(context->device, renderPass, nullptr));
}