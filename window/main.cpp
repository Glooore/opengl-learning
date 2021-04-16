#include <iostream>
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

/* void framebuffer_size_callback(GLFWwindow* window, int width, int height); */

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	//initializes GLFW
	if ( !glfwInit() )
	{
		std::cout << "Initialization failed" << std::endl;
		return -1;
	}

	// integer part of the version -- 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// decimal part of the verson -- 0.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// both mean 3.3
	
	// use subset of all OpenGL features (?)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// creates 800x600 window named "opengl"
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "opengl", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// set the lower left coord (0, 0) and the top right coord (800, 600)
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	// set resizing 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	while (!glfwWindowShouldClose(window))
	{
		// input processing
		processInput(window);

		// rendering

		/* glClearColor(color_float_thingy, color_float_thingy, color_float_thingy, 1.0f); */
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// whatever
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	std::cout << "width " << width << std::endl;
	std::cout << "height " << height << std::endl;
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	// if ESC is pressed, close the window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
