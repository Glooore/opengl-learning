#include <iostream>
#include <cmath>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "shader/shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

	float mix = 0.2;
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

	Shader ourShader("shaders/shader.vert", "shaders/shader.frag");

	// setting up vertex data
	float vertices[] = {
		0.5f, 0.5f,   0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f, // top right
		0.5f, -0.5f,  0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,// bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,// bottom left
		-0.5f, 0.5f,  0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f // top left
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

	/* // coords + colours */
	/* float trig1[] = { */
	/* 	-0.25f, -0.25f, 0.0f, 1.0f, 0.2f, 0.0f, */
	/* 	0.0f, 0.25f, 0.0f, 0.2f, 1.0f, 0.2f, */
	/* 	0.25f, -0.25f, 0.0f, 0.2f, 0.2f, 1.0f */
	/* }; */

	/* float texCoords[] = { */
	/* 	0.0f, 0.0f, */
	/* 	1.0f, 0.0f, */
	/* 	0.5f, 1.0f */
	/* } */

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char *data = stbi_load("./textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "TEXTURE::LOAD_FAIL" << std::endl;
	}

	stbi_image_free(data);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("./textures/face.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "TEXTURE::LOAD_FAIL" << std::endl;
	}

	stbi_image_free(data);




	// making VBO for first trig
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO); // vertex array object
	glGenBuffers(1, &VBO); // vertex buffer object
	glGenBuffers(1, &EBO); // element buffer object

	// VAO1 goes first
	glBindVertexArray(VAO);

	// binding and passing vertex data to VBO1
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/* glBufferData(GL_ARRAY_BUFFER, sizeof(two_trigs), two_trigs, GL_STATIC_DRAW); */

	// binding and passing indices to VBO1
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// configuring vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	/* glBindBuffer(GL_ARRAY_BUFFER, 0); */
	glBindVertexArray(0);

	/* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
	ourShader.use();
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	ourShader.setInt("texture2", 1);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input processing
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// color changing
		float timeValue = glfwGetTime();
		float offset_x = sin(timeValue) / 2.0f;
		/* float offset_x = 0; */

		/* glUseProgram(shaderProgram); */
		ourShader.setFloat("offset", offset_x);
		ourShader.setFloat("mix_f", mix);

		// first triangle
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);
		/* glDrawArrays(GL_TRIANGLES, 0, 6); */
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mix = mix + 0.1;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mix = mix - 0.1;
	}
}
