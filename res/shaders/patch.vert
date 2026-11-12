#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 3) in vec3 aCentroid;

flat out vec3 faceColor;

#define MAX_LIGHTS 4

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform int numLights;
uniform vec4 lightColors[MAX_LIGHTS];
uniform vec3 lightPositions[MAX_LIGHTS];

uniform vec3 viewPosition;
uniform vec3 diffuseColor;

const float ambientStrength = 0.1;

void main()
{
   vec3 center = vec3(model * vec4(aCentroid, 1.0));
   vec3 normal = normalize(mat3(transpose(inverse(model))) * aNormal);

   vec3 color = ambientStrength * diffuseColor;

   for (int i = 0; i < numLights; i++)
   {
      vec3 lightDir = normalize(lightPositions[i] - center);
      float diffuse = max(dot(normal, lightDir), 0.0);
      color += diffuse * diffuseColor * vec3(lightColors[i]);
   }

   faceColor = color;
   gl_Position = proj * view * model * vec4(aPos, 1.0);
}
