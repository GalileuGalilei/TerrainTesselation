#version 450 core

out vec4 FragColor;
in vec3 worldPos;
in vec3 worldNormal;
in vec2 TexCoord;
uniform vec3 worldLightPos;
uniform bool useNormalMap;
uniform sampler2D normalMap;
uniform sampler2D heightMap;

void main()
{
	vec3 normal = vec3(0,1,0);
	if(!useNormalMap)
	{
		vec3 ddx = dFdx(worldPos);
		vec3 ddy = dFdy(worldPos);
		normal = normalize(cross(ddx, ddy));
	}
	else
	{
		normal = texture(normalMap, TexCoord).rgb;
		normal = normalize(normal * 2.0 - 1.0).xzy;
	}

	vec3 lightDir = normalize(worldLightPos - worldPos);
	float diff = max(dot(normal, lightDir), 0.0);
	FragColor = vec4(diff, diff, diff, 1.0);
}