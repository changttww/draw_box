#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 ourcolor;
layout(location = 2) in vec2 texCoord;

out vec3 OurColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    OurColor = ourcolor;
    TexCoord = texCoord;
}
