#version 330 core

in vec3 outNormal;
in vec2 TexCoord;
in vec3 fragPos;
uniform sampler2D texture0;

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

vec4 CalDiffuseColor(vec3 _lightPos, vec3 _fragPos, vec3 _norm, vec3 _lightColor, vec3 _ambientResult);

void main()
{
    vec3 lightPos = vec3(0.0f, 1.0f, 10.0f);
    vec3 lightColor = vec3(0.0, 1.0, 1.0); //if u change the blue color to zero then u dont see the diffuse
    vec4 texColor = texture(texture0, TexCoord);
    //if(texColor.a < 0.1) discard;
    vec3 norm = normalize(outNormal);


    vec3 ambientResult = (ambient.color * material.ambient) * ambient.str;
    //gl_FragColor = texColor * vec4(ambientResult,1.0);
    //gl_FragColor = material.color * vec4(ambientResult,1.0);
    gl_FragColor = CalDiffuseColor(lightPos, fragPos, norm, lightColor, ambientResult);

}
//_norm need be normalized before
vec4 CalDiffuseColor(vec3 _lightPos, vec3 _fragPos, vec3 _norm, vec3 _lightColor, vec3 _ambientResult)
{
    vec3 _lightDir = normalize(_lightPos - _fragPos);

    float _diff = max(dot(_norm, _lightDir), 0.0);
    vec3 _diffuse = _diff * _lightColor;
    return vec4((_ambientResult + _diffuse), 1.0) * material.color;


}