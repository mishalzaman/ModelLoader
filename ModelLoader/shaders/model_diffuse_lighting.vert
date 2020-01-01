#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// out
out vec3 fragmentPosition;
out vec3 normal;
out vec2 textureCoord;

// uniforms
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    fragmentPosition = vec3(model * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(model))) * aNormal;  
    textureCoord = aTexCoord;
    gl_Position = projection * view * vec4(fragmentPosition, 1.0);
}