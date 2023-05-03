#version 330 core

in vec4 outColor;
in vec2 TexCoord;

uniform sampler2D texture0;
//uniform sampler2D texture2;

struct Ambient
{
    vec3 color;
};

uniform Ambient ambient;

void main() {
    //gl_FragColor = texture(texture,TexCoord); 
    vec4 texColor = texture(texture0, TexCoord);

    if (texColor.a < 0.1) //when escuchas algo en discord y funciona :D
        discard;
    //vec4 light = vec4(1.0, 0.5, 0.31,1.0);
    gl_FragColor = texColor * (outColor * vec4(ambient.color, 1.0));
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    //mix(texture(texture1, TexCoord),texture(texture2, TexCoord),0.2);
}