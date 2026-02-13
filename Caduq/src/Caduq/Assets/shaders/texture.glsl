#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoords;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
	v_TexCoords = a_TexCoords;
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

in vec2 v_TexCoords;

uniform sampler2D u_Texture;
uniform float u_TilingFactor;
uniform vec4 u_Color;
		
void main()
{
	color = texture(u_Texture, v_TexCoords * u_TilingFactor) * u_Color;
}