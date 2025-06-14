#version 450

layout( location = 0 ) in vec2 a_Position;
layout( location = 1 ) in vec2 a_TexCoords;
layout( location = 2 ) in vec2 a_WorldPos;
layout( location = 3 ) in vec4 a_Color;

void main() {
	gl_Position = vec4( a_Position, 0.0, 1.0 );
}