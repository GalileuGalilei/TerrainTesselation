#version 450
layout(vertices = 3) out;

in vec3 vPos[];
in vec2 vTexCoord[];
out vec2 tcTexCoord[];

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	gl_out[gl_InvocationID].gl_Position = vec4(vPos[gl_InvocationID], 1.0);
	tcTexCoord[gl_InvocationID] = vTexCoord[gl_InvocationID];

	if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = 1;
        gl_TessLevelOuter[1] = 1;
        gl_TessLevelOuter[2] = 1;

        gl_TessLevelInner[0] = 1;
        gl_TessLevelInner[1] = 1;
    }
}
