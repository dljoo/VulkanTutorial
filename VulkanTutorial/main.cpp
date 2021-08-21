#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

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
	}

	void createInstance()
	{
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

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		checkGLFWRequiredInstanceExtensionsSupported(glfwExtensionCount, glfwExtensions); // challenge function == optional

		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		createInfo.enabledLayerCount = 0;

		// create instance
		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
			throw std::runtime_error("Failed to create instance!");
	}

	void checkGLFWRequiredInstanceExtensionsSupported(uint32_t _glfwExtensionCount, const char** _glfwExtensions)
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

		for (uint32_t i = 0; i < _glfwExtensionCount; ++i)
		{
			bool bSupported = false;
			for (const auto& extension : extensions)
			{
				if (strcmp(_glfwExtensions[i], extension.extensionName) == 0)
				{
					bSupported = true;
					break;
				}
			}

			if (bSupported == false)
			{
				bAllSupported = false;

				std::cout << "not supported glfw required extensions:\n";
				std::cout << '\t' << _glfwExtensions[i] << '\n';
			}
		}
		
		if (bAllSupported == true)
			std::cout << "All glfw required extensions are available!\n";
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
		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);

		glfwTerminate();
	}

private:
	GLFWwindow* window;

	VkInstance instance;
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