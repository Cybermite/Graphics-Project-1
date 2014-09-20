// AUTHOR: ZANE RALSTON
// DATE: SEPTEMBER 19, 2014
// EMAIL: z641r311@ku.edu

#version 410 core

in vec2 mcPosition; // vertex position; "mc" stands for "model coordinates"
in vec2 relativePos;
uniform vec4 scaleTrans; // for mapping coordinates into Logical Device Space

out float ldsX, ldsY;
out vec2 mcPositionToFS;
out vec2 relativePosToFS;


void main()
{
	ldsX = scaleTrans[0]*mcPosition.x + scaleTrans[1];
	ldsY = scaleTrans[2]*mcPosition.y + scaleTrans[3];
    mcPositionToFS = mcPosition;
    relativePosToFS = relativePos;
	gl_Position = vec4(ldsX, ldsY, 0, 1);
}

