#version 460 core

layout (location=0) in vec3 aPos;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;
uniform vec4 uColor;

out vec4 fColor;

void main() {
	fColor = uColor;
	gl_Position = uProj * uView * uModel * vec4(aPos, 1.0f);
}