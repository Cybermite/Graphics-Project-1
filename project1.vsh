#version 410 core

in vec2 mcPosition; // vertex position; "mc" stands for "model coordinates"
uniform vec4 scaleTrans; // for mapping coordinates into Logical Device Space

out float ldsX, ldsY;
out vec2 mc;

// replace the placeholder implementation here

void main()
{
	ldsX = scaleTrans[0]*mcPosition.x + scaleTrans[1];
	ldsY = scaleTrans[2]*mcPosition.y + scaleTrans[3];
	mc = mcPosition;
	gl_Position = vec4(ldsX, ldsY, 0, 1);
}

