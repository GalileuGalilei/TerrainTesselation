#version 450 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 worldNormal;
out vec3 fragWorldPos;
out vec2 fragTexCoord;

in vec3 geoWorldPos[];
in vec2 geoTexCoord[];

vec3 CalculateNormal()
{
	vec3 edge1 = geoWorldPos[1] - geoWorldPos[0];
	vec3 edge2 = geoWorldPos[2] - geoWorldPos[0];
	return normalize(cross(edge1, edge2));
}

void main()
{
	vec3 faceNormal = CalculateNormal();
	for (int i = 0; i < 3; i++)
	{
		worldNormal = faceNormal;
		fragWorldPos = geoWorldPos[i];
		fragTexCoord = geoTexCoord[i];
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}
