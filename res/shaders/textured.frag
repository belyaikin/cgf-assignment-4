#version 330 core

in vec3 normal;
in vec3 currentPosition;
in vec2 texCoord;

out vec4 FragColor;

#define MAX_LIGHTS 4

uniform int numLights;
uniform vec4 lightColors[MAX_LIGHTS];
uniform vec3 lightPositions[MAX_LIGHTS];

uniform vec3 viewPosition;

// Material: the diffuse color is replaced by the texture color.
uniform sampler2D diffuseTexture;
uniform float ambientStrength;
uniform vec3 specularColor;
uniform float shininess;

void main()
{
   vec3 diffuseColor = texture(diffuseTexture, texCoord).rgb;

   vec3 n = normalize(normal);
   vec3 viewDir = normalize(viewPosition - currentPosition);

   // Two-sided lighting: light the side of the patch that faces the viewer.
   if (dot(n, viewDir) < 0.0)
      n = -n;

   vec3 color = ambientStrength * diffuseColor;

   for (int i = 0; i < numLights; i++)
   {
      vec3 lightColor = vec3(lightColors[i]);
      vec3 lightDir = normalize(lightPositions[i] - currentPosition);

      float diffuse = max(dot(n, lightDir), 0.0);

      vec3 reflectDir = reflect(-lightDir, n);
      float specular = diffuse > 0.0 ? pow(max(dot(viewDir, reflectDir), 0.0), shininess) : 0.0;

      color += diffuse * diffuseColor * lightColor + specular * specularColor * lightColor;
   }

   FragColor = vec4(color, 1.0);
}
