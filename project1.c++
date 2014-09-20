// AUTHOR: ZANE RALSTON
// DATE: SEPTEMBER 19, 2014
// EMAIL: z641r311@ku.edu
// project1.c++

#include "GLFWController.h"
#include "ModelView.h"

int main(int argc, char* argv[])
{
    // define vertex coordinates for each box
    vec2 vertexPositions[][4] =
    {
        { {-12, 5 }, { -1, 5 }, {-12, 16}, { -1, 16} },
        { { -7, 16}, { -5, 18}, { -9, 18}, { -7, 20} },
        { {  6, 3 }, { 10, 3 }, {  6, 7 }, { 10, 7 } },
        { {  8, 7 }, { 13, 12}, {  3, 12}, {  8, 17} },
        { {  2,-11}, { 10,-11}, {  2, -3}, { 10, -3} },
        { {-12,-14}, { -6,-14}, {-12, -8}, { -6, -8} }
    };

    // define circle colors for each model.
    vec4 colors[][5] =
    {
        { { 0.2, 1.0, 0.5, 1.0 }, { 0.7, 0.2, 0.5, 1.0 }, { 1.0, 1.0, 0.2, 1.0 },
            { 0.2, 0.2, 0.2, 1.0 }, { 1.0, 1.0, 0.5, 1.0 } },
        { { 0.2, 0.5, 0.5, 1.0 }, { 0.2, 0.9, 0.2, 1.0 }, { 1.0, 0.2, 0.2, 1.0 },
            { 0.2, 0.0, 0.2, 1.0 }, { 0.6, 1.0, 0.5, 1.0 } },
        { { 0.1, 1.0, 0.7, 1.0 }, { 0.7, 0.2, 0.5, 1.0 }, { 0.9, 1.0, 0.6, 1.0 },
            { 0.1, 0.8, 0.6, 1.0 }, { 1.0, 1.0, 0.7, 1.0 } },
        { { 0.2, 0.0, 0.5, 1.0 }, { 0.0, 0.2, 1.0, 1.0 }, { 1.0, 0.3, 0.8, 1.0 },
            { 0.2, 0.0, 1.0, 1.0 }, { 1.0, 0.3, 0.2, 1.0 } },
        { { 0.5, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 }, { 1.0, 0.1, 0.3, 1.0 },
            { 0.7, 0.0, 0.5, 1.0 }, { 0.2, 0.3, 0.1, 1.0 } },
        { { 0.9, 0.0, 0.0, 1.0 }, { 0.8, 1.0, 1.0, 1.0 }, { 1.0, 0.8, 0.1, 1.0 },
            { 0.7, 0.9, 0.2, 1.0 }, { 0.9, 0.5, 1.0, 1.0 } }
    };

    GLFWController c(argv[0]);
    c.reportVersions(std::cout);

    for(int i = 0; i < 6; i++)
    {
        c.addModel( new ModelView(vertexPositions[i], colors[i] ) );
    }
    
    double xyz[6]; // xyz limits
    c.getOverallMCBoundingBox(xyz);
    ModelView::setMCRegionOfInterest(xyz);

    c.run();

    return 0;
}
