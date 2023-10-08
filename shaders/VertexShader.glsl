// vertex shader
#version 450 core

// vertex position
layout (location = 0) in vec3 aPos;
// texture coordinate
layout (location = 1) in vec2 aTex;
//In which mesh we are rendering
uniform int id;

out vec2 TexCoord;

vec2 GetSubTexCoord(vec2 mainTex)
{
    vec2 meshCoord = vec2(id % 3, id / 3);
    float offset = 1.0 / 3.0;

    float init = offset * meshCoord.x;
    float end = offset * (meshCoord.x + 1);
    float u = mix(init, end, mainTex.x);

    init = offset * meshCoord.y;
    end = offset * (meshCoord.y + 1);
    float v = mix(init, end, mainTex.y);

    return vec2(u, v);
}

void main()
{
    // convert XYZ vertex to XYZW homogeneous coordinate
    gl_Position = vec4(aPos, 1.0);
    // pass texture coordinate though
    TexCoord = GetSubTexCoord(aTex);
}