#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

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
		glfwDestroyWindow(window);

		glfwTerminate();
	}

private:
	GLFWwindow* window;
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