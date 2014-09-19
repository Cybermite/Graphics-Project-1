#version 410 core

in float ldsX, ldsY; // Logical Device Space: -1 <= lds_ <= +1
in vec2 mvMaxBoundsToFS;
in vec2 mvMinBoundsToFS;
in vec2 mcPositionToFS;
in vec4 mvColorToFS;
uniform int colorMode;
//uniform vec2 mvBounds[2];
//uniform vec4 mvColor;
uniform int numberOfCircles;

// Replace the placeholder implementation here...

out vec4 fragmentColor;

//#define numberOfCircles 50
#define circleRadiusPercent 0.25
#define PI 3.1415926535

void main()
{
    vec2 center = (mvMinBoundsToFS + mvMaxBoundsToFS)/2.0;
    vec2 origCenter = center;
    vec2 ldsVec2 = vec2( ldsX, ldsY );
    float radius = circleRadiusPercent*distance(mvMinBoundsToFS, mvMaxBoundsToFS)/(2.0);
    unsigned int circleCount = 0;
    float radians = 0;

    for(int i = 0; i <= numberOfCircles; i++)
    {
        if(distance(center, mcPositionToFS) <= radius)
        {
            circleCount++;
        }
        
        center = vec2(origCenter.x + cos(radians)*radius, origCenter.y + sin(radians)*radius);
        radians = radians + (2.0*PI/float(numberOfCircles));
    }

    /*
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
    }*/

    if(circleCount == 0)
    {
        fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);       
    }
    else
    {
        fragmentColor = mvColorToFS*circleCount/float(numberOfCircles);
    }
}

