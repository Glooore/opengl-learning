#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

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

	// declaring vertex shader (used for getting (and maybe transforming) the vertexes
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	// declaring fragment shader (used for colouring and such)
	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(0.5f, 0.5f, 0.7f, 1.0f);\n"
		"}\0";

	const char *fragmentShaderSource2 = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(0.7f, 0.7f, 0.5f, 1.0f);\n"
		"}\0";
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

	// variables for getting error message and other stuff
	int success;
	char infoLog[512];

	// BUILDING AND CREATING SHADER PROGRAM

	// creating a vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	// get error message if compilation failed
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// creating a fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// creating a shader program
	unsigned int shaderProgram1;
	shaderProgram1 = glCreateProgram();

	glAttachShader(shaderProgram1, vertexShader); // link vertex
	glAttachShader(shaderProgram1, fragmentShader); // link fragment
	glLinkProgram(shaderProgram1);

	// checking for linking errors
	glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	glShaderSource(fragmentShader, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader);

	// creating a shader program
	unsigned int shaderProgram2;
	shaderProgram2 = glCreateProgram();

	glAttachShader(shaderProgram2, vertexShader); // link vertex
	glAttachShader(shaderProgram2, fragmentShader); // link fragment
	glLinkProgram(shaderProgram2);

	// checking for linking errors
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	//cleanup after linking
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// setting up vertex data
	float vertices[] = {
		0.5f, 0.5f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f // top left
	};

	float trig1[] = {
		-0.5f, -0.5f, 0.0f,
		-0.25f, 0.5f, 0.0f,
		0.0f, -0.5f, 0.0f
	};
	float trig2[] = {
		0.0f, 0.5f, 0.0f,
		0.25f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};
	// setting up indices for tiangles
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};


	// making VBO for first trig
	unsigned int VBO1, VAO1, EBO1;
	glGenVertexArrays(1, &VAO1); // vertex array object
	glGenBuffers(1, &VBO1); // vertex buffer object
	glGenBuffers(1, &EBO1); // element buffer object

	// VAO1 goes first
	glBindVertexArray(VAO1);

	// binding and passing vertex data to VBO1
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trig1), trig1, GL_STATIC_DRAW);
	/* glBufferData(GL_ARRAY_BUFFER, sizeof(two_trigs), two_trigs, GL_STATIC_DRAW); */

	// binding and passing indices to VBO1
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// configuring vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	/* glBindBuffer(GL_ARRAY_BUFFER, 0); */
	glBindVertexArray(0);

	unsigned int VBO2, VAO2, EBO2;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trig2), trig2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

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

		glUseProgram(shaderProgram1);

		// first triangle
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgram2);

		// second triangle
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		/* glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); */

		// show pixels on window
		glfwSwapBuffers(window);
		// check for key pressess
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &EBO1);

	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);

	glDeleteProgram(shaderProgram1);
	glDeleteProgram(shaderProgram2);


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
