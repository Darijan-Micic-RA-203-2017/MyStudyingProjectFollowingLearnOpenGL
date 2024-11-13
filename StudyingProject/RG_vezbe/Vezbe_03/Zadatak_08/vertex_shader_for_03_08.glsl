#version 330 core

layout (location = 0) in vec2 aPos;

uniform float widthShrinkingFactor;
uniform float heightShrinkingFactor;

void main()
{
	gl_Position = vec4(widthShrinkingFactor * aPos.x, heightShrinkingFactor * aPos.y, 0.0f, 1.0f);
}
