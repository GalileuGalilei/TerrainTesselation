#version 450 core

out vec4 FragColor;
//uniform float time; //temporariamente inutil
in vec2 tcTexCoord;
uniform sampler2D Texture0;

void main()
{
	FragColor = texture(Texture0, tcTexCoord);
	FragColor = vec4(FragColor.x, 0, 0, 1);
}