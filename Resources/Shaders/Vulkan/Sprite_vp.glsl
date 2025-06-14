#version 450 core

layout( location = 0 ) in vec2 a_Position;
layout( location = 1 ) in vec2 a_TexCoords;
layout( location = 2 ) in vec2 a_WorldPos;
layout( location = 3 ) in vec4 a_Color;

layout( location = 0 ) out vec2 v_TexCoords;
layout( location = 1 ) out vec2 v_WorldPos;
layout( location = 2 ) out vec4 v_Color;

layout( std140, binding = 0 ) uniform Matrix {
	mat4 u_ModelViewProjection;
};

void main() {
	v_TexCoords = a_TexCoords;
	v_WorldPos = v_WorldPos;
	v_Color = a_Color;

	gl_Position = u_ModelViewProjection * vec4( a_Position, 0.0, 1.0 );
}