#version 330 core

in vec3 outNormal;
in vec2 TexCoord;

uniform sampler2D texture0;
//uniform sampler2D texture2;

struct Ambient
{
    vec3 color;
    float str;
};

uniform Ambient ambient;

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

    vec3 norm = normalize(outNormal);

    vec4 texColor = texture(texture0, TexCoord);
    if (texColor.a < 0.1) discard;

    vec3 ambientResult = ambient.color * ambient.str;
    gl_FragColor = texColor * vec4(ambientResult, 1.0);
}