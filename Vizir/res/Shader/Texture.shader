#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

out vec2 v_TextureCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	v_TextureCoords = texCoords;
};

#shader fragment
#version 330 core

in vec2 v_TextureCoords;
out vec4 fragColor;

uniform sampler2D tex;

void main()
{
	vec4 texColor = texture(tex, v_TextureCoords);
	fragColor = texColor;
};

