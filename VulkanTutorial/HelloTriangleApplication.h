#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan.h>

#include <array>
#include <optional>
#include <string>
#include <vector>
#include <glm/glm.hpp>

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

struct UniformBufferObject 
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct Vertex
{
	glm::vec2 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	static VkVertexInputBindingDescription getBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
	{
		// attributeDescriptions[0] - position
		// attributeDescriptions[1] - color
		// attributeDescriptions[2] - texCoord
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}
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

	VkCommandBuffer beginSingleTimeCommands();

	void cleanup();

	void cleanupSwapChain();

	void createBuffer(VkDeviceSize _size, VkBufferUsageFlags _usage, VkMemoryPropertyFlags _properties, VkBuffer& _buffer, VkDeviceMemory& _bufferMemory);

	void createCommandBuffers();

	void createCommandPool();

	void createDescriptorPool(); 
	void createDescriptorSetLayout();
	void createDescriptorSets();

	void createFramebuffers();

	void createGraphicsPipeline();

	void createImageViews();

	VkImageView createImageView(VkImage _image, VkFormat _format);

	void createInstance();

	void createLogicalDevice();

	void createRenderPass();

	void createSyncObjects();

	VkShaderModule createShaderModule(const std::vector<char>& _code);

	void createSurface();

	void createSwapChain();

	void createVertexBuffer();

	void createIndexBuffer();

	void createTextureImage();

	void createTextureImageView();

	void createTextureSampler();

	void createImage(uint32_t _width, uint32_t _height, VkFormat _format, VkImageTiling _tiling, VkImageUsageFlags _usage, VkMemoryPropertyFlags _properties, VkImage& _image, VkDeviceMemory& _imageMemory);

	void createUniformBuffers();

	void copyBuffer(VkBuffer _srcBuffer, VkBuffer _dstBuffer, VkDeviceSize _size);

	void copyBufferToImage(VkBuffer _buffer, VkImage _image, uint32_t _width, uint32_t _height);

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

	void endSingleTimeCommands(VkCommandBuffer _commandBuffer);

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice _device);

	std::vector<const char*> getRequiredExtensions();

	bool isDeviceSuitable(VkPhysicalDevice _device);

	void pickPhysicalDevice();

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice _device);

	int rateDeviceSuitability(VkPhysicalDevice _device);

	void recreateSwapChain();

	void setupDebugMessenger();

	void transitionImageLayout(VkImage _image, VkFormat _format, VkImageLayout _oldLayout, VkImageLayout _newLayout);

	void updateUniformBuffer(uint32_t _currentImage);

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

	VkDescriptorPool descriptorPool;
	VkDescriptorSetLayout descriptorSetLayout; 
	std::vector<VkDescriptorSet> descriptorSets;

	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;

	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkCommandPool commandPool;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	VkImage textureImage;
	VkImageView textureImageView;
	VkSampler textureSampler;
	VkDeviceMemory textureImageMemory;

	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	bool framebufferResized = false;
};

