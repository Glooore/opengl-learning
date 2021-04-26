#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

struct Material 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform vec3 object_color;
uniform vec3 light_color;
uniform Material material;

void main()
{
	// ambient
	float ambient_strength = 0.1f;
	vec3 ambient = ambient_strength * light_color;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 light_direction = normalize(LightPos - FragPos);
	float diff = max(dot(norm, light_direction), 0.0);
	vec3 diffuse = diff * light_color;

	// specular
	float specular_strength = 0.5;
	vec3 view_direction = normalize(-FragPos);
	vec3 reflect_direction = reflect(-light_direction, norm);

	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
	vec3 specular = specular_strength * spec * light_color;

	vec3 result = (ambient + diffuse + specular) * object_color;
	FragColor = vec4(result, 1.0f);
}
