#version 410 core

in vec2 mcPosition; // vertex position; "mc" stands for "model coordinates"
in vec2 relativePos;
in vec4 mvColor;
uniform vec4 scaleTrans; // for mapping coordinates into Logical Device Space

out float ldsX, ldsY;
out vec2 mcPositionToFS;
out vec2 relativePosToFS;

// replace the placeholder implementation here

void main()
{
	ldsX = scaleTrans[0]*mcPosition.x + scaleTrans[1];
	ldsY = scaleTrans[2]*mcPosition.y + scaleTrans[3];
    mcPositionToFS = mcPosition;
    relativePosToFS = relativePos;
	gl_Position = vec4(ldsX, ldsY, 0, 1);
}

