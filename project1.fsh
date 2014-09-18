#version 410 core

in float ldsX, ldsY; // Logical Device Space: -1 <= lds_ <= +1
in vec2 mc;
uniform int colorMode;
uniform vec2 mvBounds[2];

// Replace the placeholder implementation here...

out vec4 fragmentColor;

#define numberOfCircles 6
#define circleRadiusPercent 0.25
#define PI 3.1415926535

void main()
{
    vec2 center = (mvBounds[0] + mvBounds[1])/2.0;
    vec2 origCenter = center;
    vec2 ldsVec2 = vec2( ldsX, ldsY );
    float radius = circleRadiusPercent*distance(mvBounds[0], mvBounds[1])/(2.0);
    unsigned int circleCount = 0;
    for(float radians = 0; radians < 2*PI; radians = radians + (2.0*PI/float(numberOfCircles)))
    {
        if(distance(center, mc) <= radius)
        {
            circleCount++;
        }
        
        center = vec2(origCenter.x + cos(radians)*radius, origCenter.y + sin(radians)*radius);
    }
    
    switch(circleCount)
    {
        case 1:
            fragmentColor = vec4(0.0, 1.0, 0.0, 1.0);
            break;
        case 2:
            fragmentColor = vec4(0.0, 0.0, 1.0, 1.0);
            break;
        case 3:
            fragmentColor = vec4(0.5, 0.5, 0.5, 1.0);
            break;
        case 4:
            fragmentColor = vec4(0.7, 0.7, 0.7, 1.0);
            break;
        default:
            fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
            break;
    }
}

