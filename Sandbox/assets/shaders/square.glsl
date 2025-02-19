#type vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 viewProjection;
uniform mat4 transform;

out vec3 v_position;

void main()
{
	v_position = position;
	gl_Position = viewProjection * transform * vec4(position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_position;

uniform vec3 u_color;

void main()
{
	color = vec4(u_color, 1.0);
}