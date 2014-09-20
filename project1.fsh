#version 410 core

in vec2 mcPositionToFS;
in vec2 relativePosToFS;
uniform int colorMode;
uniform vec4 userColors[5];

// Replace the placeholder implementation here...

out vec4 fragmentColor;

#define numberOfCirclesAround 6
#define circleRadiusPercent 0.25
#define PI 3.1415926535

void factorOfColor(int circleCount)
{
    if(circleCount == 0)
    {
        fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);
        return;
    }
    fragmentColor = userColors[3]*circleCount/float(numberOfCirclesAround);
}

void preDefinedColors(int circleCount)
{
    if(circleCount == 0)
    {
        fragmentColor = vec4(0.0, 1.0, 0.0, 1.0);
        return;
    }

    vec4 colors[4];
    colors[0] = vec4(1.0, 0.0, 0.0, 1.0);
    colors[1] = vec4(0.0, 0.0, 1.0, 1.0);
    colors[2] = vec4(0.5, 0.5, 0.5, 1.0);
    colors[3] = vec4(0.7, 0.7, 0.7, 1.0);
    
    fragmentColor = colors[circleCount%4];
}

void userDefinedColors(int circleCount)
{
    fragmentColor = userColors[circleCount];
}

void main()
{
    vec2 center = vec2(0.0, 0.0);
    vec2 origCenter = center;
    float radius = 0.3;
    int circleCount = 0;
    float radians = 0;

    for(int i = 0; i <= numberOfCirclesAround; i++)
    {
        if(distance(center, relativePosToFS) <= radius)
        {
            circleCount++;
        }
        
        center = vec2(origCenter.x + cos(radians)*radius, origCenter.y + sin(radians)*radius);
        radians = radians + (2.0*PI/float(numberOfCirclesAround));
    }

    switch(colorMode){
        case 0:
            userDefinedColors(circleCount);
            break;
        case 1:
            factorOfColor(circleCount);
            break;
        case 2:
            preDefinedColors(circleCount);
            break;
    }
}

