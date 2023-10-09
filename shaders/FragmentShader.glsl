#version 450 core

out vec4 FragColor;
in vec3 fragWorldPos;
in vec2 fragTexCoord;
in vec3 worldNormal;

uniform vec3 worldLightPos1;
uniform vec3 worldLightPos2;
uniform bool useNormalMap;
uniform sampler2D normalMap;
uniform sampler2D heightMap;

void main()
{
	vec3 normal = worldNormal;
	if(useNormalMap)
	{
		normal = texture(normalMap, fragTexCoord).rgb;
		normal = normalize(normal * 2.0 - 1.0).xzy;
	}

	vec3 lightDir = normalize(worldLightPos1 - fragWorldPos);
	float diff = max(dot(normal, lightDir), 0.0) * 0.6;

	lightDir = normalize(worldLightPos2 - fragWorldPos);
	diff += max(dot(normal, lightDir), 0.0) * 0.6;
	diff = clamp(diff, 0, 1);

	FragColor = vec4(diff, diff, diff, 1.0);
}