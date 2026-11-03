#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec3 currentPosition;

#define MAX_LIGHTS 4

uniform int numLights;
uniform vec4 lightColors[MAX_LIGHTS];
uniform vec3 lightPositions[MAX_LIGHTS];

uniform vec3 viewPosition;

void main()
{
   vec3 norm = normalize(normal);

   vec3 totalDiffuse = vec3(0.0);

   for (int i = 0; i < numLights; i++)
   {
      vec3 lightDir = normalize(lightPositions[i] - currentPosition);
      float diffuse = max(dot(norm, lightDir), 0.0);

      totalDiffuse += diffuse * vec3(lightColors[i]);
   }

   vec3 finalColor = totalDiffuse;

   FragColor = vec4(finalColor, 1.0);
}