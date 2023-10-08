#version 450
// specify number of control points per patch output
// this value controls the size of the input and output arrays
layout (vertices=4) out;

// varying input from vertex shader
in vec2 TexCoord[];
// varying output to evaluation shader
out vec2 TextureCoord[];

//In which mesh we are rendering
uniform int id;
//array of tess level of all 9 meshes
uniform float tessLevel[9]; 

float FindSideTessLevel(int side)
{
    int dim = 3;
    vec2 coord = vec2(id % dim, id / dim);
    vec2 sideIndex;

    if(side == 0)
    {
        sideIndex = vec2(coord.x - 1, coord.y); 
    }

    if(side == 1)
	{
		sideIndex = vec2(coord.x, coord.y + 1); 
	}

    if(side == 2)
	{
		sideIndex = vec2(coord.x + 1, coord.y); 
	}

    if(side == 3)
	{
		sideIndex = vec2(coord.x, coord.y - 1); 
	}

	if(sideIndex.x < 0 || sideIndex.x >= dim || sideIndex.y < 0 || sideIndex.y >= dim)
	{
		return tessLevel[id]; //its own tess level
	}

	int index = int(sideIndex.y * dim + sideIndex.x);
	return tessLevel[index];

}

void main()
{
    float currentTessLevel = tessLevel[id];

    // ----------------------------------------------------------------------
    // pass attributes through
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    TextureCoord[gl_InvocationID] = TexCoord[gl_InvocationID];

    // ----------------------------------------------------------------------
    // invocation zero controls tessellation levels for the entire patch
    if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = currentTessLevel > FindSideTessLevel(0) ? currentTessLevel : FindSideTessLevel(0);
        gl_TessLevelOuter[1] = currentTessLevel > FindSideTessLevel(1) ? currentTessLevel : FindSideTessLevel(1);
        gl_TessLevelOuter[2] = currentTessLevel > FindSideTessLevel(2) ? currentTessLevel : FindSideTessLevel(2);
        gl_TessLevelOuter[3] = currentTessLevel > FindSideTessLevel(3) ? currentTessLevel : FindSideTessLevel(3);

        gl_TessLevelInner[0] = currentTessLevel;
        gl_TessLevelInner[1] = currentTessLevel;
    }
}