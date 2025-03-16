#type vertex
#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 textCoord;
layout(location = 3) in float textureIndex;
layout(location = 4) in float textureMultiplier;
layout(location = 5) in int entityID;

uniform mat4 viewProjection;

out vec4 v_color;
out vec2 v_textCoord;
out flat float v_textureIndex;
out float v_textureMultiplier;
out flat int v_entityID;

void main()
{
	v_color = color;
	v_textCoord = textCoord;
	v_textureIndex = textureIndex;
	v_textureMultiplier = textureMultiplier;
	v_entityID = entityID;
	gl_Position = viewProjection * vec4(position, 1.0);
}

#type fragment
#version 450

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

in vec4 v_color;
in vec2 v_textCoord;
in flat float v_textureIndex;
in float v_textureMultiplier;
in flat int v_entityID;

uniform sampler2D u_textures[32];

void main()
{
	color = texture(u_textures[int(v_textureIndex)], v_textCoord * v_textureMultiplier) * v_color;
	color2 = v_entityID;
}