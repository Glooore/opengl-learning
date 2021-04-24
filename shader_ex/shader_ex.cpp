#include <iostream>
#include <cmath>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "shader/shader.h"

// function prototypes for resizing the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// function prototype for processing input (basically, check if escape is pressed)
void processInput(GLFWwindow* window);

// anonymous namespace -- global but cannot be accessed by other files
// extern will fail with 'undefined reference' or so
namespace {
	// initializing window dimensions
	static const int window_width = 800;
	static const int window_height = 600;
}

int main()
{
	//initializes GLFW
	if ( !glfwInit() )
	{
		// if initalization fails, print out
		// could probably print to std::cerr?
		std::cout << "Initialization failed" << std::endl;
		return -1;
	}

	// integer part of the version -- 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// decimal part of the verson -- 0.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// use subset of all OpenGL features (?)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// creates 800x600 window named "opengl"
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "opengl", NULL, NULL);
	if (window == NULL)
	{
		// if the pointer is NULL (the window wasn't created), print error
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// kinda dun know yet
	glfwMakeContextCurrent(window);

	// 'glad' makes getting the process ID of the window much easier (and portable!)
	// loads all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Shader ourShader("shaders/shader.vs", "shaders/shader.fs");

	// setting up vertex data
	/* float vertices[] = { */
	/* 	0.5f, 0.5f, 0.0f, // top right */
	/* 	0.5f, -0.5f, 0.0f, // bottom right */
	/* 	-0.5f, -0.5f, 0.0f, // bottom left */
	/* 	-0.5f, 0.5f, 0.0f // top left */
	/* }; */

	// coords + colours
	float trig1[] = {
		-0.25f, -0.25f, 0.0f, 1.0f, 0.2f, 0.0f,
		0.0f, 0.25f, 0.0f, 0.2f, 1.0f, 0.2f,
		0.25f, -0.25f, 0.0f, 0.2f, 0.2f, 1.0f
	};
	/* float trig2[] = { */
	/* 	0.0f, 0.5f, 0.0f, */
	/* 	0.25f, -0.5f, 0.0f, */
	/* 	0.5f, 0.5f, 0.0f */
	/* }; */
	// setting up indices for tiangles
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};


	// making VBO for first trig
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO); // vertex array object
	glGenBuffers(1, &VBO); // vertex buffer object
	glGenBuffers(1, &EBO); // element buffer object

	// VAO1 goes first
	glBindVertexArray(VAO);

	// binding and passing vertex data to VBO1
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trig1), trig1, GL_STATIC_DRAW);
	/* glBufferData(GL_ARRAY_BUFFER, sizeof(two_trigs), two_trigs, GL_STATIC_DRAW); */

	// binding and passing indices to VBO1
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// configuring vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/* glBindBuffer(GL_ARRAY_BUFFER, 0); */
	glBindVertexArray(0);

	/* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input processing
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// color changing
		float timeValue = glfwGetTime();
		/* float redValue = (sin(timeValue - 2) / 2.0f) + 0.5f; */
		/* float greenValue = (sin(timeValue) / 2.0f) + 0.5f; */
		/* float blueValue = (sin(timeValue + 2) / 2.0f) + 0.5f; */
		/* int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); */
		float offset_x = sin(timeValue) / 2.0f;

		/* glUseProgram(shaderProgram); */
		ourShader.use();
		ourShader.setFloat("offset", offset_x);

		// first triangle
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// show pixels on window
		glfwSwapBuffers(window);
		// check for key pressess
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	/* std::cout << "width " << width << std::endl; */
	/* std::cout << "height " << height << std::endl; */
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
