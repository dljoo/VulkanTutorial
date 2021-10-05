﻿#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan.h>

#include <optional>
#include <string>
#include <vector>

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;		// Basic surface capabilities (min/max 이미지 개수 & 이미지 크기)
	std::vector<VkSurfaceFormatKHR> formats;	// Surface formats (pixel format, color space)
	std::vector<VkPresentModeKHR> presentModes;	// Available presentation modes
};

class HelloTriangleApplication
{
public:
	HelloTriangleApplication() {}
	~HelloTriangleApplication() {}

	void Run();

private:
	void initWindow();

	void initVulkan();

	void mainLoop();

	void cleanup();

	void cleanupSwapChain();

	void createCommandBuffers();

	void createCommandPool();

	void createFramebuffers();

	void createGraphicsPipeline();

	void createImageViews();

	void createInstance();

	void createLogicalDevice();

	void createRenderPass();

	void createSyncObjects();

	VkShaderModule createShaderModule(const std::vector<char>& _code);

	void createSurface();

	void createSwapChain();

	bool checkDeviceExtensionSupport(VkPhysicalDevice _device);
	
	void checkGLFWRequiredInstanceExtensionsSupported(std::vector<const char*> _extensions);
	
	bool checkValidationLayerSupport();

	// choose swap chain details
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& _capabilities);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& _availablePresentModes);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& _availableFormats);

	// Acquire an image from the swap chain
	// Execute the command buffer with that image as attachment in the framebuffer
	// Return the image to the swap chain for presentation
	void drawFrame();

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice _device);

	std::vector<const char*> getRequiredExtensions();

	bool isDeviceSuitable(VkPhysicalDevice _device);

	void pickPhysicalDevice();

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice _device);

	int rateDeviceSuitability(VkPhysicalDevice _device);

	void recreateSwapChain();

	void setupDebugMessenger();

	static std::vector<char> readFile(const std::string& _filename);

	// _messageSeverity : severity of message (verbose / info / warning / error) 
	// _messageType : type (general / validation / performance)
	// _pCallbackData : data (message, vulkan object, object count) 
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT _messageSeverity, VkDebugUtilsMessageTypeFlagsEXT _messageType, const VkDebugUtilsMessengerCallbackDataEXT* _pCallbackData, void* _pUserData);

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

private:
	GLFWwindow* window;

	VkInstance instance;

	VkDebugUtilsMessengerEXT debugMessenger;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	VkDevice device; // logical device

	VkQueue graphicsQueue;

	VkQueue presentQueue;

	VkSurfaceKHR surface;

	VkSwapchainKHR swapChain; 

	VkFormat swapChainImageFormat;

	VkExtent2D swapChainExtent;

	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;

	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;

	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkCommandPool commandPool;

	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	bool framebufferResized = false;
};

