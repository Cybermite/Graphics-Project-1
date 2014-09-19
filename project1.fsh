#version 410 core

in float ldsX, ldsY; // Logical Device Space: -1 <= lds_ <= +1
in vec2 mcPositionToFS;
in vec2 relativePosToFS;
in vec4 mvColorToFS;
uniform int colorMode;
uniform int numberOfCircles;

// Replace the placeholder implementation here...

out vec4 fragmentColor;

//#define numberOfCircles 50
#define circleRadiusPercent 0.25
#define PI 3.1415926535

void factorOfColor(int circleCount, int numberOfCircles)
{
    fragmentColor = mvColorToFS*circleCount/float(numberOfCircles);
}

void preDefinedColors(int circleCount, int numberOfCircles)
{
    vec4 colors[4];
    colors[0] = vec4(0.0, 1.0, 0.0, 1.0);
    colors[1] = vec4(0.0, 0.0, 1.0, 1.0);
    colors[2] = vec4(0.5, 0.5, 0.5, 1.0);
    colors[3] = vec4(0.7, 0.7, 0.7, 1.0);
    
    fragmentColor = colors[circleCount%4];
}

void main()
{
    vec2 center = vec2(0.0, 0.0);
    vec2 origCenter = center;
    vec2 ldsVec2 = vec2( ldsX, ldsY );
    float radius = 0.3;
    int circleCount = 0;
    float radians = 0;

    for(int i = 0; i <= numberOfCircles; i++)
    {
        if(distance(center, relativePosToFS) <= radius)
        {
            circleCount++;
        }
        
        center = vec2(origCenter.x + cos(radians)*radius, origCenter.y + sin(radians)*radius);
        radians = radians + (2.0*PI/float(numberOfCircles));
    }

    if(circleCount == 0)
    {
        //color background
        fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);       
    }
    
    else
    {
        switch(colorMode){
            case 0:
                factorOfColor(circleCount, numberOfCircles);
                break;
            case 1:
                preDefinedColors(circleCount, numberOfCircles);
                break;
        }
    }
}

