#version 410 core

layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inColor;

uniform mat4 MVP;

out vec3 vertColor;

void main()
{
    gl_Position = MVP * vec4(inVertex, 1.0);
    vertColor = inColor;
}
