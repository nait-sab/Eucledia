#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textCoord;

uniform mat4 viewProjection;
uniform mat4 transform;

out vec2 v_textCoord;

void main()
{
	v_textCoord = textCoord;
	gl_Position = viewProjection * transform * vec4(position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_textCoord;

uniform vec4 u_color;
uniform float u_multiplier;
uniform sampler2D u_texture;

void main()
{
	color = u_color * texture(u_texture, v_textCoord * u_multiplier);
}