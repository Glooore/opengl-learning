#include <iostream>
// for rand() that I haven't used.
#include <stdlib.h>
// for sin() and such that I haven't used
#include <cmath>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "shader/shader.h"
#include "camera/camera.h"

// matrix and vector types, a buncha useful stuff!
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// anonymous namespace so other files can't see it. Not really useful here.
namespace {
	const int window_width = 800;
	const int window_height = 600;

	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
	float last_x = window_width / 2.0f;
	float last_y = window_height / 2.0f;
	bool first_mouse = true;

	float delta_time = 0.0f;
	float last_frame = 0.0f;
	float current_frame = 0.0f;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double x_position, double y_position);
void process_input(GLFWwindow* window);

int main(int argc, char** argv)
{
	// standard GLFW initialization
	if (!glfwInit())
	{
		std::cout << "GLFW initialization failed" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// window creation
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "triangle", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// loading all GL functions (I think)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, window_width, window_height);
	// enabling z-buffer even though it isn't used
	glEnable(GL_DEPTH_TEST);

	// creating the shader object and compiling vertex and fragment shaders
	Shader shader("shaders/lighting.vert", "shaders/lighting.frag");
	Shader light_shader("shaders/lighting2.vert", "shaders/lighting2.frag");

	float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};// cube vertices

	// creating vertex array object and vertex buffer object
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// need to bind the VAO first to bind VBO to it, otherwise, doesn't work
	glBindVertexArray(VAO);

	// binding the VBO as a array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// filling the buffer data with vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// enabling vertex attribute (location 0, basically vertex coords). 
	// the attrib takes in a vec3 (array of three floats)
	// 3 * sizeof(float) -- stride of 3 
	// 0 -- first element
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);


	// enabling our shader
	shader.use();

	// render loop
	while(!glfwWindowShouldClose(window))
	{
		current_frame = glfwGetTime();

		glm::vec3 light_position = glm::vec3(1.2f, sin(current_frame), 2.0f);

		delta_time = current_frame - last_frame;
		last_frame = current_frame;
		
		process_input(window);
		/* camera.updateCameraVectors(); */

		glBindVertexArray(VAO);
		shader.use();
		shader.setVec3("object_color", glm::vec3(1.0f, 0.5f, 0.31f));
		shader.setVec3("light_color", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setVec3("light_position", light_position);

		// setting a gery background
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		// clearing color buffer and z buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// setting up a basic identity matrix
		glm::mat4 model = glm::mat4(1.0f);
		shader.setMat4("model", model);

		glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), 
					(float) window_width / (float) window_height, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 mvp = projection * view * model;
		shader.setMat4("mvp", mvp);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(lightVAO);
		light_shader.use();

		glm::mat4 light_model = glm::translate(glm::mat4(1.0f), light_position);
		light_model = glm::scale(light_model, glm::vec3(0.2f));
		mvp = projection * view * light_model;
		light_shader.setMat4("mvp", mvp);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VAO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{ 
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.processKeyboard(FORWARD, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.processKeyboard(BACKWARD, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.processKeyboard(RIGHT, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.processKeyboard(LEFT, delta_time);
	}
}

void mouse_callback(GLFWwindow* window, double x_position, double y_position)
{
	if (first_mouse)
	{
		last_x = x_position;
		last_y = y_position;
		first_mouse = false;
	}

	float x_offset = x_position - last_x;
	float y_offset = y_position - last_y;

	last_x = x_position;
	last_y = y_position;

	camera.processMouseMovement(x_offset, y_offset);
}

