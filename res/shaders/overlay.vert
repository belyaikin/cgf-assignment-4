#version 330 core

// Unlit, per-vertex colored geometry (control points and axes).

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float pointSize;

void main()
{
   color = aColor;
   gl_PointSize = pointSize;
   gl_Position = proj * view * model * vec4(aPos, 1.0);
}
