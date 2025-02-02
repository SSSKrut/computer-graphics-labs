#version 330 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoords;

uniform sampler2D texture1;

void main() {
    vec4 texColor = texture(texture1, TexCoords);
    FragColor = vec4(vertexColor, 1.0);
}