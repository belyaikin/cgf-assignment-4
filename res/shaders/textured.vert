#version 330 core

// Phong-shaded, textured Bezier patch: lighting is evaluated per fragment from
// interpolated smooth normals. The texture coordinate is the patch (u, v).

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 normal;
out vec3 currentPosition;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
   currentPosition = vec3(model * vec4(aPos, 1.0));
   normal = mat3(transpose(inverse(model))) * aNormal;
   texCoord = aTexCoord;

   gl_Position = proj * view * vec4(currentPosition, 1.0);
}
