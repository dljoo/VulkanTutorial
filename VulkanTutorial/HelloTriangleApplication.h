﻿#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;

	bool isComplete() { return graphicsFamily.has_value(); }
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

	void createInstance();

	void createLogicalDevice();
	
	void checkGLFWRequiredInstanceExtensionsSupported(std::vector<const char*> _extensions);
	
	bool checkValidationLayerSupport();

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice _device);

	std::vector<const char*> getRequiredExtensions();

	bool isDeviceSuitable(VkPhysicalDevice _device);

	void pickPhysicalDevice();

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo);

	int rateDeviceSuitability(VkPhysicalDevice _device);

	void setupDebugMessenger();

	// _messageSeverity : severity of message (verbose / info / warning / error) 
	// _messageType : type (general / validation / performance)
	// _pCallbackData : data (message, vulkan object, object count) 
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT _messageSeverity, VkDebugUtilsMessageTypeFlagsEXT _messageType, const VkDebugUtilsMessengerCallbackDataEXT* _pCallbackData, void* _pUserData);


private:
	GLFWwindow* window;

	VkInstance instance;

	VkDebugUtilsMessengerEXT debugMessenger;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	VkDevice device; // logical device

	VkQueue graphicsQueue;
};

