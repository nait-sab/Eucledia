#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

uniform mat4 viewProjection;
uniform mat4 transform;

out vec3 v_position;
out vec4 v_color;

void main()
{
	v_position = position;
	v_color = color;
	gl_Position = viewProjection * transform * vec4(position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_position;
in vec4 v_color;

void main()
{
	color = vec4(v_position * .5 + .5, 1.0);
	color = v_color;
}