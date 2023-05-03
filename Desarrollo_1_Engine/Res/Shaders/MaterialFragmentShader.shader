#version 330 core

in vec3 outNormal;
in vec2 TexCoord;

out vec4 outColor;
out vec2 TexCoord;
uniform sampler2D texture0;
//uniform sampler2D texture2;

struct Light
{
	vec3 ambient;
};

uniform Light light;

struct Material
{
	vec4 color;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

void main() {
	vec3 norm = normalize(OutNormal);

	vec3 norm = normalize(outNormal);

	vec4 texColor = texture(texture0, TexCoord);
	vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
	if (texColor.a < 0.1) //when escuchas algo en discord y funciona :D
		discard;
	//vec4 light = vec4(1.0, 0.5, 0.31,1.0);
	gl_FragColor = texColor * vec4(light.ambient, 1.0);
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//mix(texture(texture1, TexCoord),texture(texture2, TexCoord),0.2);
}