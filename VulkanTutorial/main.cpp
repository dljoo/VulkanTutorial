#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <optional>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

// valiatation layer
const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

struct QueueFamilyIndices 
{
	std::optional<uint32_t> graphicsFamily;

	bool isComplete() { return graphicsFamily.has_value(); }
};

VkResult CreateDebugUtilsMessengerEXT(VkInstance _instance, const VkDebugUtilsMessengerCreateInfoEXT* _pCreateInfo, const VkAllocationCallbacks* _pAllocator, VkDebugUtilsMessengerEXT* _pDebugMessenger) 
{
	// func is nullptr if "vkCreateDebugUtilsMessengerEXT" function couldn't be loaded.
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_instance, "vkCreateDebugUtilsMessengerEXT");

	if (func != nullptr) 
		return func(_instance, _pCreateInfo, _pAllocator, _pDebugMessenger);
	else 
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerEXT(VkInstance _instance, VkDebugUtilsMessengerEXT _debugMessenger, const VkAllocationCallbacks* _pAllocator) 
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_instance, "vkDestroyDebugUtilsMessengerEXT");

	if (func != nullptr) 
		func(_instance, _debugMessenger, _pAllocator);
}

class HelloTriangleApplication {
public:
	void run() 
	{
		initWindow();

		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	void initWindow() 
	{
		glfwInit(); // initialize GLFW library

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // not create OpenGL context

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // not allow resize

		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr); // create window
	}

	void initVulkan() 
	{
		createInstance();

		setupDebugMessenger();

		pickPhysicalDevice();
	}

	void pickPhysicalDevice() 
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if (deviceCount == 0) 
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		for (const auto& device : devices) 
		{
			if (isDeviceSuitable(device) == true) {
				physicalDevice = device;
				break;
			}
		}

		if (physicalDevice == VK_NULL_HANDLE) 
			throw std::runtime_error("failed to find a suitable GPU!");
	}

	int rateDeviceSuitability(VkPhysicalDevice _device)
	{
		VkPhysicalDeviceProperties deviceProperties; // name, type, vulkan version...
		vkGetPhysicalDeviceProperties(_device, &deviceProperties);

		VkPhysicalDeviceFeatures deviceFeatures; // optional features (texture compression, 64bit floats, multi viewport rendering)
		vkGetPhysicalDeviceFeatures(_device, &deviceFeatures);

		int score = 0;

		// Discrete GPUs have a significant performance advantage
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) 
			score += 1000;

		// Maximum possible size of textures affects graphics quality
		score += deviceProperties.limits.maxImageDimension2D;

		// Application can't function without geometry shaders
		if (deviceFeatures.geometryShader == false) 
			return 0;
		
		return score;
	}

	bool isDeviceSuitable(VkPhysicalDevice _device)
	{
		QueueFamilyIndices indices = findQueueFamilies(_device);

		return indices.isComplete();
	}

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice _device)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) 
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
				indices.graphicsFamily = i;

			if (indices.isComplete() == true)
				break;

			i++;
		}

		return indices;
	}

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo) 
	{
		_createInfo = {};
		_createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		_createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		_createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		_createInfo.pfnUserCallback = debugCallback;
	}

	void setupDebugMessenger() 
	{
		if (enableValidationLayers == false) 
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		populateDebugMessengerCreateInfo(createInfo);

		createInfo.pUserData = nullptr; // Optional

		// create VkDebugUtilsMessengerEXT(debugMessenger)
		if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) 
			throw std::runtime_error("failed to set up debug messenger!");
	}

	void createInstance()
	{
		// validation layer support check
		if (enableValidationLayers == true && checkValidationLayerSupport() == false) 
			throw std::runtime_error("validation layers requested, but not available!");


		VkApplicationInfo appInfo{}; // information about this application - optional
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{}; // not optional
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;


		// extension info
		std::vector<const char*> extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		checkGLFWRequiredInstanceExtensionsSupported(extensions); // challenge function == optional


		// validation layer info
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (enableValidationLayers == true)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();

			// debug util messenger before create instance
			populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		// create instance
		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
			throw std::runtime_error("Failed to create instance!");
	}

	void checkGLFWRequiredInstanceExtensionsSupported(std::vector<const char*> _extensions)
	{
		// check all supported extensions
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		std::cout << "available extensions:\n";

		for (const auto& extension : extensions) 
		{
			std::cout << '\t' << extension.extensionName << '\n';
		}

		bool bAllSupported = true;

		for (uint32_t i = 0; i < static_cast<uint32_t>(_extensions.size()); ++i)
		{
			bool bSupported = false;
			for (const auto& extension : extensions)
			{
				if (strcmp(_extensions[i], extension.extensionName) == 0)
				{
					bSupported = true;
					break;
				}
			}

			if (bSupported == false)
			{
				bAllSupported = false;

				std::cout << "not supported glfw required extensions:\n";
				std::cout << '\t' << _extensions[i] << '\n';
			}
		}
		
		if (bAllSupported == true)
			std::cout << "All glfw required extensions are available!\n" << std::endl;
	}

	bool checkValidationLayerSupport() 
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) 
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
				return false;
		}

		return true;
	}

	std::vector<const char*> getRequiredExtensions() 
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		// add debug extension
		if (enableValidationLayers == true) 
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		return extensions;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT _messageSeverity,		// severity of message (verbose / info / warning / error)
		VkDebugUtilsMessageTypeFlagsEXT _messageType,				// type (general / validation / performance)
		const VkDebugUtilsMessengerCallbackDataEXT* _pCallbackData,  // data (message, vulkan object, object count)
		void* pUserData) 
	{
		std::string severity = "";
		switch (_messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			severity = "INFO";
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			severity = "INFO";
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			severity = "WARNING";
			break;		
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			severity = "ERROR";
			break;

		}

		std::cerr << "validation layer: " << "[" << severity << "]" << _pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	void mainLoop() 
	{
		while (glfwWindowShouldClose(window) == false) // glfw window loop + render loop
		{
			glfwPollEvents();
		}
	}

	void cleanup() 
	{
		if (enableValidationLayers == true)
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);

		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);

		glfwTerminate();
	}

private:
	GLFWwindow* window;

	VkInstance instance;

	VkDebugUtilsMessengerEXT debugMessenger;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
};

int main() 
{
	HelloTriangleApplication app;

	try 
	{
		app.run();
	}
	catch (const std::exception& e) 
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}