#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 currentPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

vec4 calculatePerspective(vec3 position) {
   return proj * view * model * vec4(position, 1.0);
}

void main()
{
   currentPosition = vec3(model * vec4(aPos, 1.0f));

   gl_Position = calculatePerspective(aPos);

   normal = aNormal;
}