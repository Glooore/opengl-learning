#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 mvp;
uniform mat4 view;
uniform mat4 model;

uniform vec3 light_position;

out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;

void main()
{
	gl_Position = mvp * vec4(aPos, 1.0f);
	FragPos = vec3(view * model * vec4(aPos, 1.0f));
	Normal = mat3(transpose(inverse(view * model))) * aNormal;
	LightPos = vec3(view * vec4(light_position, 1.0f));
}
